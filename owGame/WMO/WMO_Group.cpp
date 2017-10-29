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

struct WMOGroupInfoDef
{
	WMOGroupFlags flags;
	CAaBox bounding_box;
	int32 nameoffset;                                   // name in MOGN chunk (-1 for no name)

	//
	static const uint32 __size = 32;
};

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

	m_Indices = nullptr;

	m_VertexesCount = 0;
	m_Vertexes = nullptr;

	m_Normals = nullptr;
	m_TextureCoords = nullptr;


	m_WMOBatchIndexesCount = 0;
	m_WMOBatchIndexes = nullptr;


	m_WMOLightsIndexesCount = 0;
	m_WMOLightsIndexes = nullptr;


	m_DoodadsIndexesCount = 0;
	m_DoodadsIndexes = nullptr;

	m_VertexColors = nullptr;

	m_Liquid = nullptr;

	// Read hgroup info for bounding box and name
	WMOGroupInfoDef groupInfo;
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
	delete[] m_Indices;
	delete[] m_WMOBatchIndexes;
	delete[] m_DoodadsIndexes;

	if (m_Liquid) delete m_Liquid;
}

void WMOGroup::Load()
{
	// open group file
	char temp[256];
	strcpy_s(temp, m_ParentWMO->GetName().c_str());
	temp[m_ParentWMO->GetName().length() - 4] = 0;

	char fname[256];
	sprintf_s(fname, "%s_%03d.wmo", temp, m_GroupIndex);

	File f = fname;
	if (!f.Open())
	{
		return;
	}

	uint32 MOTVCount = 0;
	uint32 MOCVCount = 0;

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

		if (strcmp(fourcc, "MVER") == 0)
		{
			uint32 version;
			f.ReadBytes(&version, 4);
			assert1(version == 17);
		}
		else if (strcmp(fourcc, "MOGP") == 0)
		{
			nextpos = f.GetPos() + WMOGroupHeader::__size; // The MOGP chunk size will be way more than the header variables!

			f.ReadBytes(&m_Header, WMOGroupHeader::__size);

			// Bounds
			m_Bounds.set(m_Header.boundingBox.min, m_Header.boundingBox.max, true);

			// Multi texture coords
			if (m_Header.flags.FLAG_HAS_3_MOTV)
			{
				m_TextureCoords = new vec2*[3];
			}
			else if (m_Header.flags.FLAG_HAS_2_MOTV)
			{
				m_TextureCoords = new vec2*[2];
			}
			else
			{
				m_TextureCoords = new vec2*[1];
			}

			//

			// Multi vertex colors
			if (m_Header.flags.FLAG_HAS_VERTEX_COLORS || m_Header.flags.FLAG_HAS_2_MOCV) // Flag doesnot set
			{
				if (m_Header.flags.FLAG_HAS_2_MOCV)
				{
					m_VertexColors = new uint32*[2];
				}
				else
				{
					m_VertexColors = new uint32*[1];
				}
			}
		}
		else if (strcmp(fourcc, "MOPY") == 0) // Material info for triangles
		{
			m_MaterialsInfoCount = size / WMOGroup_MaterialInfo::__size;
			m_MaterialsInfo = (WMOGroup_MaterialInfo*)f.GetDataFromCurrent();
		}
		else if (strcmp(fourcc, "MOVI") == 0) // Vertex indices for triangles
		{
			m_IndicesCount = size / sizeof(uint16);
			m_Indices = (uint16*)f.GetDataFromCurrent();
		}
		else if (strcmp(fourcc, "MOVT") == 0) // Vertices chunk.
		{
			m_VertexesCount = size / sizeof(vec3);
			m_Vertexes = (vec3*)f.GetDataFromCurrent();
		}
		else if (strcmp(fourcc, "MONR") == 0) // Normals
		{
			m_Normals = (vec3*)f.GetDataFromCurrent();
		}
		else if (strcmp(fourcc, "MOTV") == 0) // Texture coordinates
		{
			m_TextureCoords[MOTVCount] = (vec2*)f.GetDataFromCurrent();
			MOTVCount++;
		}
		else if (strcmp(fourcc, "MOBA") == 0) // Render m_WMOBatchIndexes.
		{
			m_WMOBatchIndexesCount = size / WMOBatch::__size;
			m_WMOBatchIndexes = new WMOBatch[m_WMOBatchIndexesCount];
			for (uint32 i = 0; i < m_WMOBatchIndexesCount; i++)
			{
				// Read
				f.ReadBytes(&m_WMOBatchIndexes[i], WMOBatch::__size);

				// Set material
				WMOBatch* _batch = &m_WMOBatchIndexes[i];
				WMOMaterial* _wmoMat = m_ParentWMO->m_Materials[_batch->material_id];

				_batch->__material.SetDiffuseTexture(_wmoMat->texture->GetObj());

				_batch->__material.SetBlendState(_wmoMat->GetBlendMode());
				_batch->__material.SetRenderState(_wmoMat->IsTwoSided());
			}
		}
		else if (strcmp(fourcc, "MOLR") == 0) // Light references
		{
			assert1(m_Header.flags.FLAG_HAS_LIGHTS);
			m_WMOLightsIndexesCount = size / sizeof(uint16);
			m_WMOLightsIndexes = (uint16*)f.GetDataFromCurrent();
		}
		else if (strcmp(fourcc, "MODR") == 0) // Doodad references
		{
			assert1(m_Header.flags.FLAG_HAS_DOODADS);
			m_DoodadsIndexesCount = size / sizeof(uint16);
			m_DoodadsIndexes = new uint16[size / sizeof(uint16)];
			f.ReadBytes(m_DoodadsIndexes, size);
		}
		else if (strcmp(fourcc, "MOBN") == 0)
		{
			assert1(m_Header.flags.FLAG_HAS_COLLISION);
		}
		else if (strcmp(fourcc, "MOBR") == 0)
		{
			assert1(m_Header.flags.FLAG_HAS_COLLISION);
		}
		else if (strcmp(fourcc, "MOCV") == 0) // Vertex colors
		{
			assert1(m_Header.flags.FLAG_HAS_VERTEX_COLORS || m_Header.flags.FLAG_HAS_2_MOCV);
			m_Header.flags.FLAG_HAS_VERTEX_COLORS = true;
			m_VertexColors[MOCVCount] = (uint32*)f.GetDataFromCurrent();
			MOCVCount++;
		}
		else if (strcmp(fourcc, "MLIQ") == 0) // Liquid
		{
			WMO_Group_MLIQ liquidHeader;
			f.ReadBytes(&liquidHeader, WMO_Group_MLIQ::__size);

			if (m_Header.liquidType > 0)
			{
				Modules::log().Green("WMO[%s]: Contain liquid! [%s]", m_ParentWMO->GetName().c_str(), DBC_LiquidType[m_Header.liquidType]->Get_Name());

				m_Liquid = new Liquid(liquidHeader.A, liquidHeader.B, From_XYZ_To_XZminusY_RET(liquidHeader.pos));
				m_Liquid->initFromWMO2(f, m_ParentWMO->m_Materials[liquidHeader.type], DBC_LiquidType[m_Header.liquidType], m_Header.flags.FLAG_IS_INDOOR);
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
			Modules::log().Info("WMO_Group[]: No implement group chunk %s [%d].", fourcc, size);
		}

		f.Seek(nextpos);
	}

	//

	//

	WMOFog* wf = m_ParentWMO->m_Fogs[m_Header.m_Fogs[0]];
	if (wf->fogDef.largerRadius <= 0)
		fog = -1; // default outdoor fog..?
	else
		fog = m_Header.m_Fogs[0];

	//

	vec4* vertexColors = nullptr;

	if (m_Header.flags.FLAG_HAS_VERTEX_COLORS)
	{
		vertexColors = new vec4[m_VertexesCount];
	}

	float v35 = 0;
	float v36 = 0;
	float v37 = 0;

	if (m_ParentWMO->m_Header.flags.FLAG_skip_base_color)
	{
		v35 = 0;
		v36 = 0;
		v37 = 0;
	}
	else
	{
		v35 = m_ParentWMO->m_Header.ambColor.r;
		v37 = m_ParentWMO->m_Header.ambColor.g;
		v36 = m_ParentWMO->m_Header.ambColor.b;
	}

	// Converts
	for (uint32 i = 0; i < m_VertexesCount; i++)
	{
		m_Vertexes[i] = From_XYZ_To_XZminusY_RET(m_Vertexes[i]);
		m_Normals[i] = From_XYZ_To_XZminusY_RET(m_Normals[i]);

		if (m_Header.flags.FLAG_HAS_VERTEX_COLORS)
		{
			vertexColors[i] = fromBGRA(m_VertexColors[0][i]);

			if (m_ParentWMO->m_Header.flags.FLAG_lighten_interiors)
			{
				vertexColors[i].w = m_Header.flags.FLAG_IS_OUTDOOR ? 1.0f : 0.0f;
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

				vertexColors[i].w = m_Header.flags.FLAG_IS_OUTDOOR ? 1.0f : 0.0f;
			}
		}
		//else
		//{
		//	vertexColors[i] = vec4(0.0f, 0.3f, 0.6f, 0.9f);
		//}
	}

	initLighting();

	uint32 bufferSize = 8 * sizeof(float);
	if (m_Header.flags.FLAG_HAS_VERTEX_COLORS)
	{
		bufferSize += sizeof(vec4);
	}

	// Vertex buffer
	uint32 __vb = _Render->r->createVertexBuffer(m_VertexesCount * bufferSize, nullptr);

	_Render->r->updateBufferData(__vb, m_VertexesCount * 0 * sizeof(float), m_VertexesCount * sizeof(vec3), m_Vertexes);
	_Render->r->updateBufferData(__vb, m_VertexesCount * 3 * sizeof(float), m_VertexesCount * sizeof(vec2), m_TextureCoords[0]); // FIXME
	_Render->r->updateBufferData(__vb, m_VertexesCount * 5 * sizeof(float), m_VertexesCount * sizeof(vec3), m_Normals);
	if (m_Header.flags.FLAG_HAS_VERTEX_COLORS)
	{
		_Render->r->updateBufferData(__vb, m_VertexesCount * 8 * sizeof(float), m_VertexesCount * sizeof(vec4), vertexColors);
	}

	//

	__geom = _Render->r->beginCreatingGeometry(m_Header.flags.FLAG_HAS_VERTEX_COLORS ? _RenderStorage->__layoutWMO_VC : _RenderStorage->__layoutWMO);

	// Vertex params
	_Render->r->setGeomVertexParams(__geom, __vb, 0, m_VertexesCount * 0 * sizeof(float), 0);
	_Render->r->setGeomVertexParams(__geom, __vb, 1, m_VertexesCount * 3 * sizeof(float), 0);
	_Render->r->setGeomVertexParams(__geom, __vb, 2, m_VertexesCount * 5 * sizeof(float), 0);
	if (m_Header.flags.FLAG_HAS_VERTEX_COLORS)
	{
		_Render->r->setGeomVertexParams(__geom, __vb, 3, m_VertexesCount * 8 * sizeof(float), 0);
	}

	// Index bufer
	uint32 __ib = _Render->r->createIndexBuffer(m_IndicesCount * sizeof(uint16), m_Indices);
	_Render->r->setGeomIndexParams(__geom, __ib, R_IndexFormat::IDXFMT_16);

	// Finish
	_Render->r->finishCreatingGeometry(__geom);

	//


	delete[] m_TextureCoords;

	if (m_Header.flags.FLAG_HAS_VERTEX_COLORS)
	{
		delete[] vertexColors;
		delete[] m_VertexColors;
	}
}

void WMOGroup::initLighting()
{
	if (m_Header.flags.FLAG_IS_INDOOR && m_Header.flags.FLAG_HAS_VERTEX_COLORS)
	{
		vec3 dirmin(1, 1, 1);
		float lenmin;
		int lmin;

#ifdef DOODADS_INCL
		for (uint32 i = 0; i < m_DoodadsIndexesCount; i++)
		{
			lenmin = 999999.0f * 999999.0f;
			lmin = 0;
			DoodadInstance* mi = m_ParentWMO->m_MDXInstances[m_DoodadsIndexes[i]];
			for (uint32 j = 0; j < m_ParentWMO->m_Header.nLights; j++)
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
	if (dist > Modules::config().culldistance + m_Bounds.Radius)
	{
		return false;
	}*/

	if (_CameraFrustum->_frustum.cullBox(aabb))
	{
		return false;
	}

	visible = true;

	_TechniquesMgr->m_WMO_GeometryPass->BindS();
	_TechniquesMgr->m_WMO_GeometryPass->SetPVW();

	_Render->r->setGeometry(__geom);

	for (uint32 i = 0; i < m_WMOBatchIndexesCount; i++)
	{
		WMOBatch* batch = &m_WMOBatchIndexes[i];
		WMOMaterial* material = m_ParentWMO->m_Materials[batch->material_id];

		batch->__material.Set();

		//_TechniquesMgr->m_WMO_GeometryPass->SetDiffuseColor(fromARGB(material->GetDiffuseColor()));
		_TechniquesMgr->m_WMO_GeometryPass->SetHasMOCV(false);

		_Render->r->drawIndexed(PRIM_TRILIST, batch->indexStart, batch->indexCount, batch->vertexStart, (batch->vertexEnd - batch->vertexStart), false);
	}

	return true;
}

bool WMOGroup::drawDoodads(uint32 _doodadSet)
{
	if (!visible)
	{
		return false;
	}

	if (m_DoodadsIndexesCount == 0)
	{
		return false;
	}

	// draw doodads
	for (uint32 i = 0; i < m_DoodadsIndexesCount; i++)
	{
		uint16 doodadIndex = m_DoodadsIndexes[i];

#ifdef DOODADS_INCL
		if (m_ParentWMO->doodadsets[_doodadSet]->InSet(doodadIndex) || m_ParentWMO->doodadsets[0]->InSet(doodadIndex))
		{
			m_ParentWMO->m_MDXInstances[doodadIndex]->Render();
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

	if (m_Liquid == nullptr)
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

	m_Liquid->draw();

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