#include "stdafx.h"

// Includes
#include "Wmo_Fog.h"
#include "Wmo_Light.h"
#include "Wmo_Material.h"

// General
#include "wmo.h"

// Additional
#include "Wmo_Group.h"

#include "liquid.h"

WMO::WMO(cstring name) : RefItemNamed(name)
{
	skybox = nullptr;
}

WMO::~WMO()
{
	Debug::Info("WMO[%s]: Unloading...", GetName().c_str());

	delete[] texbuf;
	delete[] groupnames;

	for (auto it = mat.begin(); it != mat.end(); ++it)
	{
		delete *it;
	}

#ifdef MDX_INCL
	for (vector<string>::iterator it = m_MDXNames.begin(); it != m_MDXNames.end(); ++it)
	{
		_ModelsMgr->Delete(*it);
	}
#endif

	//if(mat) bouzi
	//	delete[] mat;

	if (skybox)
	{
		//delete skybox;
		//_ModelsMgr->del(sbid);
	}
}

bool WMO::Init()
{
	File f = GetName();

	if (!f.Open())
	{
		Debug::Info("WMO[%s]: Error loading WMO.", GetName().c_str());
		return false;
	}

	Debug::Info("WMO[%s]: Loading...", GetName().c_str());

	char fourcc[5];
	uint32_t size;
	while (!f.IsEof())
	{
		memset(fourcc, 0, 4);
		f.ReadBytes(fourcc, 4);
        flipcc(fourcc);
		fourcc[4] = 0;

		size = 0;
		f.ReadBytes(&size, 4);
		if (size == 0) continue;
		size_t nextpos = f.GetPos() + size;

		if (strcmp(fourcc, "MVER") == 0)                    // Version
		{
			uint32_t version;
			f.ReadBytes(&version, 4);
			assert3(version == 17, "Version mismatch != 17", std::to_string(version).c_str());
		}
		else if (strcmp(fourcc, "MOHD") == 0)               // Header
		{
			f.ReadBytes(&header, header.__size);
		}
		else if (strcmp(fourcc, "MOTX") == 0)               // List of textures (BLP Files) used in this map object.
		{
			texbuf = new char[size + 1];
			f.ReadBytes(texbuf, size);
			texbuf[size] = 0x00;
		}
		else if (strcmp(fourcc, "MOMT") == 0)               // Materials used in this map object, 64 bytes per texture (BLP file), nMaterials entries.
		{
			for (uint32_t i = 0; i < header.nTextures; i++)
			{
				WMOMaterial* _mat = new WMOMaterial(f);
				_mat->initTexture(texbuf);
				mat.push_back(_mat);
			}
		}
		else if (strcmp(fourcc, "MOGN") == 0)              // List of group names for the groups in this map object.
		{
			groupnames = new char[size + 1];
			f.ReadBytes(groupnames, size);
			groupnames[size] = 0x00;
		}
		else if (strcmp(fourcc, "MOGI") == 0)
		{
			for (uint32_t i = 0; i < header.nGroups; i++)
			{
				auto group = new WMOGroup;
				group->init(this, f, i, groupnames);
				groups.push_back(group);
			}
		}
		else if (strcmp(fourcc, "MOSB") == 0)
		{
			/*
			Skybox. Always 00 00 00 00. Skyboxes are now defined in DBCs (Light.dbc etc.). Contained a M2 filename that was used as skybox.
			*/
			if (size > 4)
			{
				string path = string((char*)f.GetDataFromCurrent());
				if (path.length())
				{
					Debug::Warn("SKYBOX:");

#ifdef MDX_INCL
					skybox = _ModelsMgr->Add(path);

					if (!skybox->ok)
					{
						_ModelsMgr->Delete(path);
						skybox = nullptr;
					}

#endif
				}
			}
		}
		else if (strcmp(fourcc, "MOPV") == 0)
		{
			/*
			Portal vertices, 4 * 3 * float per portal, nPortals entries.
			Portals are (always?) rectangles that specify where doors or entrances are in a WMO. They could be used for visibility, but I currently have no idea what relations they have to each other or how they work.
			Since when "playing" WoW, you're confined to the ground, checking for passing through these portals would be enough to toggle visibility for indoors or outdoors areas, however, when randomly flying around, this is not necessarily the case.
			So.... What happens when you're flying around on a gryphon, and you fly into that arch-shaped portal into Ironforge? How is that portal calculated? It's all cool as long as you're inside "legal" areas, I suppose.
			It's fun, you can actually map out the topology of the WMO using this and the MOPR chunk. This could be used to speed up the rendering once/if I figure out how.
			*/
			WMO_PortalVertices p;
			for (uint32_t i = 0; i < header.nPortals; i++)
			{
				float ff[3];

				f.ReadBytes(ff, 12);
				p.a = vec3(ff[0], ff[2], -ff[1]);

				f.ReadBytes(ff, 12);
				p.b = vec3(ff[0], ff[2], -ff[1]);

				f.ReadBytes(ff, 12);
				p.c = vec3(ff[0], ff[2], -ff[1]);

				f.ReadBytes(ff, 12);
				p.d = vec3(ff[0], ff[2], -ff[1]);

				m_PortalVertices.push_back(p);
			}
		}
		else if (strcmp(fourcc, "MOPT") == 0)
		{
			/*
			Portal information. 20 bytes per portal, nPortals entries.
			Offset	Type 		Description
			0x00 	uint16_t 		Base vertex index?
			0x02 	uint16_t 		Number of vertices (?), always 4 (?)
			0x04 	3*float 	a normal vector maybe? haven't checked.
			0x10 	float 		unknown  - if this is NAN, the three floats will be (0,0,1)
			*/
		}
		else if (strcmp(fourcc, "MOPR") == 0)
		{
			uint32_t nn = size / WMO_PortalReferences::__size;
			WMO_PortalReferences* pr = (WMO_PortalReferences*)f.GetDataFromCurrent();
			for (uint32_t i = 0; i < nn; i++)
			{
				m_PortalReferences.push_back(*pr++);
			}
		}
		else if (strcmp(fourcc, "MOVV") == 0)
		{
			/*
			Visible block vertices
			Just a list of vertices that corresponds to the visible block list.
			*/
		}
		else if (strcmp(fourcc, "MOVB") == 0)
		{
			/*
			Visible block list
			unsigned short firstVertex;
			unsigned short count;
			*/
		}
		else if (strcmp(fourcc, "MOLT") == 0)
		{
			for (uint32_t i = 0; i < header.nLights; i++)
			{
				WMOLight l(f);
				lights.push_back(l);
			}
		}
		else if (strcmp(fourcc, "MODS") == 0)
		{
			for (uint32_t i = 0; i < header.nDoodadSets; i++)
			{
#ifdef MDX_INCL
				WMO_DoodadSet dds;
				f.ReadBytes(&dds, WMO_DoodadSet::__size);
				doodadsets.push_back(dds);
#else
				f.SeekRelative(WMO_DoodadSet::__size);
#endif
			}
		}
		else if (strcmp(fourcc, "MODN") == 0)
		{
			/*
			List of filenames for M2 (mdx) models that appear in this WMO.
			A block of zero-padded, zero-terminated strings. There are nModels file names in this list. They have to be .MDX!
			*/

			if (size)
			{
#ifdef MDX_INCL
				m_MDXFilenames = (char*)f.GetDataFromCurrent();

				char *p = m_MDXFilenames, *end = p + size;
				int t = 0;
				while (p < end)
				{
					string path(p);
					p += strlen(p) + 1;
					while ((p < end) && (*p == 0)) p++;

					_ModelsMgr->Add(path);
					m_MDXNames.push_back(path);

				}
#endif
				f.SeekRelative(size);
			}

		}
		else if (strcmp(fourcc, "MODD") == 0)
		{
			/*
			Information for doodad instances. 40 bytes per doodad instance, nDoodads entries.
			While WMOs and models (M2s) in a map m_TileExists are rotated along the axes, doodads within a WMO are oriented using quaternions! Hooray for consistency!
			I had to do some tinkering and mirroring to orient the doodads correctly using the quaternion, see model.cpp in the WoWmapview source code for the exact transform matrix. It's probably because I'm using another coordinate system, as a lot of other coordinates in WMOs and models also have to be read as (X,Z,-Y) to work in my system. But then again, the ADT files have the "correct" order of coordinates. Weird.
			Offset 	Type 		Description
			0x00 	uint32_t 		Offset to the start of the model's filename in the MODN chunk.
			0x04 	3 * float 	Position (X,Z,-Y)
			0x10 	float 		W component of the orientation quaternion
			0x14 	3 * float 	X, Y, Z components of the orientaton quaternion
			0x20 	float 		Scale factor
			0x24 	4 * uint8_t 	(B,G,R,A) Lightning-color.
			Are you sure the order of the quaternion components is W,X,Y,Z? It seems it is X,Y,Z,W -andhansen
			struct SMODoodadDef // 03-29-2005 By ObscuR
			*/
			header.nDoodadNames = size / 0x40;
			for (uint32_t i = 0; i < header.nDoodadNames; i++)
			{
				int ofs;
				f.ReadBytes(&ofs, 4);
#ifdef MDX_INCL
				if (!m_MDXFilenames)
				{
					continue;
				}

				Model *m = (Model*)_ModelsMgr->objects[m_MDXFilenames + ofs];
				DoodadInstance mi(m, f);
				m_MDXInstances.push_back(mi);
#endif
			}

		}
		else if (strcmp(fourcc, "MFOG") == 0)
		{
			// Fog information
			int fogsCount = size / WMOFogDef::__size;
			for (uint32_t i = 0; i < fogsCount; i++)
			{
				WMOFog fog(f);
				fogs.push_back(fog);
			}
		}
		else if (strcmp(fourcc, "MCVP") == 0)
		{
			//Convex Volume Planes.Contains blocks of floating - point numbers.
			// C4Plane convex_volume_planes[];   // normal points out
		}
		else
		{
			Debug::Warn("WMO[%s]: No implement wmo chunk [%s] [%d].", GetName().c_str(), fourcc, size);
		}

		f.Seek(nextpos);
	}

	// Init groups
	for (auto it = groups.begin(); it != groups.end(); ++it)
	{
		(*it)->initDisplayList();
	}

	return true;
}

void WMO::draw(int doodadset, cvec3 ofs, const float roll)
{
	for (auto it = groups.begin(); it != groups.end(); ++it)
	{
		(*it)->draw(ofs, roll);
	}

	if (_WowSettings->drawdoodads)
	{
		for (auto it = groups.begin(); it != groups.end(); ++it)
		{
			(*it)->drawDoodads(doodadset, ofs, roll);
		}
	}

	for (auto it = groups.begin(); it != groups.end(); ++it)
	{
		(*it)->drawLiquid();
	}

	/*
	// draw light placeholders
	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLES);
	for (int i=0; i<nLights; i++) {
		glColor4fv(lights[i].fcolor);
		glVertex3fv(lights[i].pos);
		glVertex3fv(lights[i].pos + vec3(-0.5f,1,0));
		glVertex3fv(lights[i].pos + vec3(0.5f,1,0));
	}
	glEnd();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glColor4f(1,1,1,1);
	*/

	/*
	// draw fog positions..?
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	for (size_t i=0; i<fogs.size(); i++) {
		WMOFog &fog = fogs[i];
		glColor4f(1,1,1,1);
		glBegin(GL_LINE_LOOP);
		glVertex3fv(fog.pos);
		glVertex3fv(fog.pos + vec3(fog.rad1, 5, -fog.rad2));
		glVertex3fv(fog.pos + vec3(fog.rad1, 5, fog.rad2));
		glVertex3fv(fog.pos + vec3(-fog.rad1, 5, fog.rad2));
		glVertex3fv(fog.pos + vec3(-fog.rad1, 5, -fog.rad2));
		glEnd();
	}
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	*/


	// draw group boundingboxes
	/*glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	for (int i=0; i<header.nGroups; i++) {
		WMOGroup* g = groups[i];
		float fc[2] = {1,0};
		glColor4f(fc[i%2],fc[(i/2)%2],fc[(i/3)%2],1);
		glBegin(GL_LINE_LOOP);

		glVertex3f(g->b1.x, g->b1.y, g->b1.z);
		glVertex3f(g->b1.x, g->b2.y, g->b1.z);
		glVertex3f(g->b2.x, g->b2.y, g->b1.z);
		glVertex3f(g->b2.x, g->b1.y, g->b1.z);

		glVertex3f(g->b2.x, g->b1.y, g->b2.z);
		glVertex3f(g->b2.x, g->b2.y, g->b2.z);
		glVertex3f(g->b1.x, g->b2.y, g->b2.z);
		glVertex3f(g->b1.x, g->b1.y, g->b2.z);

		glEnd();
	}

	// draw portal relations
	glBegin(GL_LINES);
	for (size_t i=0; i<m_PortalReferences.size(); i++) {
		WMO_PortalReferences &pr = m_PortalReferences[i];
		WMO_PortalVertices &pv = m_PortalVertices[pr.portal];
		if (pr.dir>0) glColor4f(1,0,0,1);
		else glColor4f(0,0,1,1);
		vec3 pc = (pv.a+pv.b+pv.c+pv.d)*0.25f;
		vec3 gc = (groups[pr.group]->b1 + groups[pr.group]->b2)*0.5f;
		glVertex3fv(pc);
		glVertex3fv(gc);
	}
	glEnd();
	glColor4f(1,1,1,1);


	// draw portals
	for (int i=0; i<header.nPortals; i++) {
		glBegin(GL_LINE_STRIP);
		glVertex3fv(m_PortalVertices[i].d);
		glVertex3fv(m_PortalVertices[i].c);
		glVertex3fv(m_PortalVertices[i].b);
		glVertex3fv(m_PortalVertices[i].a);
		glEnd();
	}

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);*/

}

void WMO::drawSkybox()
{
	skybox = nullptr; // HACK

	if (skybox != nullptr)
	{
		// TODO: only draw sky if we are "inside" the WMO... ?

		// We need to clear the depth buffer, because the skybox model can (will?)
		// require it *. This is inefficient - is there a better way to do this?
		// * planets in front of "space" in Caverns of Time
		//glClear(GL_DEPTH_BUFFER_BIT);

		// update: skybox models seem to have an explicit renderop ordering!
		// that saves us the depth buffer clear and the depth testing, too

		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);

		glPushMatrix();
		{
			glTranslatef(_Camera->Position.x, _Camera->Position.y, _Camera->Position.z);
			const float sc = 2.0f;
			glScalef(sc, sc, sc);
			skybox->draw();
		}
		glPopMatrix();

		// _World->hadSky = true; FIXME WORLD

		glEnable(GL_DEPTH_TEST);
	}
}

void WMO::drawPortals()
{
	glBegin(GL_LINE_STRIP);
	for (uint32_t i = 0; i < header.nPortals; i++)
	{
		glVertex3fv(glm::value_ptr(m_PortalVertices[i].d));
		glVertex3fv(glm::value_ptr(m_PortalVertices[i].c));
		glVertex3fv(glm::value_ptr(m_PortalVertices[i].b));
		glVertex3fv(glm::value_ptr(m_PortalVertices[i].a));
	}
	glEnd();
}
