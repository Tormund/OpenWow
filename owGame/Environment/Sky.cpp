#include "../stdafx.h"

// General
#include "Sky.h"

// Additional
#include "../DBC__Storage.h"

const float skymul = 36.0f;

Sky::Sky(DBC_LightRecord* data)
{
	if (data == nullptr)
	{
		return;
	}

	position.x = data->Get_PositionX() / skymul;
	position.y = data->Get_PositionY() / skymul;
	position.z = data->Get_PositionZ() / skymul;

	radiusInner = data->Get_RadiusInner() / skymul;
	radiusOuter = data->Get_RadiusOuter() / skymul;

	for (int i = 0; i < 18; i++)
	{
		mmin[i] = -2;
	}

	global = (position.x == 0.0f && position.y == 0.0f && position.z == 0.0f);

	uint32 ParamsClear = data->Get_Params(0) * 18;

	for (uint32 i = 0; i < 18; i++)
	{

		auto rec = DBC_LightIntBand[ParamsClear + i];
		if (rec == nullptr)
		{
			Modules::log().Error("Sky NOT FOUND!!!!!!");
			continue;
		}

		uint32 entries = rec->Get_Count();

		if (entries == 0)
		{
			mmin[i] = -1;
		}
		else
		{
			mmin[i] = rec->Get_Times(0);
			for (int l = 0; l < entries; l++)
			{
				SkyColor sc(rec->Get_Times(l), rec->Get_Values(l));
				colorRows[i].push_back(sc);
			}
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
		for (uint32 i = last; i >= 0; i--)
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
