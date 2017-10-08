#include "../stdafx.h"

// Includes
#include "Wmo.h"

// General
#include "Wmo_Group.h"

// Additional
#include "../Environment/EnvironmentManager.h"
#include "Wmo_Fog.h"
#include "Wmo_Light.h"
#include "Wmo_Material.h"

struct WMO_Group_MLIQ
{
	uint32 X;
	uint32 Y;

	uint32 A;
	uint32 B;

	vec3 pos;

	uint16 type;

	//
	static const uint32 __size = 30;
};

WMOGroup::WMOGroup(const WMO* _parentWMO, const uint32 _groupIndex, File& f) : m_ParentWMO(_parentWMO), m_GroupIndex(_groupIndex)
{
	m_MaterialsInfoCount = 0;
	m_MaterialsInfo = nullptr;

	indices = nullptr;

	nVertices = 0;
	vertices = nullptr;

	normals = nullptr;
	texcoords = nullptr;


	nBatches = 0;
	m_WMOBatchIndexes = nullptr;


	nLights = 0;
	m_WMOLightsIndexes = nullptr;


	nDoodads = 0;
	m_DoodadsIndexes = nullptr;

	m_HasVertexColors = false;
	m_VertexColors = nullptr;

	lq = nullptr;

	// Read hgroup info for bounding box and name
	f.ReadBytes(&groupInfo, WMOGroupInfoDef::__size);

	if (groupInfo.nameoffset > 0)
	{
		m_GroupName = string(m_ParentWMO->m_GroupsNames + groupInfo.nameoffset);
	}
	else
	{
		m_GroupName = "(no name)";
	}

}

WMOGroup::~WMOGroup()
{
	//if (dl) glDeleteLists(dl, 1);
	//if (dl_light) glDeleteLists(dl_light, 1);

	//for (size_t i=0; i<lists.size(); i++) {
	//	glDeleteLists(lists[i].first);
	//}
	//************if (nBatches && lists.size()) glDeleteLists(lists[0].first, nBatches);

	if (lq) delete lq;
}

void WMOGroup::initDisplayList()
{
	// open group file
	char temp[256];
	strcpy_s(temp, m_ParentWMO->GetName().c_str());
	temp[m_ParentWMO->GetName().length() - 4] = 0;

	char fname[256];
	sprintf_s(fname, "%s_%03d.wmo", temp, m_GroupIndex);

	File f(fname);
	if (!f.Open())
	{
		return;
	}

	f.SeekRelative(12); // Version 4 + 4 + 4

	f.SeekRelative(8); // Chunk and size 4 + 4
	f.ReadBytes(&m_Header, WMOGroupHeader::__size);


	WMOFog* wf = m_ParentWMO->m_Fogs[m_Header.m_Fogs[0]];
	if (wf->fogDef.largerRadius <= 0)
		fog = -1; // default outdoor fog..?
	else
		fog = m_Header.m_Fogs[0];

	m_Bounds.set(m_Header.boundingBox.min, m_Header.boundingBox.max, true);

	char fourcc[5];
	uint32 size = 0;
	while (!f.IsEof())
	{
		memset(fourcc, 0, 4);
		size = 0;
		f.ReadBytes(fourcc, 4);
		f.ReadBytes(&size, 4);
		flipcc(fourcc);
		fourcc[4] = 0;
		if (size == 0)	continue;
		size_t nextpos = f.GetPos() + size;

		if (strcmp(fourcc, "MOPY") == 0) // Material info for triangles
		{
			m_MaterialsInfoCount = size / WMOGroup_MaterialInfo::__size;
			m_MaterialsInfo = (WMOGroup_MaterialInfo*)f.GetDataFromCurrent();
		}
		else if (strcmp(fourcc, "MOVI") == 0) // Vertex indices for triangles
		{
			indices = (uint16*)f.GetDataFromCurrent();
		}
		else if (strcmp(fourcc, "MOVT") == 0) // Vertices chunk.
		{
			nVertices = size / sizeof(vec3);
			vertices = (vec3*)f.GetDataFromCurrent();
		}
		else if (strcmp(fourcc, "MONR") == 0) // Normals
		{
			normals = (vec3*)f.GetDataFromCurrent();
		}
		else if (strcmp(fourcc, "MOTV") == 0) // Texture coordinates
		{
			texcoords = (vec2*)f.GetDataFromCurrent();
		}
		else if (strcmp(fourcc, "MOBA") == 0) // Render m_WMOBatchIndexes.
		{
			nBatches = size / WMOBatch::__size;
			m_WMOBatchIndexes = (WMOBatch*)f.GetDataFromCurrent();
		}
		else if (strcmp(fourcc, "MOLR") == 0) // Light references
		{
			nLights = size / sizeof(uint16);
			m_WMOLightsIndexes = (uint16*)f.GetDataFromCurrent();
		}
		else if (strcmp(fourcc, "MODR") == 0) // Doodad references
		{
			nDoodads = size / sizeof(uint16);
			m_DoodadsIndexes = (uint16*)f.GetDataFromCurrent();
		}
		else if (strcmp(fourcc, "MOBN") == 0)
		{
		}
		else if (strcmp(fourcc, "MOBR") == 0)
		{
		}
		else if (strcmp(fourcc, "MOCV") == 0) // Vertex colors
		{
			m_HasVertexColors = true;
			m_VertexColors = (uint32*)f.GetDataFromCurrent();
		}
		else if (strcmp(fourcc, "MLIQ") == 0) // Liquid
		{
			WMO_Group_MLIQ liquidHeader;
			f.ReadBytes(&liquidHeader, WMO_Group_MLIQ::__size);

			if (m_Header.liquidType > 0)
			{
				Debug::Green("WMO[%s]: Contain liquid! [%s]", m_ParentWMO->GetName().c_str(), DBC_LiquidType[m_Header.liquidType]->Get_Name());

				lq = new Liquid(liquidHeader.A, liquidHeader.B, From_XYZ_To_XZminusY_RET(liquidHeader.pos));
				lq->initFromWMO2(f, m_ParentWMO->m_Materials[liquidHeader.type], DBC_LiquidType[m_Header.liquidType], groupInfo.flags.FLAG_IS_INDOOR);
			}
		}
		else if (strcmp(fourcc, "MORI") == 0)
		{
		}
		else if (strcmp(fourcc, "MORB") == 0)
		{
		}
		else if (strcmp(fourcc, "MOTA") == 0)
		{
		}
		else if (strcmp(fourcc, "MOBS") == 0)
		{
		}
		else
		{
			Debug::Info("WMO_Group[]: No implement group chunk %s [%d].", fourcc, size);
		}

		f.Seek(nextpos);
	}

	vec4* vertexColors;

	if (m_HasVertexColors)
	{
		vertexColors = new vec4[nVertices];
	}

	float v35 = 0;
	float v36 = 0;
	float v37 = 0;

	if (m_ParentWMO->header.flags.FLAG_skip_base_color)
	{
		v35 = 0;
		v36 = 0;
		v37 = 0;
	}
	else
	{
		v35 = m_ParentWMO->header.ambColor.r;
		v37 = m_ParentWMO->header.ambColor.g;
		v36 = m_ParentWMO->header.ambColor.b;
	}

	// Converts
	for (uint32 i = 0; i < nVertices; i++)
	{
		vertices[i] = From_XYZ_To_XZminusY_RET(vertices[i]);
		normals[i] = From_XYZ_To_XZminusY_RET(normals[i]);

		if (m_HasVertexColors)
		{
			vertexColors[i] = fromBGRA(m_VertexColors[i]);

			if (m_ParentWMO->header.flags.FLAG_lighten_interiors)
			{
				vertexColors[i].w = groupInfo.flags.FLAG_IS_OUTDOOR ? 1.0f : 0.0f;
			}
			else
			{
				vertexColors[i].x -= v36;
				vertexColors[i].y -= v37;
				vertexColors[i].z -= v35;

				float v38 = vertexColors[i].w;

				float v11 = vertexColors[i].x - v38 * vertexColors[i].x;
				vertexColors[i].x = v11 / 2;

				float v13 = vertexColors[i].y - v38 * vertexColors[i].y;
				vertexColors[i].y = v13 / 2;

				float v14 = vertexColors[i].z - v38 * vertexColors[i].z;
				vertexColors[i].z = v14 / 2;


				vertexColors[i].x = clamp(vertexColors[i].x, 0.0f, 1.0f);
				vertexColors[i].y = clamp(vertexColors[i].y, 0.0f, 1.0f);
				vertexColors[i].z = clamp(vertexColors[i].z, 0.0f, 1.0f);

				vertexColors[i].w = groupInfo.flags.FLAG_IS_OUTDOOR ? 1.0f : 0.0f;
			}
		}
		//else
		//{
		//	vertexColors[i] = vec4(0.0f, 0.3f, 0.6f, 0.9f);
		//}
	}

	initLighting();

	for (uint32 b = 0; b < nBatches; b++)
	{

		WMOBatch* batch = &m_WMOBatchIndexes[b];
		WMOMaterial* mat = m_ParentWMO->m_Materials[batch->material_id];

		/*bool overbright = (mat->IsUnshaded() && !m_HasVertexColors);
		bool spec_shader = (mat->GetShader() && !m_HasVertexColors && !overbright);

		pair<GLuint, int> currentList;
		currentList.first = list;
		currentList.second = spec_shader ? 1 : 0;

		glNewList(list, GL_COMPILE);
		{
			// setup texture
			mat->setup();

			if (mat->GetBlendMode())
			{
				glEnable(GL_ALPHA_TEST);

				if (mat->IsTesClampT())
					glAlphaFunc(GL_GREATER, 0.3f);

				if (mat->IsLightingDisabled())
					glAlphaFunc(GL_GREATER, 0.0f);
			}

			

			if (spec_shader)
			{
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(fromARGB(mat->GetDiffuseColor())));
			}
			else
			{
				vec4 nospec(0, 0, 0, 1);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(nospec));
			}

			if (overbright)
			{
				GLfloat em[4] = {mat->EmissiveColor().r, mat->EmissiveColor().g, mat->EmissiveColor().b, mat->EmissiveColor().a};
				glMaterialfv(GL_FRONT, GL_EMISSION, em);
			}

			// render
			glBegin(GL_TRIANGLES);
			for (int t = 0, i = batch->indexStart; t < batch->indexCount; t++, i++)
			{
				int a = indices[i];

				if (groupInfo.flags.FLAG_IS_INDOOR && m_HasVertexColors)
				{
					setGLColor(m_VertexColors[a]);
				}

				glNormal3fv(glm::value_ptr(From_XYZ_To_XZminusY_RET(normals[a])));
				glTexCoord2fv(glm::value_ptr(texcoords[a]));
				glVertex3fv(glm::value_ptr(From_XYZ_To_XZminusY_RET(vertices[a])));
			}
			glEnd();

			if (overbright)
			{
				GLfloat em[4] = {0,0,0,1};
				glMaterialfv(GL_FRONT, GL_EMISSION, em);
			}

			if (mat->GetBlendMode())
			{
				glDisable(GL_ALPHA_TEST);
			}
		}
		glEndList();
		*/

		GLsizeiptr bufferSize = 8 * sizeof(float);
		if (m_HasVertexColors)
		{
			bufferSize += 4 * sizeof(float);
		}

		glGenBuffers(1, &globalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, globalBuffer);

		glBufferData(GL_ARRAY_BUFFER, nVertices * bufferSize, NULL, GL_STATIC_DRAW);

		glBufferSubData(GL_ARRAY_BUFFER, nVertices * 0 * sizeof(float), nVertices * 3 * sizeof(float), vertices);
		glBufferSubData(GL_ARRAY_BUFFER, nVertices * 3 * sizeof(float), nVertices * 2 * sizeof(float), texcoords);
		glBufferSubData(GL_ARRAY_BUFFER, nVertices * 5 * sizeof(float), nVertices * 3 * sizeof(float), normals);

		if (m_HasVertexColors)
		{
			glBufferSubData(GL_ARRAY_BUFFER, nVertices * 8 * sizeof(float), nVertices * 4 * sizeof(float), vertexColors);
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	if (m_HasVertexColors)
	{
		delete[] vertexColors;
	}
}

void WMOGroup::initLighting()
{
	if (groupInfo.flags.FLAG_IS_INDOOR && m_HasVertexColors)
	{
		vec3 dirmin(1, 1, 1);
		float lenmin;
		int lmin;

#ifdef DOODADS_INCL
		for (uint32 i = 0; i < nDoodads; i++)
		{
			lenmin = 999999.0f * 999999.0f;
			lmin = 0;
			DoodadInstance* mi = m_ParentWMO->m_MDXInstances[m_DoodadsIndexes[i]];
			for (uint32 j = 0; j < m_ParentWMO->header.nLights; j++)
			{
				WMOLight* l = m_ParentWMO->m_Lights[j];
				vec3 dir = l->lightDef.pos - mi->placementInfo->position;
				float ll = dir.length2();
				if (ll < lenmin)
				{
					lenmin = ll;
					dirmin = dir;
					lmin = j;
				}
			}
			mi->light = lmin;
			mi->ldir = dirmin;
		}
#endif

		m_EnableOutdoorLights = false;
	}
	else
	{
		m_EnableOutdoorLights = true;
	}
}

//

bool WMOGroup::Render()
{
	visible = false;

	BoundingBox aabb = m_Bounds;
	aabb.transform(_Pipeline->GetWorld());

	/*float dist = (aabb.Center - _Camera->Position).length();
	if (dist > Settings::culldistance + m_Bounds.Radius)
	{
		return false;
	}*/

	if (_Camera->_frustum.cullBox(aabb))
	{
		return false;
	}

	visible = true;

	/*if (m_HasVertexColors)
	{
		glDisable(GL_LIGHTING);
		_EnvironmentManager->SetAmbientLights(false);
	}
	else
	{
		if (Settings::lighting)
		{
			if (_EnvironmentManager->skies->hasSkies())
			{
				_EnvironmentManager->SetAmbientLights(true);
			}
			else
			{
				// Default m_Lights
				glEnable(GL_LIGHT0);
				glLightfv(GL_LIGHT0, GL_AMBIENT, glm::value_ptr(vec4(0.4f, 0.4f, 0.4f, 1)));
				glLightfv(GL_LIGHT0, GL_DIFFUSE, glm::value_ptr(vec4(0.8f, 0.8f, 0.8f, 1)));
				glLightfv(GL_LIGHT0, GL_POSITION, glm::value_ptr(vec4(1, 1, 1, 0)));

				glDisable(GL_LIGHT1);
			}
		}
		else
		{
			glDisable(GL_LIGHTING);
		}
	}
	setupFog();*/

	_TechniquesMgr->m_WMO_GeometryPass->Bind();
	_TechniquesMgr->m_WMO_GeometryPass->SetPVW();

	for (uint32 i = 0; i < nBatches; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, globalBuffer);

		// Vertex
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// Texture
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)(nVertices * 3 * sizeof(float)));

		// Normal
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)(nVertices * 5 * sizeof(float)));

		// Color
		if (m_HasVertexColors)
		{
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)(nVertices * 8 * sizeof(float)));
		}

		WMOBatch* batch = &m_WMOBatchIndexes[i];
		WMOMaterial* material = m_ParentWMO->m_Materials[batch->material_id];

		// Materials settings
		material->setup();
		material->texture->Bind(5);

		//_TechniquesMgr->m_WMO_GeometryPass->SetDiffuseColor(fromARGB(material->GetDiffuseColor()));
		_TechniquesMgr->m_WMO_GeometryPass->SetHasMOCV(false);

		glDrawElements(GL_TRIANGLES, batch->indexCount,	GL_UNSIGNED_SHORT, indices + batch->indexStart);
		PERF_INC(PERF_MAP_MODELS_WMOs_GEOMETRY);

		if (m_HasVertexColors)
		{
			glDisableVertexAttribArray(3);
		}

		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

	_TechniquesMgr->m_WMO_GeometryPass->Unbind();

	return true;
}

bool WMOGroup::drawDoodads(uint32 _doodadSet)
{
	if (!visible)
	{
		return false;
	}

	if (nDoodads == 0)
	{
		return false;
	}

	// draw doodads
	for (uint32 i = 0; i < nDoodads; i++)
	{
		uint16 doodadIndex = m_DoodadsIndexes[i];

#ifdef DOODADS_INCL
		if (m_ParentWMO->doodadsets[_doodadSet]->InSet(doodadIndex) || m_ParentWMO->doodadsets[0]->InSet(doodadIndex))
		{
			/*DoodadInstance* doodadInstance = m_ParentWMO->m_MDXInstances[doodadIndex];
			if (!m_EnableOutdoorLights)
			{
				WMOLight::setupOnce(GL_LIGHT2, doodadInstance->ldir, doodadInstance->lcol);
			}*/

			m_ParentWMO->m_MDXInstances[doodadIndex]->Draw();
}
#endif
	}

	return true;
}

bool WMOGroup::drawLiquid()
{
	if (!visible)
	{
		return false;
	}

	if (lq == nullptr)
	{
		return false;
	}

	//setupFog();

	if (m_EnableOutdoorLights)
	{
		//_EnvironmentManager->SetAmbientLights(true);
	}
	else
	{
		// TODO: setup some kind of indoor lighting... ?
		/*_EnvironmentManager->SetAmbientLights(false);
		glEnable(GL_LIGHT2);
		glLightfv(GL_LIGHT2, GL_AMBIENT, vec4(0.1f, 0.1f, 0.1f, 1));
		glLightfv(GL_LIGHT2, GL_DIFFUSE, vec4(0.8f, 0.8f, 0.8f, 1));
		glLightfv(GL_LIGHT2, GL_POSITION, vec4(0, 1, 0, 0));*/
	}

	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	lq->draw();

	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	PERF_INC(PERF_MAP_MODELS_WMOs_LIQUIDS);

	return true;
}

void WMOGroup::setupFog()
{
	if (m_EnableOutdoorLights || fog == -1)
	{
		_EnvironmentManager->SetFog();
	}
	else
	{
		m_ParentWMO->m_Fogs[fog]->setup();
	}
}