#include "../stdafx.h"

// General
#include "SkyManager.h"


const float C_SkyRadius = 400.0f;
const uint32 C_SkySegmentsCount = 32;

//............................top....med....medh........horiz..........bottom
const float C_SkyAngles[] = {90.0f, 30.0f, 15.0f, 5.0f, 0.0f, -30.0f, -90.0f};
const uint32 C_Skycolors[] = {LIGHT_COLOR_SKY_0,      LIGHT_COLOR_SKY_1,      LIGHT_COLOR_SKY_2,    LIGHT_COLOR_SKY_3,    LIGHT_COLOR_SKY_4,    LIGHT_COLOR_FOG,     LIGHT_COLOR_FOG};
const uint32 C_SkycolorsCount = 7;

SkyManager::SkyManager(DBC_MapRecord* _mapRecord)
{
	for (auto it = DBC_Light.Records()->begin(); it != DBC_Light.Records()->end(); ++it)
	{
		if (_mapRecord == it->second->Get_MapID())
		{
			Sky* sky = new Sky(it->second);
			skies.push_back(sky);

			//Log::Warn("Sky [%d] position = %f, %f, %f", it->second->Get_Map(), sky->position.x, sky->position.y, sky->position.z);
		}
	}

    std::sort(skies.begin(), skies.end(), [](const Sky* lhs, const Sky* rhs)
    {
        if (lhs->m_IsGlobalSky)
            return false;
        else if (rhs->m_IsGlobalSky)
            return true;
        else
            return lhs->m_Range.max < rhs->m_Range.max;
    });

    if (skies.size() > 0 && !skies.back()->m_IsGlobalSky)
    {
        Log::Error("Sky for maps [%d] size [%d] don't have global sky!!!", _mapRecord->Get_ID(), skies.size());
        skies.back()->m_IsGlobalSky = true;
    }

	InitBuffer();

	/*stars = new MDX("Environments\\Stars\\Stars.m2");  // BOUZI FIXME ENABLE ME
	stars->Init(true);*/
}

SkyManager::~SkyManager()
{
	/*delete stars;*/  // BOUZI FIXME ENABLE ME
}

void SkyManager::Calculate(cvec3 _cameraPosition, uint32 _time)
{
    if (skies.empty())
    {
        return;
    }

    CalculateSkiesWeights(_cameraPosition);

    m_Interpolated.Clear();

    // interpolation
    for (auto it : skies)
    {
        if (it->m_Wight > 0.0f)
        {
            SkyParams params = it->GetByTime(_time);
            params *= it->m_Wight;

            m_Interpolated += params;
        }
    }

    vector<vec3> colors;

    for (uint32 h = 0; h < C_SkySegmentsCount; h++)
    {
        for (uint32 v = 0; v < C_SkycolorsCount - 1; v++)
        {
            colors.push_back(m_Interpolated.m_InterpolatedColors[C_Skycolors[v]]);
            colors.push_back(m_Interpolated.m_InterpolatedColors[C_Skycolors[v + 1]]);
            colors.push_back(m_Interpolated.m_InterpolatedColors[C_Skycolors[v + 1]]);
            colors.push_back(m_Interpolated.m_InterpolatedColors[C_Skycolors[v + 1]]);
            colors.push_back(m_Interpolated.m_InterpolatedColors[C_Skycolors[v]]);
            colors.push_back(m_Interpolated.m_InterpolatedColors[C_Skycolors[v]]);
        }
    }

    // Fill buffer with color

    _Render->r->updateBufferData(__vb, __vertsSize * sizeof(vec3), __vertsSize * sizeof(vec3), colors.data());
}

bool SkyManager::drawSky(cvec3 pos)
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

bool SkyManager::DEBUG_Render()
{
    _Render->r->setFillMode(R_FillMode::RS_FILL_WIREFRAME);
    _TechniquesMgr->m_Debug_GeometryPass->BindS();

    _Render->r->setGeometry(_RenderStorage->_sphereGeo);

    for (auto it : skies)
    {
        /*_Pipeline->Clear();
        _Pipeline->Translate(it->position);
        _Pipeline->Scale(it->radiusInner);

        _TechniquesMgr->m_Debug_GeometryPass->SetPVW();
        _TechniquesMgr->m_Debug_GeometryPass->SetColor4(vec4(1.0f, 0.0f, 1.0f, 0.3f));

        _Render->r->drawIndexed(PRIM_TRILIST, 0, 128 * 3, 0, 126, false);*/

        //---------------------------------------------------------------------------------

        _Pipeline->Clear();
        _Pipeline->Translate(it->m_Position);
        _Pipeline->Scale(it->m_Range.max);

        _TechniquesMgr->m_Debug_GeometryPass->SetPVW();
        _TechniquesMgr->m_Debug_GeometryPass->SetColor4(vec4(1.0f, 1.0f, 0.0f, 0.3f));

        _Render->r->drawIndexed(PRIM_TRILIST, 0, 128 * 3, 0, 126, false);
    }

    _TechniquesMgr->m_Debug_GeometryPass->Unbind();
    _Render->r->setFillMode(R_FillMode::RS_FILL_SOLID);

    return false;
}

void SkyManager::InitBuffer()
{
	// Draw sky
	vec3 basepos1[C_SkycolorsCount];
	vec3 basepos2[C_SkycolorsCount];

	vector<vec3> vertices;

	for (uint32 h = 0; h < C_SkySegmentsCount; h++)
	{
		for (uint32 i = 0; i < C_SkycolorsCount; i++)
		{
			basepos1[i] = basepos2[i] = vec3(cosf(degToRad(C_SkyAngles[i])), sinf(degToRad(C_SkyAngles[i])), 0.0f) * C_SkyRadius;
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

void SkyManager::CalculateSkiesWeights(cvec3 pos)
{
	skies.back()->m_Wight = 1.0f;
    assert1(skies.back()->m_IsGlobalSky);

	for (int i = skies.size() - 2; i >= 0; i--)
	{
		Sky* s = skies[i];
		const float dist = (pos - s->m_Position).length();

		if (dist < s->m_Range.min)
		{
			// we're in a sky, zero out the rest
			s->m_Wight = 1.0f;
			for (size_t j = i + 1; j < skies.size(); j++)
			{
				skies[j]->m_Wight = 0.0f;
			}
		}
		else if (dist < s->m_Range.max)
		{
			// we're in an outer area, scale down the other weights
			float r = (dist - s->m_Range.min) / (s->m_Range.max - s->m_Range.min);
			s->m_Wight = 1.0f - r;
			for (size_t j = i + 1; j < skies.size(); j++)
			{
				skies[j]->m_Wight *= r;
			}
		}
		else
		{
			s->m_Wight = 0.0f;
		}
	}
}