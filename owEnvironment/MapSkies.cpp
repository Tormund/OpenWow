#include "stdafx.h"

// General
#include "MapSkies.h"


const float C_SkyRadius = 400.0f;
const uint32_t C_SkySegmentsCount = 32;

//............................top....med....medh........horiz..........bottom
const float C_SkyAngles[] = {90.0f, 30.0f, 15.0f, 5.0f, 0.0f, -30.0f, -90.0f};
const uint32_t C_Skycolors[] = {SKY_COLOR_0,      SKY_COLOR_1,      SKY_COLOR_2,    SKY_COLOR_3,    SKY_COLOR_4,    FOG_COLOR,     FOG_COLOR};
const uint32_t C_SkycolorsCount = 7;

MapSkies::MapSkies(uint32_t mapid)
{
	for (auto it = gLightDB.Records()->begin(); it != gLightDB.Records()->end(); ++it)
	{
		if (mapid == it->second->Get_Map())
		{
			Sky* sky = new Sky(it->second);
			skies.push_back(sky);

			//Debug::Warn("Sky [%d] position = %f, %f, %f", it->second->Get_Map(), sky->position.x, sky->position.y, sky->position.z);
		}
	}

	sort(skies.begin(), skies.end());

	InitBuffer();

	/*stars = new MDX("Environments\\Stars\\Stars.m2");  // BOUZI FIXME ENABLE ME
	stars->Init(true);*/
}

MapSkies::~MapSkies()
{
	glDeleteBuffers(1, &m_GlobalBuffer);

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

	for (uint32_t h = 0; h < C_SkySegmentsCount; h++)
	{
		for (uint32_t i = 0; i < C_SkycolorsCount; i++)
		{
			basepos1[i] = basepos2[i] = vec3(cosf(C_SkyAngles[i] * DEG_TO_RAD) * C_SkyRadius, sinf(C_SkyAngles[i] * DEG_TO_RAD) * C_SkyRadius, 0);
			rotate(0, 0, &basepos1[i].x, &basepos1[i].z, M_2_PI / C_SkySegmentsCount * (h + 0));
			rotate(0, 0, &basepos2[i].x, &basepos2[i].z, M_2_PI / C_SkySegmentsCount * (h + 1));
		}

		for (uint32_t v = 0; v < C_SkycolorsCount - 1; v++)
		{
			//glColor3fv(glm::value_ptr(colorSet[skycolors[v]]));
			//glVertex3fv(glm::value_ptr(basepos2[v]));
			//glVertex3fv(glm::value_ptr(basepos1[v]));

			//glColor3fv(glm::value_ptr(colorSet[skycolors[v + 1]]));
			//glVertex3fv(glm::value_ptr(basepos1[v + 1]));
			//glVertex3fv(glm::value_ptr(basepos2[v + 1]));

			vertices.push_back(basepos2[v]);
			vertices.push_back(basepos1[v]);
			vertices.push_back(basepos1[v + 1]);
			vertices.push_back(basepos2[v + 1]);
		}
	}

	m_GlobalBufferSize = vertices.size();

	glGenBuffers(1, &m_GlobalBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, m_GlobalBuffer);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float) * m_GlobalBufferSize, NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, m_GlobalBufferSize * 0 * sizeof(float), m_GlobalBufferSize * sizeof(vec3), vertices.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void MapSkies::initSky(cvec3 _cameraPosition, int _time)
{
	if (skies.empty())
	{
		return;
	}

	CalculateSkiesWeights(_cameraPosition);

	for (int i = 0; i < 18; i++)
	{
		colorSet[i] = vec3(1, 1, 1);
	}

	// interpolation
	for (size_t j = 0; j < skies.size(); j++)
	{
		if (skies[j]->weight > 0)
		{
			for (uint32_t i = 0; i < 18; i++)
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

	for (uint32_t i = 0; i < 18; i++)
	{
		colorSet[i] -= vec3(1, 1, 1);
	}


	colors.clear();


	for (uint32_t h = 0; h < C_SkySegmentsCount; h++)
	{
		for (uint32_t v = 0; v < C_SkycolorsCount - 1; v++)
		{
			colors.push_back(colorSet[C_Skycolors[v]]);
			colors.push_back(colorSet[C_Skycolors[v]]);
			colors.push_back(colorSet[C_Skycolors[v + 1]]);
			colors.push_back(colorSet[C_Skycolors[v + 1]]);
		}
	}

	// Fill buffer with color
	glBindBuffer(GL_ARRAY_BUFFER, m_GlobalBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, m_GlobalBufferSize * 3 * sizeof(float), m_GlobalBufferSize * sizeof(vec3), colors.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void MapSkies::CalculateSkiesWeights(cvec3 pos)
{
	skies.back()->weight = 1.0f;

	for (int i = skies.size() - 2; i >= 0; i--)
	{
		Sky* s = skies[i];
		const float dist = glm::length(pos - s->position);

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

	_TechniquesMgr->m_Sky_GeometryPass->Bind();

	_Pipeline->Clear();
	_Pipeline->Translate(pos);
	_TechniquesMgr->m_Sky_GeometryPass->SetPVW();

	glBindBuffer(GL_ARRAY_BUFFER, m_GlobalBuffer);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)(m_GlobalBufferSize * sizeof(vec3)));

	glDrawArrays(GL_QUADS, 0, m_GlobalBufferSize);

	//_Perfomance->Inc(PERF_MAP_MH20);

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	_TechniquesMgr->m_Sky_GeometryPass->Unbind();



	/*glPushMatrix();
	{
		glTranslatef(pos.x, pos.y, pos.z);

		// Draw sky
		vec3 basepos1[cnum], basepos2[cnum];
		glBegin(GL_QUADS);
		for (int h = 0; h < C_SkySegmentsCount; h++)
		{

			for (int i = 0; i < cnum; i++)
			{
				basepos1[i] = basepos2[i] = vec3(cosf(C_SkyAngles[i] * DEG_TO_RAD) * rad, sinf(C_SkyAngles[i] * DEG_TO_RAD) * rad, 0);
				rotate(0, 0, &basepos1[i].x, &basepos1[i].z, M_2_PI / C_SkySegmentsCount * (h + 0));
				rotate(0, 0, &basepos2[i].x, &basepos2[i].z, M_2_PI / C_SkySegmentsCount * (h + 1));
			}

			for (int v = 0; v < cnum - 1; v++)
			{
				glColor3fv(glm::value_ptr(colorSet[skycolors[v]]));
				glVertex3fv(glm::value_ptr(basepos2[v]));
				glVertex3fv(glm::value_ptr(basepos1[v]));

				glColor3fv(glm::value_ptr(colorSet[skycolors[v + 1]]));
				glVertex3fv(glm::value_ptr(basepos1[v + 1]));
				glVertex3fv(glm::value_ptr(basepos2[v + 1]));
			}
		}
		glEnd();

		// if it's night, draw the stars
		//float ni = _World->dayNightPhase.nightIntensity; // BOUZI FIXME ENABLE ME
		//if (ni > 0)
		//{
		//const float sc = 0.1f;
		//glScalef(sc, sc, sc);
		//glEnable(GL_TEXTURE_2D);
		//stars->trans = ni;
		//stars->draw();
		//}
	}
	glPopMatrix();*/

	return true;
}
