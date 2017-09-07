#include "stdafx.h"

#include "sky.h"
#include "world.h"

const float skymul = 36.0f;

Sky::Sky(gLightDBRecord* data)
{
	position = vec3(data->Get_PositionX(), data->Get_PositionY(), data->Get_PositionZ()) / skymul;

	radiusInner = data->Get_RadiusInner() / skymul;
	radiusOuter = data->Get_RadiusOuter() / skymul;

	for (int i = 0; i < 36; i++)
		mmin[i] = -2;

	global = (position.x == 0.0f && position.y == 0.0f && position.z == 0.0f);

	int FirstId = data->Get_DataIDs() * 18;

	for (int i = 0; i < 18; i++)
	{
		try
		{
			auto* rec = gLightIntBandDB.getByID(FirstId + i);
			int entries = rec->getInt(LightIntBandDB::Entries);

			if (entries == 0)
				mmin[i] = -1;
			else
			{
				mmin[i] = rec->getInt(LightIntBandDB::Times);
				for (int l = 0; l < entries; l++)
				{
					SkyColor sc(rec->getInt(LightIntBandDB::Times + l), rec->getInt(LightIntBandDB::Values + l));
					colorRows[i].push_back(sc);
				}
			}
		}
		catch (DBCNotFound)
		{
			Debug::Info("Sky NOT FOUND!!!!!!");
		}
	}
}

vec3 Sky::colorFor(int r, int _time) const
{
	if (mmin[r] < 0)
	{
		return vec3(0, 0, 0);
	}
	vec3 c1, c2;
	int t1, t2;
	size_t last = colorRows[r].size() - 1;

	if (_time < mmin[r])
	{
		// reverse interpolate
		c1 = colorRows[r][last].color;
		c2 = colorRows[r][0].color;
		t1 = colorRows[r][last].time;
		t2 = colorRows[r][0].time + 2880;
		_time += 2880;
	}
	else
	{
		for (size_t i = last; i >= 0; i--)
		{
			if (colorRows[r][i].time <= _time)
			{
				c1 = colorRows[r][i].color;
				t1 = colorRows[r][i].time;

				if (i == last)
				{
					c2 = colorRows[r][0].color;
					t2 = colorRows[r][0].time + 2880;
				}
				else
				{
					c2 = colorRows[r][i + 1].color;
					t2 = colorRows[r][i + 1].time;
				}
				break;
			}
		}
	}

	float tt = (float)(_time - t1) / (float)(t2 - t1);
	return c1*(1.0f - tt) + c2*tt;
}

const float rad = 400.0f;

//.......................top....med....medh........horiz..........bottom
const float angles[] = {90.0f, 30.0f, 15.0f, 5.0f, 0.0f, -30.0f, -90.0f};
const int skycolors[] = {2,      3,      4,    5,    6,    7,     7};
const int cnum = 7;


const int hseg = 32;


//

void MapSkies::draw()
{

}

MapSkies::MapSkies(int mapid)
{
	for (auto it = gLightDB.Records()->begin(); it != gLightDB.Records()->end(); ++it)
	{
		if (mapid == it->second->Get_Map())
		{
			Sky* sky = new Sky(it->second);
			skies.push_back(sky);
		}
	}

	// sort skies from smallest to largest; global last.
	// smaller skies will have precedence when calculating weights to achieve smooth transitions etc.
	sort(skies.begin(), skies.end());

	stars = new Model("Environments\\Stars\\Stars.m2", true);

}

MapSkies::~MapSkies()
{
	delete stars;
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

void MapSkies::initSky(cvec3 _cameraPosition, int _time)
{
	if (skies.empty())
	{
		return;
	}

	CalculateSkiesWeights(_cameraPosition);

	for (int i = 0; i < 18; i++)
		colorSet[i] = vec3(1, 1, 1);

	// interpolation
	for (size_t j = 0; j < skies.size(); j++)
	{
		if (skies[j]->weight > 0)
		{
			for (int i = 0; i < 18; i++)
			{
				if ((skies[j]->colorFor(i, _time).x > 1.0f) || (skies[j]->colorFor(i, _time).y > 1.0f) || (skies[j]->colorFor(i, _time).z > 1.0f))
				{
					Debug::Info("Sky %d %d is out of bounds!", j, i);
					continue;
				}
				colorSet[i] += skies[j]->colorFor(i, _time) * skies[j]->weight;
			}
		}
	}

	for (int i = 0; i < 18; i++)
	{
		colorSet[i] -= vec3(1, 1, 1);
	}
}

bool MapSkies::drawSky(cvec3 pos)
{
	if (skies.empty())
	{
		return false;
	}

	glPushMatrix();
	{
		glTranslatef(pos.x, pos.y, pos.z);

		// Draw sky
		vec3 basepos1[cnum], basepos2[cnum];
		glBegin(GL_QUADS);
		for (int h = 0; h < hseg; h++)
		{

			for (int i = 0; i < cnum; i++)
			{
				basepos1[i] = basepos2[i] = vec3(cosf(angles[i] * PI / 180.0f)*rad, sinf(angles[i] * PI / 180.0f)*rad, 0);
				rotate(0, 0, &basepos1[i].x, &basepos1[i].z, PI*2.0f / hseg*h);
				rotate(0, 0, &basepos2[i].x, &basepos2[i].z, PI*2.0f / hseg*(h + 1));
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
		float ni = _World->dayNightPhase.nightIntensity;
		if (ni > 0)
		{
			const float sc = 0.1f;
			glScalef(sc, sc, sc);
			glEnable(GL_TEXTURE_2D);
			stars->trans = ni;
			stars->draw();
		}
	}
	glPopMatrix();

	return true;
}
