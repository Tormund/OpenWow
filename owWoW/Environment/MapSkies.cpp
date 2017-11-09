#include "../stdafx.h"

// General
#include "MapSkies.h"


const float C_SkyRadius = 400.0f;
const uint32 C_SkySegmentsCount = 32;

//............................top....med....medh........horiz..........bottom
const float C_SkyAngles[] = {90.0f, 30.0f, 15.0f, 5.0f, 0.0f, -30.0f, -90.0f};
const uint32 C_Skycolors[] = {SKY_COLOR_0,      SKY_COLOR_1,      SKY_COLOR_2,    SKY_COLOR_3,    SKY_COLOR_4,    FOG_COLOR,     FOG_COLOR};
const uint32 C_SkycolorsCount = 7;

MapSkies::MapSkies(uint32 mapid)
{
	for (auto it = DBC_Light.Records()->begin(); it != DBC_Light.Records()->end(); ++it)
	{
		if (mapid == it->second->Get_MapID()->Get_ID())
		{
			Sky* sky = new Sky(it->second);
			skies.push_back(sky);

			//Log::Warn("Sky [%d] position = %f, %f, %f", it->second->Get_Map(), sky->position.x, sky->position.y, sky->position.z);
		}
	}

	sort(skies.begin(), skies.end());

	InitBuffer();

	/*stars = new MDX("Environments\\Stars\\Stars.m2");  // BOUZI FIXME ENABLE ME
	stars->Init(true);*/
}

MapSkies::~MapSkies()
{
	/*delete stars;*/  // BOUZI FIXME ENABLE ME
}

void MapSkies::InitBuffer()
{
	if (skies.empty())
	{
		return;
	}

	// Draw sky
	vec3 basepos1[C_SkycolorsCount];
	vec3 basepos2[C_SkycolorsCount];

	vector<vec3> vertices;

	for (uint32 h = 0; h < C_SkySegmentsCount; h++)
	{
		for (uint32 i = 0; i < C_SkycolorsCount; i++)
		{
			basepos1[i] = basepos2[i] = vec3(cosf(C_SkyAngles[i] * DEG_TO_RAD) * C_SkyRadius, sinf(C_SkyAngles[i] * DEG_TO_RAD) * C_SkyRadius, 0);
			rotate(0, 0, &basepos1[i].x, &basepos1[i].z, M_2_PI / C_SkySegmentsCount * (h + 0));
			rotate(0, 0, &basepos2[i].x, &basepos2[i].z, M_2_PI / C_SkySegmentsCount * (h + 1));
		}

		for (uint32 v = 0; v < C_SkycolorsCount - 1; v++)
		{
			vertices.push_back(basepos1[v]);
			vertices.push_back(basepos1[v + 1]);
			vertices.push_back(basepos2[v + 1]);

			vertices.push_back(basepos2[v + 1]);
			vertices.push_back(basepos2[v]);
			vertices.push_back(basepos1[v]);
		}
	}
	__vertsSize = vertices.size();


	// Vertex buffer
	__vb = _Render->r->createVertexBuffer(2 * __vertsSize * sizeof(vec3), nullptr);

	_Render->r->updateBufferData(__vb, 0, __vertsSize * sizeof(vec3), vertices.data());

	//

	__geom = _Render->r->beginCreatingGeometry(_RenderStorage->__layoutSky);

	// Vertex params
	_Render->r->setGeomVertexParams(__geom, __vb, R_DataType::T_FLOAT, 0,                              0);
	_Render->r->setGeomVertexParams(__geom, __vb, R_DataType::T_FLOAT, __vertsSize * sizeof(vec3),     0);

	// Index bufer
	//uint32 __ib = _Render->r->createIndexBuffer(striplen, strip);
	//_Render->r->setGeomIndexParams(lowrestiles[j][i], __ib, R_IndexFormat::IDXFMT_16);

	// Finish
	_Render->r->finishCreatingGeometry(__geom);
}

void MapSkies::initSky(cvec3 _cameraPosition, uint32 _time)
{
	if (skies.empty())
	{
		return;
	}

	CalculateSkiesWeights(_cameraPosition);

	for (int i = 0; i < SKY_COLORSCOUNT; i++)
	{
		colorSet[i] = vec3(1, 1, 1);
	}

	// interpolation
	for (size_t j = 0; j < skies.size(); j++)
	{
		if (skies[j]->weight > 0)
		{
			for (uint32 i = 0; i < SKY_COLORSCOUNT; i++)
			{
				if ((skies[j]->colorFor(i, _time).x > 1.0f) || (skies[j]->colorFor(i, _time).y > 1.0f) || (skies[j]->colorFor(i, _time).z > 1.0f))
				{
					fail1();
					continue;
				}
				colorSet[i] += skies[j]->colorFor(i, _time) * skies[j]->weight;
			}
		}
	}

	for (uint32 i = 0; i < SKY_COLORSCOUNT; i++)
	{
		colorSet[i] -= vec3(1, 1, 1);
	}


	colors.clear();


	for (uint32 h = 0; h < C_SkySegmentsCount; h++)
	{
		for (uint32 v = 0; v < C_SkycolorsCount - 1; v++)
		{
			colors.push_back(colorSet[C_Skycolors[v]]);
			colors.push_back(colorSet[C_Skycolors[v + 1]]);
			colors.push_back(colorSet[C_Skycolors[v + 1]]);
			colors.push_back(colorSet[C_Skycolors[v + 1]]);
			colors.push_back(colorSet[C_Skycolors[v]]);
			colors.push_back(colorSet[C_Skycolors[v]]);
		}
	}

	// Fill buffer with color

	_Render->r->updateBufferData(__vb, __vertsSize * sizeof(vec3), __vertsSize * sizeof(vec3), colors.data());
}

void MapSkies::CalculateSkiesWeights(cvec3 pos)
{
	skies.back()->weight = 1.0f;

	for (int i = skies.size() - 2; i >= 0; i--)
	{
		Sky* s = skies[i];
		const float dist = (pos - s->position).length();

		if (dist < s->radiusInner)
		{
			// we're in a sky, zero out the rest
			s->weight = 1.0f;
			for (size_t j = i + 1; j < skies.size(); j++)
			{
				skies[j]->weight = 0.0f;
			}
		}
		else if (dist < s->radiusOuter)
		{
			// we're in an outer area, scale down the other weights
			float r = (dist - s->radiusInner) / (s->radiusOuter - s->radiusInner);
			s->weight = 1.0f - r;
			for (size_t j = i + 1; j < skies.size(); j++)
			{
				skies[j]->weight *= r;
			}
		}
		else
		{
			s->weight = 0.0f;
		}
	}
}

bool MapSkies::drawSky(cvec3 pos)
{
	if (skies.empty())
	{
		return false;
	}

	_Pipeline->Clear();
	_Pipeline->Translate(pos);

	_TechniquesMgr->m_Sky_GeometryPass->BindS();
	_TechniquesMgr->m_Sky_GeometryPass->SetPVW();

	_Render->r->setGeometry(__geom);
	_Render->r->draw(PRIM_TRILIST, 0, __vertsSize);

	return true;
}
