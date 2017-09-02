#include "stdafx.h"

// Includes
#include "quaternion.h"
#include "Wmo_Fog.h"
#include "Wmo_Light.h"
#include "Wmo_Material.h"

// General
#include "wmo.h"

// Additional
#include "Wmo_Group.h"

#include "world.h"
#include "liquid.h"
#include "shaders.h"

#include "ModelsManager.h"


WMO::WMO(cstring name) : RefItemNamed(name), groups(0), mat(0), skybox(0) {
	File f(name.c_str());
	ok = f.Open();
	if(!ok) {
		Debug::Info("WMO[%s]: Error loading WMO.", name.c_str());
		return;
	}

	Debug::Info("WMO[%s]: Loading...", name.c_str());

	char fourcc[5];
	uint32_t size;
	float ff[3];

	char *ddnames = nullptr;
	char *groupnames = nullptr;

	char *texbuf = nullptr;

	while(!f.IsEof()) {
		memset(fourcc, 0, 4);
		size = 0;
		f.ReadBytes(fourcc, 4);
		f.ReadBytes(&size, 4);
		flipcc(fourcc);
		fourcc[4] = 0;

		if(size == 0)
			continue;

		size_t nextpos = f.GetPos() + size;

		if(strcmp(fourcc, "MVER") == 0) {
			uint32_t version;
			f.ReadBytes(&version, 4);
			assert3(version == 17, "Version mismatch != 17", std::to_string(version).c_str());
		}
		else if(strcmp(fourcc, "MOHD") == 0) {
			// Header for the map object.
			f.ReadBytes(&header, header.__size);

			//f.ReadBytes(&LiquidType, 4); // LiquidType related, see below in the MLIQ chunk.

			//groups = new WMOGroup[header.nGroups];
		}
		else if(strcmp(fourcc, "MOTX") == 0) {
			// List of textures (BLP Files) used in this map object.
			texbuf = new char[size + 1];
			f.ReadBytes(texbuf, size);
			texbuf[size] = 0;
		}
		else if(strcmp(fourcc, "MOMT") == 0) {
			// Materials used in this map object, 64 bytes per texture (BLP file), nMaterials entries.
			// WMOMaterialBlock bl;
/*
The flags might used to tweak alpha testing values, I'm not sure about it, but some grates and flags in IF seem to require an alpha testing threshold of 0, at other places this is greater than 0.
flag2 		Meaning
0x01 		?(I'm not sure atm I tend to use lightmap or something like this)
0x04 		Two-sided (disable backface culling)
0x10 		Bright at night (unshaded) (used on windows and lamps in Stormwind, for example) -ProFeT: i think that is Unshaded becase external face of windows are flagged like this.
0x20 		?
0x28 		Darkned ?, the intern face of windows are flagged 0x28
0x40 		looks like GL_CLAMP
0x80 		looks like GL_REPEAT
*/
			for(uint32_t i = 0; i < header.nTextures; i++) {
				WMOMaterial* _mat = new WMOMaterial(f);
				_mat->initTexture(texbuf);
				mat.push_back(_mat);

				/*WMOMaterial* m = mat[i];
				f.ReadBytes(m, 0x40); // read 64 bytes, struct WMOMaterial is 68 bytes

				string texpath(texbuf + m->nameStart);
				fixname(texpath);

				m->tex = _TexturesMgr->Add(texpath);
				textures.push_back(texpath);*/

				/*
				// material logging
				Debug::Info("Material %d:\t%d\t%d\t%d\t%X\t%d\t%X\t%d\t%f\t%f",
					i, m->flags, m->d1, m->transparent, m->col1, m->d3, m->col2, m->d4, m->f1, m->f2);
				for (int j=0; j<2; j++) Debug::Info("\t%d", m->dx[j]);
				Debug::Info("\t - %s", texpath.c_str());
				*/

			}
		}
		else if(strcmp(fourcc, "MOGN") == 0) {
			// List of group names for the groups in this map object. There are nGroups entries in this chunk.
			// A contiguous block of zero-terminated strings. The names are purely informational, they aren't used elsewhere (to my knowledge)
			// i think think that his realy is zero terminated and just a name list .. but so far im not sure _what_ else it could be - tharo
			groupnames = (char*)f.GetDataFromCurrent();
		}
		else if(strcmp(fourcc, "MOGI") == 0) {
			for(uint32_t i = 0; i < header.nGroups; i++) {
				auto group = new WMOGroup;
				group->init(this, f, i, groupnames);
				groups.push_back(group);
			}
		}
		else if(strcmp(fourcc, "MOSB") == 0) {
			/*
			Skybox. Always 00 00 00 00. Skyboxes are now defined in DBCs (Light.dbc etc.). Contained a M2 filename that was used as skybox.
			*/
			if(size > 4) {
				string path = (char*)f.GetDataFromCurrent();
				//fixname(path);
				if(path.length()) {
					Debug::Info("SKYBOX:");

					skybox = _ModelsMgr->Add(path);

					if(!skybox->ok) {
						_ModelsMgr->Delete(path);
						skybox = 0;
					}
				}
			}
		}
		else if(strcmp(fourcc, "MOPV") == 0) {
			/*
			Portal vertices, 4 * 3 * float per portal, nPortals entries.
			Portals are (always?) rectangles that specify where doors or entrances are in a WMO. They could be used for visibility, but I currently have no idea what relations they have to each other or how they work.
			Since when "playing" WoW, you're confined to the ground, checking for passing through these portals would be enough to toggle visibility for indoors or outdoors areas, however, when randomly flying around, this is not necessarily the case.
			So.... What happens when you're flying around on a gryphon, and you fly into that arch-shaped portal into Ironforge? How is that portal calculated? It's all cool as long as you're inside "legal" areas, I suppose.
			It's fun, you can actually map out the topology of the WMO using this and the MOPR chunk. This could be used to speed up the rendering once/if I figure out how.
			*/
			WMOPV p;
			for(uint32_t i = 0; i < header.nPortals; i++) {
				f.ReadBytes(ff, 12);
				p.a = vec3(ff[0], ff[2], -ff[1]);
				f.ReadBytes(ff, 12);
				p.b = vec3(ff[0], ff[2], -ff[1]);
				f.ReadBytes(ff, 12);
				p.c = vec3(ff[0], ff[2], -ff[1]);
				f.ReadBytes(ff, 12);
				p.d = vec3(ff[0], ff[2], -ff[1]);
				pvs.push_back(p);
			}
		}
		else if(strcmp(fourcc, "MOPT") == 0) {
			/*
			Portal information. 20 bytes per portal, nPortals entries.
			Offset	Type 		Description
			0x00 	uint16_t 		Base vertex index?
			0x02 	uint16_t 		Number of vertices (?), always 4 (?)
			0x04 	3*float 	a normal vector maybe? haven't checked.
			0x10 	float 		unknown  - if this is NAN, the three floats will be (0,0,1)
			*/
		}
		else if(strcmp(fourcc, "MOPR") == 0) {
			/*
			Portal <> group relationship? 2*nPortals entries of 8 bytes.
			I think this might specify the two WMO groups that a portal connects.
			Offset 	Type 		Description
			0x0 	uint16_t 		Portal index
			0x2 	uint16_t 		WMO group index
			0x4 	int16_t 		1 or -1
			0x6 	uint16_t 		always 0
			struct SMOPortalRef
			{
			  000h  UINT16 portalIndex;
			  000h  UINT16 groupIndex;
			  004h  UINT16 side;
			  006h  UINT16 filler;
			  008h
			};
			*/
			int nn = (int)size / sizeof(WMOPR);
			WMOPR *pr = (WMOPR*)f.GetDataFromCurrent();
			for(int i = 0; i < nn; i++) {
				prs.push_back(*pr++);
			}
		}
		else if(strcmp(fourcc, "MOVV") == 0) {
			/*
			Visible block vertices
			Just a list of vertices that corresponds to the visible block list.
			*/
		}
		else if(strcmp(fourcc, "MOVB") == 0) {
			/*
			Visible block list
			unsigned short firstVertex;
			unsigned short count;
			*/
		}
		else if(strcmp(fourcc, "MOLT") == 0) {
			/*
			Lighting information. 48 bytes per light, nLights entries
			Offset 	Type 		Description
			0x00 	4 * uint8_t 	Flags or something? Mostly (0,1,1,1)
			0x04 	4 * uint8_t 	Color (B,G,R,A)
			0x08 	3 * float 	Position (X,Z,-Y)
			0x14 	7 * float 	Unknown (light properties?)
			enum LightType
			{
				OMNI_LGT,
				SPOT_LGT,
				DIRECT_LGT,
				AMBIENT_LGT
			};
			struct SMOLight // 04-29-2005 By ObscuR
			{
			  000h  UINT8 LightType;
			  001h  UINT8 type;
			  002h  UINT8 useAtten;
			  003h  UINT8 pad;
			  004h  UINT8 color[4];
			  008h  float position[3];
			  014h  float intensity;
			  018h  float attenStart;
			  01Ch  float attenEnd;
			  020h  float unk1;
			  024h  float unk2;
			  028h  float unk3;
			  02Ch  float unk4;
			  030h
			};
			I haven't quite figured out how WoW actually does lighting, as it seems much smoother than the regular vertex lighting in my screenshots. The light paramters might be range or attenuation information, or something else entirely. Some WMO groups reference a lot of lights at once.
			The WoW client (at least on my system) uses only one light, which is always directional. Attenuation is always (0, 0.7, 0.03). So I suppose for models/doodads (both are M2 files anyway) it selects an appropriate light to turn on. Global light is handled similarly. Some WMO textures (BLP files) have specular maps in the alpha channel, the pixel shader renderpath uses these. Still don't know how to determine direction/color for either the outdoor light or WMO local lights... :)
			*/
			for(uint32_t i = 0; i < header.nLights; i++) {
				WMOLight l;
				l.init(f);
				lights.push_back(l);
			}
		}
		else if(strcmp(fourcc, "MODS") == 0) {
			/*
			This chunk defines doodad sets.
			Doodads in WoW are M2 model files. There are 32 bytes per doodad set, and nSets entries. Doodad sets specify several versions of "interior decoration" for a WMO. Like, a small house might have tables and a bed laid out neatly in one set called "Set_$DefaultGlobal", and have a horrible mess of abandoned broken things in another set called "Set_Abandoned01". The names are only informative.
			The doodad set number for every WMO instance is specified in the ADT files.
			Offset 	Type 		Description
			0x00 	20 * char 	Set name
			0x14 	uint32_t 		index of first doodad instance in this set
			0x18 	uint32_t 		number of doodad instances in this set
			0x1C 	uint32_t 		unused? (always 0)
			struct SMODoodadSet // --Schlumpf 17:03, 31 July 2007 (CEST)
			{
			000h  char   name[20];
			014h  UINT32 firstinstanceindex;
			018h  UINT32 numDoodads;
			01Ch  UINT32 nulls;
			}
			*/
			for(uint32_t i = 0; i < header.nDoodadSets; i++) {
				WMODoodadSet dds;
				f.ReadBytes(&dds, sizeof(WMODoodadSet));
				doodadsets.push_back(dds);
			}
		}
		else if(strcmp(fourcc, "MODN") == 0) {
			/*
			List of filenames for M2 (mdx) models that appear in this WMO.
			A block of zero-padded, zero-terminated strings. There are nModels file names in this list. They have to be .MDX!
			*/
			if(size) {
				ddnames = (char*)f.GetDataFromCurrent();
				//fixnamen(ddnames, size);

				char *p = ddnames, *end = p + size;
				int t = 0;
				while(p < end) {
					string path(p);
					p += strlen(p) + 1;
					while((p < end) && (*p == 0)) p++;

					_ModelsMgr->Add(path);
					models.push_back(path);
				}
				f.SeekRelative((int)size);
			}
		}
		else if(strcmp(fourcc, "MODD") == 0) {
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
			{
			  000h  UINT32 nameIndex
			  004h  float pos[3];
			  010h  float rot[4];
			  020h  float scale;
			  024h  UINT8 color[4];
			  028h
			};
			*/
			header.nDoodadNames = (int)size / 0x28;
			for(uint32_t i = 0; i < header.nDoodadNames; i++) {
				int ofs;
				f.ReadBytes(&ofs, 4);
				if(!ddnames)
					continue;
				Model *m = (Model*)_ModelsMgr->items[_ModelsMgr->GetIDByName(ddnames + ofs)];
				ModelInstance mi;
				mi.InitAsDoodad(m, f);
				modelis.push_back(mi);
			}

		}
		else if(strcmp(fourcc, "MFOG") == 0) {
			// Fog information
			int fogsCount = size / 0x30;
			for(int i = 0; i < fogsCount; i++) {
				WMOFog fog;
				fog.init(f);
				fogs.push_back(fog);
			}
		}
		else if(strcmp(fourcc, "MCVP") == 0) {
			//Convex Volume Planes.Contains blocks of floating - point numbers.
			// C4Plane convex_volume_planes[];   // normal points out
		}
		else {
			Debug::Info("WMO[]: No implement wmo chunk %s [%d].", fourcc, size);
		}

		f.Seek(nextpos);
	}

	delete[] texbuf;

	// Init groups
	for(auto it = groups.begin(); it != groups.end(); ++it)
		(*it)->initDisplayList();

	//for(int i = 0; i < header.nGroups; i++)
	//	groups[i].initDisplayList();
}

WMO::~WMO() {
	if(!ok)
		return;

	Debug::Info("Unloading WMO %s", GetName().c_str());
	//if(groups)
	//	delete[] groups;

	//for (vector<string>::iterator it = textures.begin(); it != textures.end(); ++it)
			//video.textures.delbyname(*it);

	for(vector<string>::iterator it = models.begin(); it != models.end(); ++it)
		_ModelsMgr->Delete(*it);

	//if(mat) bouzi
	//	delete[] mat;

	if(skybox) {
		//delete skybox;
		//_ModelsMgr->del(sbid);
	}
}

void WMO::draw(int doodadset, cvec3 ofs, const float rot) {
	if(!ok)
		return;

	for(auto it = groups.begin(); it != groups.end(); ++it)
		(*it)->draw(ofs, rot);

	if(_World->drawdoodads)
		for(auto it = groups.begin(); it != groups.end(); ++it)
			(*it)->drawDoodads(doodadset, ofs, rot);

	for(auto it = groups.begin(); it != groups.end(); ++it)
		(*it)->drawLiquid();

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
	for (size_t i=0; i<prs.size(); i++) {
		WMOPR &pr = prs[i];
		WMOPV &pv = pvs[pr.portal];
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
		glVertex3fv(pvs[i].d);
		glVertex3fv(pvs[i].c);
		glVertex3fv(pvs[i].b);
		glVertex3fv(pvs[i].a);
		glEnd();
	}
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);*/
	
}

void WMO::drawSkybox() {
	if(skybox) {
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
			vec3 o = vec3(_Camera->Position.x, _Camera->Position.y, _Camera->Position.z);
			glTranslatef(o.x, o.y, o.z);
			const float sc = 2.0f;
			glScalef(sc, sc, sc);
			skybox->draw();
		}
		glPopMatrix();
		_World->hadSky = true;
		glEnable(GL_DEPTH_TEST);
	}
}

void WMO::drawPortals() {
	// not used ;)
	glBegin(GL_QUADS);
	for(uint32_t i = 0; i < header.nPortals; i++) {
		glVertex3fv(glm::value_ptr(pvs[i].d));
		glVertex3fv(glm::value_ptr(pvs[i].c));
		glVertex3fv(glm::value_ptr(pvs[i].b));
		glVertex3fv(glm::value_ptr(pvs[i].a));
	}
	glEnd();
}
