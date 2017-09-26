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

WMO::WMO(cstring name) : RefItemNamed(name), m_Loaded(false)
{
	m_TexturesNames = nullptr;
	m_GroupsNames = nullptr;

#ifdef DOODADS_INCL
	m_Skybox_Filename = nullptr;
	m_Skybox = nullptr;


	m_MDXFilenames = nullptr;
#endif
}

WMO::~WMO()
{
	Debug::Info("WMO[%s]: Unloading...", GetName().c_str());

	//

	delete[] m_TexturesNames;
	ERASE_VECTOR(m_Materials);

	delete[] m_GroupsNames;
	ERASE_VECTOR(m_Groups);

#ifdef DOODADS_INCL
	delete[] m_Skybox_Filename;
	_ModelsMgr->Delete(m_Skybox);
#endif

	ERASE_VECTOR(m_PortalVertices);
	ERASE_VECTOR(m_PortalInformation);
	ERASE_VECTOR(m_PortalReferences);

	ERASE_VECTOR(m_Lights);

#ifdef DOODADS_INCL
	ERASE_VECTOR(doodadsets);
	for (auto it = m_MDXNames.begin(); it != m_MDXNames.end(); ++it)
	{
		_ModelsMgr->Delete(*it);
	}
	ERASE_VECTOR(m_MDXInstances);
#endif

	ERASE_VECTOR(m_Fogs);
}

bool WMO::Init()
{
	File f = GetName();
	if (!f.Open())
	{
		Debug::Info("WMO[%s]: Error loading WMO.", GetName().c_str());
		return false;
	}

	//Debug::Info("WMO[%s]: Loading...", GetName().c_str());

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
			f.ReadBytes(&header, WMOHeaderDef::__size);
		}
		else if (strcmp(fourcc, "MOTX") == 0)               // List of textures (BLP Files) used in this map object.
		{
			m_TexturesNames = new char[size + 1];
			f.ReadBytes(m_TexturesNames, size);
			m_TexturesNames[size] = 0x00;
		}
		else if (strcmp(fourcc, "MOMT") == 0)               // Materials used in this map object, 64 bytes per texture (BLP file), nMaterials entries.
		{
			for (uint32_t i = 0; i < header.nTextures; i++)
			{
				WMOMaterial* _mat = new WMOMaterial(this, f);
				m_Materials.push_back(_mat);
			}
		}
		else if (strcmp(fourcc, "MOGN") == 0)              // List of group names for the m_Groups in this map object.
		{
			m_GroupsNames = new char[size + 1];
			f.ReadBytes(m_GroupsNames, size);
			m_GroupsNames[size] = 0x00;
		}
		else if (strcmp(fourcc, "MOGI") == 0)
		{
			for (uint32_t i = 0; i < header.nGroups; i++)
			{
				WMOGroup* group = new WMOGroup(this, i, f, m_GroupsNames);
				m_Groups.push_back(group);
			}
		}
		else if (strcmp(fourcc, "MOSB") == 0) // Skybox. 
		{
#ifdef DOODADS_INCL
			if (size > 4)
			{
				m_Skybox_Filename = new char[size + 1];
				f.ReadBytes(m_Skybox_Filename, size);
				m_Skybox_Filename[size] = 0x00;


				Debug::Warn("WMO[%s]: Skybox [%s]", GetName().c_str(), m_Skybox_Filename);
				m_Skybox = _ModelsMgr->Add(m_Skybox_Filename);

				if (!m_Skybox->IsLoaded())
				{
					_ModelsMgr->Delete(m_Skybox_Filename);
					m_Skybox = nullptr;
				}
			}
#endif
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

			for (uint32_t i = 0; i < header.nPortals; i++)
			{
				WMO_PortalVertices* p = new WMO_PortalVertices();
				vec3 temp;

				f.ReadBytes(&temp, sizeof(vec3));
				p->a = From_XYZ_To_XZminusY_RET(temp);

				f.ReadBytes(&temp, sizeof(vec3));
				p->b = From_XYZ_To_XZminusY_RET(temp);

				f.ReadBytes(&temp, sizeof(vec3));;
				p->c = From_XYZ_To_XZminusY_RET(temp);

				f.ReadBytes(&temp, sizeof(vec3));
				p->d = From_XYZ_To_XZminusY_RET(temp);

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
			for (uint32_t i = 0; i < size / WMO_PortalReferences::__size; i++)
			{
				WMO_PortalReferences* _portalReference = new WMO_PortalReferences;
				f.ReadBytes(_portalReference, WMO_PortalReferences::__size);
				m_PortalReferences.push_back(_portalReference);
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
				WMOLight* _wmoLight = new WMOLight(f);
				m_Lights.push_back(_wmoLight);
			}
		}
		else if (strcmp(fourcc, "MODS") == 0)
		{
			for (uint32_t i = 0; i < header.nDoodadSets; i++)
			{
#ifdef DOODADS_INCL
				WMO_DoodadSet* dds = new WMO_DoodadSet();
				f.ReadBytes(dds, WMO_DoodadSet::__size);
				doodadsets.push_back(dds);
#endif
			}
		}
		else if (strcmp(fourcc, "MODN") == 0) // List of filenames for M2 (mdx) models that appear in this WMO.
		{
			if (size)
			{
#ifdef DOODADS_INCL
				m_MDXFilenames = (char*)f.GetDataFromCurrent();

				WOWCHUNK_READ_STRINGS2_BEGIN;

				_ModelsMgr->Add(_string);
				m_MDXNames.push_back(_string);

				WOWCHUNK_READ_STRINGS2_END;
#endif
			}
		}
		else if (strcmp(fourcc, "MODD") == 0) // Information for doodad instances. 40 bytes per doodad instance, nDoodads entries.
		{
			header.nDoodadNames = size / 40;
			for (uint32_t i = 0; i < header.nDoodadNames; i++)
			{
#ifdef DOODADS_INCL
				DoodadInstance* _doodadInstance = new DoodadInstance(f);

				MDX* m = (MDX*)_ModelsMgr->objects[m_MDXFilenames + _doodadInstance->placementInfo->flags.nameIndex];
				assert1(m != nullptr);
				_doodadInstance->SetModel(m);

				m_MDXInstances.push_back(_doodadInstance);
#endif
			}

		}
		else if (strcmp(fourcc, "MFOG") == 0)
		{
			// Fog information
			for (uint32_t i = 0; i < (size / WMOFogDef::__size); i++)
			{
				WMOFog* fog = new WMOFog(f);
				m_Fogs.push_back(fog);
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

	// Init m_Groups
	for (auto it = m_Groups.begin(); it != m_Groups.end(); ++it)
	{
		(*it)->initDisplayList();
	}

	m_Loaded = true;

	return true;
}

bool WMO::draw(int doodadset, cvec3 ofs, const float roll)
{
	if (!m_Loaded)
	{
		return false;
	}

	// WMO groups
	for (auto it = m_Groups.begin(); it != m_Groups.end(); ++it)
	{
		if (!(*it)->draw2(ofs, roll))
		{
			//return false;
		}
	}
	_Perfomance->Stop(PERF_WMOs);


	// WMO doodads
	_Perfomance->Start(PERF_WMOs_DOODADS);
	if (_Settings->draw_map_wmo_doodads)
	{
		for (auto it = m_Groups.begin(); it != m_Groups.end(); ++it)
		{
			if (!(*it)->drawDoodads(doodadset, ofs, roll))
			{
				//return false;
			}
		}
	}
	_Perfomance->Stop(PERF_WMOs_DOODADS);



	/*for (auto it = m_Groups.begin(); it != m_Groups.end(); ++it)
	{
		if (!(*it)->drawLiquid())
		{
			//return false;
		}
	}*/

#ifdef _DEBUG
	//DEBUG_DrawLightPlaceHolders();
	//DEBUG_DrawFogPositions();
	//DEBUG_DrawBoundingBoxes();
	//DEBUG_DrawPortalsRelations();
	//DEBUG_DrawPortals();
#endif

	_Perfomance->Start(PERF_WMOs);

	return true;
}


bool WMO::drawSkybox()
{
	if (!m_Loaded)
	{
		return false;
	}

#ifdef DOODADS_INCL
	if (m_Skybox == nullptr)
	{
		return false;
	}

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	_Pipeline->Clear();
	_Pipeline->Translate(_Camera->Position);
	_Pipeline->Scale(2.0f);

	_TechniquesMgr->m_MDX_GeometryPass->Bind();
	_TechniquesMgr->m_MDX_GeometryPass->SetPVW();

	m_Skybox->draw();

	_TechniquesMgr->m_MDX_GeometryPass->Unbind();

	_EnvironmentManager->m_HasSky = true;

#endif

	return true;
}


#ifdef _DEBUG1

void WMO::DEBUG_DrawLightPlaceHolders()
{
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	glColor4f(1, 1, 1, 1);

	glBegin(GL_TRIANGLES);
	for (int i = 0; i < m_Lights.size(); i++)
	{
		glColor4fv(glm::value_ptr(m_Lights[i]->fcolor));

		glVertex3fv(glm::value_ptr(m_Lights[i]->lightDef.pos));
		glVertex3fv(glm::value_ptr(m_Lights[i]->lightDef.pos + vec3(-0.5f, 1, 0)));
		glVertex3fv(glm::value_ptr(m_Lights[i]->lightDef.pos + vec3(0.5f, 1, 0)));
	}
	glEnd();

	glColor4f(1, 1, 1, 1);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}

void WMO::DEBUG_DrawFogPositions()
{
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	glColor4f(1, 1, 1, 1);

	for (size_t i = 0; i < m_Fogs.size(); i++)
	{
		WMOFog* fog = m_Fogs[i];

		glBegin(GL_LINE_LOOP);
		glVertex3fv(glm::value_ptr(fog->fogDef.position));
		glVertex3fv(glm::value_ptr(fog->fogDef.position + vec3(fog->fogDef.smallerRadius, 5, -fog->fogDef.largerRadius)));
		glVertex3fv(glm::value_ptr(fog->fogDef.position + vec3(fog->fogDef.smallerRadius, 5, fog->fogDef.largerRadius)));
		glVertex3fv(glm::value_ptr(fog->fogDef.position + vec3(-fog->fogDef.smallerRadius, 5, fog->fogDef.largerRadius)));
		glVertex3fv(glm::value_ptr(fog->fogDef.position + vec3(-fog->fogDef.smallerRadius, 5, -fog->fogDef.largerRadius)));
		glEnd();
	}

	glColor4f(1, 1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}

void WMO::DEBUG_DrawBoundingBoxes()
{
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);


	for (int i = 0; i < header.nGroups; i++)
	{
		WMOGroup* g = m_Groups[i];
		float fc[2] = {1, 0};

		glColor4f(fc[i % 2], fc[(i / 2) % 2], fc[(i / 3) % 2], 0.7f);

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

	glColor4f(1, 1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}

void WMO::DEBUG_DrawPortalsRelations()
{
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	glBegin(GL_LINES);
	for (size_t i = 0; i < m_PortalReferences.size(); i++)
	{
		WMO_PortalReferences* pr = m_PortalReferences[i];
		WMO_PortalVertices* pv = m_PortalVertices[pr->portalIndex];

		if (pr->side > 0)
			glColor4f(1, 0, 0, 1);
		else
			glColor4f(0, 0, 1, 1);


		vec3 pc = (pv->a + pv->b + pv->c + pv->d) * 0.25f;
		vec3 gc = (m_Groups[pr->groupIndex]->b1 + m_Groups[pr->groupIndex]->b2)*0.5f;

		glVertex3fv(glm::value_ptr(pc));
		glVertex3fv(glm::value_ptr(gc));
	}
	glEnd();

	glColor4f(1, 1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);

}

void WMO::DEBUG_DrawPortals()
{
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	glColor4f(0, 1, 0, 0.7f);

	glBegin(GL_LINE_STRIP);
	for (uint32_t i = 0; i < header.nPortals; i++)
	{
		glVertex3fv(glm::value_ptr(m_PortalVertices[i]->d));
		glVertex3fv(glm::value_ptr(m_PortalVertices[i]->c));
		glVertex3fv(glm::value_ptr(m_PortalVertices[i]->b));
		glVertex3fv(glm::value_ptr(m_PortalVertices[i]->a));
	}
	glEnd();

	glColor4f(1, 1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}

#endif