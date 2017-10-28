#include "../stdafx.h"

// Includes
#include "Wmo_Fog.h"
#include "Wmo_Light.h"
#include "Wmo_Material.h"

// General
#include "wmo.h"

// Additional
#include "../Environment/EnvironmentManager.h"
#include "../MDX/ModelsManager.h"
#include "Wmo_Group.h"

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
	Modules::log().Info("WMO[%s]: Unloading...", GetName().c_str());

	//

	delete[] m_TexturesNames;
	ERASE_VECTOR(m_Materials);

	delete[] m_GroupsNames;
	ERASE_VECTOR(m_Groups);

#ifdef DOODADS_INCL
	delete[] m_Skybox_Filename;
	_ModelsMgr->Delete(m_Skybox);
#endif

	// Clear portals
	if (m_Header.nPortals)
	{
		delete[] m_PortalVertices;
		ERASE_VECTOR(m_PortalInformation);
		ERASE_VECTOR(m_PortalReferences);
	}

	// Clean visible block verts
	if (m_VisibleBlockList.size())
	{
		delete[] m_VisibleBlockVertices;
		ERASE_VECTOR(m_VisibleBlockList);
	}

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

bool WMO::Load()
{
	File f = GetName();
	if (!f.Open())
	{
		return false;
	}

	//Modules::log().Info("WMO[%s]: Loading...", GetName().c_str());

	char fourcc[5];
	uint32 size;
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
			uint32 version;
			f.ReadBytes(&version, 4);
			assert1(version == 17);
		}
		else if (strcmp(fourcc, "MOHD") == 0)               // Header
		{
			f.ReadBytes(&m_Header, WMOHeaderDef::__size);
			//Modules::log().Error("ID = [%d]", m_Header.wmoID()->Get_ID());
		}
		else if (strcmp(fourcc, "MOTX") == 0)               // List of textures (BLP Files) used in this map object.
		{
			m_TexturesNames = new char[size + 1];
			f.ReadBytes(m_TexturesNames, size);
			m_TexturesNames[size] = 0x00;
		}
		else if (strcmp(fourcc, "MOMT") == 0)               // Materials used in this map object, 64 bytes per texture (BLP file), nMaterials entries.
		{
			for (uint32 i = 0; i < m_Header.nTextures; i++)
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
			for (uint32 i = 0; i < m_Header.nGroups; i++)
			{
				WMOGroup* group = new WMOGroup(this, i, f);
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
				Modules::log().Warn("WMO[%s]: Skybox [%s]", GetName().c_str(), m_Skybox_Filename);

				//m_SkyModel = new Sky_Model(m_Skybox_Filename);
			}
#endif
		}
		else if (strcmp(fourcc, "MOPV") == 0)
		{
			assert1((size % sizeof(vec3)) == 0);

			m_PortalVertices = new vec3[size / sizeof(vec3)];
			for (uint32 i = 0; i < size / sizeof(vec3); i++)
			{
				vec3 portalVertex;
				f.ReadBytes(&portalVertex, sizeof(vec3));
				m_PortalVertices[i] = From_XYZ_To_XZminusY_RET(portalVertex);
			}
		}
		else if (strcmp(fourcc, "MOPT") == 0)
		{
			assert1((size % WMO_PortalInformation::__size) == 0);
			assert1((size / WMO_PortalInformation::__size) == m_Header.nPortals);

			for (uint32 i = 0; i < size / WMO_PortalInformation::__size; i++)
			{
				WMO_PortalInformation* portalInformation = new WMO_PortalInformation;
				f.ReadBytes(portalInformation, WMO_PortalInformation::__size);
				m_PortalInformation.push_back(portalInformation);
			}
		}
		else if (strcmp(fourcc, "MOPR") == 0)
		{
			for (uint32 i = 0; i < size / WMO_PortalReferences::__size; i++)
			{
				WMO_PortalReferences* portalReference = new WMO_PortalReferences;
				f.ReadBytes(portalReference, WMO_PortalReferences::__size);
				m_PortalReferences.push_back(portalReference);
			}
		}
		else if (strcmp(fourcc, "MOVV") == 0)
		{
			assert1((size % sizeof(vec3)) == 0);

			m_VisibleBlockVertices = new vec3[size / sizeof(vec3)];
			for (uint32 i = 0; i < size / sizeof(vec3); i++)
			{
				vec3 visibleVertex;
				f.ReadBytes(&visibleVertex, sizeof(vec3));
				m_VisibleBlockVertices[i] = From_XYZ_To_XZminusY_RET(visibleVertex);
			}
		}
		else if (strcmp(fourcc, "MOVB") == 0)
		{
			assert1((size % WMO_VisibleBlockList::__size) == 0);

			for (uint32 i = 0; i < size / WMO_VisibleBlockList::__size; i++)
			{
				WMO_VisibleBlockList* visibleBlockList = new WMO_VisibleBlockList;
				f.ReadBytes(visibleBlockList, WMO_VisibleBlockList::__size);
				m_VisibleBlockList.push_back(visibleBlockList);
			}

		}
		else if (strcmp(fourcc, "MOLT") == 0)
		{
			for (uint32 i = 0; i < m_Header.nLights; i++)
			{
				WMOLight* _wmoLight = new WMOLight(f);
				m_Lights.push_back(_wmoLight);
			}
		}
		else if (strcmp(fourcc, "MODS") == 0)
		{
			for (uint32 i = 0; i < m_Header.nDoodadSets; i++)
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
			m_Header.nDoodadNames = size / 40;
			for (uint32 i = 0; i < m_Header.nDoodadNames; i++)
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
			for (uint32 i = 0; i < (size / WMOFogDef::__size); i++)
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
			Modules::log().Warn("WMO[%s]: No implement wmo chunk [%s] [%d].", GetName().c_str(), fourcc, size);
		}

		f.Seek(nextpos);
	}

	// Init m_Groups
	for (auto it = m_Groups.begin(); it != m_Groups.end(); ++it)
	{
		(*it)->Load();
		m_Bounds.makeUnion((*it)->m_Bounds);
	}

	m_Loaded = true;

	return true;
}

//

bool WMO::draw(uint32 _doodadSet)
{
	if (!m_Loaded)
	{
		return false;
	}

	BoundingBox aabb = m_Bounds;
	aabb.transform(_Pipeline->GetWorld());

	if (_CameraFrustum->_frustum.cullBox(aabb))
	{
		return false;
	}

	// WMO groups
	{
		PERF_START(PERF_MAP_MODELS_WMOs_GEOMETRY);
		for (auto it = m_Groups.begin(); it != m_Groups.end(); ++it)
		{
			(*it)->Render();
		}
		PERF_STOP(PERF_MAP_MODELS_WMOs_GEOMETRY);
	}

	// WMO doodads
	{
		PERF_START(PERF_MAP_MODELS_WMOs_DOODADS);
		if (Modules::config().draw_map_wmo_doodads)
		{
			for (auto it = m_Groups.begin(); it != m_Groups.end(); ++it)
			{
				(*it)->drawDoodads(_doodadSet);
			}
		}
		PERF_STOP(PERF_MAP_MODELS_WMOs_DOODADS);
	}

	// WMO liquids
	{
		PERF_START(PERF_MAP_MODELS_WMOs_LIQUIDS);
		for (auto it = m_Groups.begin(); it != m_Groups.end(); ++it)
		{
			(*it)->drawLiquid();
		}
		PERF_STOP(PERF_MAP_MODELS_WMOs_LIQUIDS);
	}

	// Debug geometry
	{

		//_TechniquesMgr->m_Debug_GeometryPass->BindS();
		//_TechniquesMgr->m_Debug_GeometryPass->SetPVW();

		//#ifdef _DEBUG
		//DEBUG_DrawLightPlaceHolders();
		//DEBUG_DrawFogPositions();
		//DEBUG_DrawMainBoundingBox();
		//DEBUG_DrawBoundingBoxes();
		//DEBUG_DrawPortalsRelations();
		//DEBUG_DrawPortals();
		//#endif

		//_TechniquesMgr->m_Debug_GeometryPass->Unbind();
	}

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

	_Pipeline->Clear();
	_Pipeline->Translate(_Camera->Position);
	_Pipeline->Scale(2.0f);

	_TechniquesMgr->m_MDX_GeometryPass->BindS();
	_TechniquesMgr->m_MDX_GeometryPass->SetPVW();

	m_Skybox->draw();

	_EnvironmentManager->m_HasSky = true;
#endif

	return true;
}

//

void WMO::DEBUG_DrawLightPlaceHolders()
{
	//glDisable(GL_CULL_FACE);
	/*glColor4f(1, 1, 1, 1);

	glBegin(GL_TRIANGLES);
	for (int i = 0; i < m_Lights.size(); i++)
	{
		glColor4fv(m_Lights[i]->fcolor);

		glVertex3fv(m_Lights[i]->lightDef.pos);
		glVertex3fv(m_Lights[i]->lightDef.pos + vec3(-0.5f, 1, 0));
		glVertex3fv(m_Lights[i]->lightDef.pos + vec3(0.5f, 1, 0));
	}
	glEnd();*/

	//glEnable(GL_CULL_FACE);
}

void WMO::DEBUG_DrawFogPositions()
{
	//glDisable(GL_TEXTURE_2D);

	/*glColor4f(1, 1, 1, 1);

	for (size_t i = 0; i < m_Fogs.size(); i++)
	{
		WMOFog* fog = m_Fogs[i];

		glBegin(GL_LINE_LOOP);
		glVertex3fv(fog->fogDef.position);
		glVertex3fv(fog->fogDef.position + vec3(fog->fogDef.smallerRadius, 5, -fog->fogDef.largerRadius));
		glVertex3fv(fog->fogDef.position + vec3(fog->fogDef.smallerRadius, 5, fog->fogDef.largerRadius));
		glVertex3fv(fog->fogDef.position + vec3(-fog->fogDef.smallerRadius, 5, fog->fogDef.largerRadius));
		glVertex3fv(fog->fogDef.position + vec3(-fog->fogDef.smallerRadius, 5, -fog->fogDef.largerRadius));
		glEnd();
	}

	glColor4f(1, 1, 1, 1);*/

	//glEnable(GL_TEXTURE_2D);
}

void WMO::DEBUG_DrawMainBoundingBox()
{
	/*_TechniquesMgr->m_Debug_GeometryPass->SetColor4(vec4(1.0f, 1.0f, 1.0f, 0.7f));

	vector<vec3> verts;

	verts.push_back(vec3(m_Bounds.Min.x, m_Bounds.Min.y, m_Bounds.Min.z));
	verts.push_back(vec3(m_Bounds.Min.x, m_Bounds.Min.y, m_Bounds.Max.z));

	verts.push_back(vec3(m_Bounds.Min.x, m_Bounds.Min.y, m_Bounds.Min.z));
	verts.push_back(vec3(m_Bounds.Min.x, m_Bounds.Max.y, m_Bounds.Min.z));

	verts.push_back(vec3(m_Bounds.Min.x, m_Bounds.Min.y, m_Bounds.Min.z));
	verts.push_back(vec3(m_Bounds.Max.x, m_Bounds.Min.y, m_Bounds.Min.z));


	verts.push_back(vec3(m_Bounds.Max.x, m_Bounds.Max.y, m_Bounds.Max.z));
	verts.push_back(vec3(m_Bounds.Max.x, m_Bounds.Min.y, m_Bounds.Max.z));

	verts.push_back(vec3(m_Bounds.Max.x, m_Bounds.Max.y, m_Bounds.Max.z));
	verts.push_back(vec3(m_Bounds.Max.x, m_Bounds.Max.y, m_Bounds.Min.z));

	verts.push_back(vec3(m_Bounds.Max.x, m_Bounds.Max.y, m_Bounds.Max.z));
	verts.push_back(vec3(m_Bounds.Min.x, m_Bounds.Max.y, m_Bounds.Max.z));


	verts.push_back(vec3(m_Bounds.Max.x, m_Bounds.Max.y, m_Bounds.Min.z));
	verts.push_back(vec3(m_Bounds.Max.x, m_Bounds.Min.y, m_Bounds.Min.z));


	verts.push_back(vec3(m_Bounds.Max.x, m_Bounds.Min.y, m_Bounds.Min.z));
	verts.push_back(vec3(m_Bounds.Max.x, m_Bounds.Min.y, m_Bounds.Max.z));

	verts.push_back(vec3(m_Bounds.Max.x, m_Bounds.Min.y, m_Bounds.Max.z));
	verts.push_back(vec3(m_Bounds.Min.x, m_Bounds.Min.y, m_Bounds.Max.z));

	verts.push_back(vec3(m_Bounds.Min.x, m_Bounds.Min.y, m_Bounds.Max.z));
	verts.push_back(vec3(m_Bounds.Min.x, m_Bounds.Max.y, m_Bounds.Max.z));

	verts.push_back(vec3(m_Bounds.Min.x, m_Bounds.Max.y, m_Bounds.Max.z));
	verts.push_back(vec3(m_Bounds.Min.x, m_Bounds.Max.y, m_Bounds.Min.z));

	verts.push_back(vec3(m_Bounds.Min.x, m_Bounds.Max.y, m_Bounds.Min.z));
	verts.push_back(vec3(m_Bounds.Max.x, m_Bounds.Max.y, m_Bounds.Min.z));

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(vec3), verts.data(), GL_STATIC_DRAW);

	// Vertex
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_LINES, 0, verts.size());

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &buffer);*/
}

void WMO::DEBUG_DrawBoundingBoxes()
{
	for (int i = 0; i < m_Header.nGroups; i++)
	{
		/*WMOGroup* g = m_Groups[i];
		float fc[2] = {1, 0};

		_TechniquesMgr->m_Debug_GeometryPass->SetColor4(vec4(fc[i % 2], fc[(i / 2) % 2], fc[(i / 3) % 2], 0.7f));

		vector<vec3> verts;

		verts.push_back(vec3(g->m_Bounds.Min.x, g->m_Bounds.Min.y, g->m_Bounds.Min.z));
		verts.push_back(vec3(g->m_Bounds.Min.x, g->m_Bounds.Min.y, g->m_Bounds.Max.z));

		verts.push_back(vec3(g->m_Bounds.Min.x, g->m_Bounds.Min.y, g->m_Bounds.Min.z));
		verts.push_back(vec3(g->m_Bounds.Min.x, g->m_Bounds.Max.y, g->m_Bounds.Min.z));

		verts.push_back(vec3(g->m_Bounds.Min.x, g->m_Bounds.Min.y, g->m_Bounds.Min.z));
		verts.push_back(vec3(g->m_Bounds.Max.x, g->m_Bounds.Min.y, g->m_Bounds.Min.z));


		verts.push_back(vec3(g->m_Bounds.Max.x, g->m_Bounds.Max.y, g->m_Bounds.Max.z));
		verts.push_back(vec3(g->m_Bounds.Max.x, g->m_Bounds.Min.y, g->m_Bounds.Max.z));

		verts.push_back(vec3(g->m_Bounds.Max.x, g->m_Bounds.Max.y, g->m_Bounds.Max.z));
		verts.push_back(vec3(g->m_Bounds.Max.x, g->m_Bounds.Max.y, g->m_Bounds.Min.z));

		verts.push_back(vec3(g->m_Bounds.Max.x, g->m_Bounds.Max.y, g->m_Bounds.Max.z));
		verts.push_back(vec3(g->m_Bounds.Min.x, g->m_Bounds.Max.y, g->m_Bounds.Max.z));


		verts.push_back(vec3(g->m_Bounds.Max.x, g->m_Bounds.Max.y, g->m_Bounds.Min.z));
		verts.push_back(vec3(g->m_Bounds.Max.x, g->m_Bounds.Min.y, g->m_Bounds.Min.z));


		verts.push_back(vec3(g->m_Bounds.Max.x, g->m_Bounds.Min.y, g->m_Bounds.Min.z));
		verts.push_back(vec3(g->m_Bounds.Max.x, g->m_Bounds.Min.y, g->m_Bounds.Max.z));

		verts.push_back(vec3(g->m_Bounds.Max.x, g->m_Bounds.Min.y, g->m_Bounds.Max.z));
		verts.push_back(vec3(g->m_Bounds.Min.x, g->m_Bounds.Min.y, g->m_Bounds.Max.z));

		verts.push_back(vec3(g->m_Bounds.Min.x, g->m_Bounds.Min.y, g->m_Bounds.Max.z));
		verts.push_back(vec3(g->m_Bounds.Min.x, g->m_Bounds.Max.y, g->m_Bounds.Max.z));

		verts.push_back(vec3(g->m_Bounds.Min.x, g->m_Bounds.Max.y, g->m_Bounds.Max.z));
		verts.push_back(vec3(g->m_Bounds.Min.x, g->m_Bounds.Max.y, g->m_Bounds.Min.z));

		verts.push_back(vec3(g->m_Bounds.Min.x, g->m_Bounds.Max.y, g->m_Bounds.Min.z));
		verts.push_back(vec3(g->m_Bounds.Max.x, g->m_Bounds.Max.y, g->m_Bounds.Min.z));

		GLuint buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);

		glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(vec3), verts.data(), GL_STATIC_DRAW);

		// Vertex
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glDrawArrays(GL_LINES, 0, verts.size());

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &buffer);*/
	}
}

void WMO::DEBUG_DrawPortalsRelations()
{
	for (size_t i = 0; i < m_Header.nPortals; i++)
	{
		/*WMO_PortalInformation* portalInformation = m_PortalInformation[i];
		WMO_PortalReferences* portalReference = m_PortalReferences[i];
		vec3 pv = m_PortalVertices[portalReference->portalIndex];

		if (portalReference->side > 0)
		{
			_TechniquesMgr->m_Debug_GeometryPass->SetColor4(vec4(1.0f, 0.0f, 0.0f, 0.8f));
		}
		else
		{
			_TechniquesMgr->m_Debug_GeometryPass->SetColor4(vec4(0.0f, 0.0f, 1.0f, 0.8f));
		}

		vec3 pc;
		for (uint32 j = portalInformation->startVertex; j < portalInformation->count; j++)
		{
			pc += m_PortalVertices[j];
		}
		pc *= 0.25f;
		vec3 gc = (m_Groups[portalReference->groupIndex]->m_Bounds.Min + m_Groups[portalReference->groupIndex]->m_Bounds.Max) * 0.5f;

		//

		vector<vec3> verts;
		verts.push_back(pc);
		verts.push_back(gc);

		//

		GLuint buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(vec3), verts.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glDrawArrays(GL_LINES, 0, verts.size());

		glDisableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &buffer);*/
	}
}

void WMO::DEBUG_DrawPortals()
{
	_TechniquesMgr->m_Debug_GeometryPass->SetColor4(vec4(0.0f, 1.0f, 0.0f, 0.5f));

	for (uint32 i = 0; i < m_Header.nPortals; i++)
	{
		/*WMO_PortalInformation* portalInformation = m_PortalInformation[i];

		vector<vec3> verts;
		for (uint32 j = portalInformation->startVertex; j < portalInformation->count; j++)
		{
			verts.push_back(m_PortalVertices[j]);
		}

		GLuint buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(vec3), verts.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, verts.size());

		glDisableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &buffer);*/
	}
}