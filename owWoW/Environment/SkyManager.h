#pragma once

#include "Sky.h"

class SkyManager
{
public:
	SkyManager(DBC_MapRecord* _mapRecord);
	~SkyManager();

public:
	void Calculate(cvec3 _cameraPosition, uint32 _time);
    bool HasSkies() { return !skies.empty(); }



    vec3 GetColor(LightColors _color)
    {
        return m_Interpolated.m_InterpolatedColors[_color];
    }
    float GetFog(LightFogs _fog)
    {
        return m_Interpolated.m_InterpolatedFogs[_fog];
    }
    float GetGlow()
    {
        return m_Interpolated.m_glow;
    }
    float GetWaterShallowAlpha()
    {
        return m_Interpolated.m_waterShallowAlpha;
    }
    float GetWaterDarkAlpha()
    {
        return m_Interpolated.m_waterDeepAlpha;
    }
    float GetOceanShallowAlpha()
    {
        return m_Interpolated.m_oceanShallowAlpha;
    }
    float GetOceanDarkAlpha()
    {
        return m_Interpolated.m_oceanDeepAlpha;
    }


	bool drawSky(cvec3 pos);
    bool DEBUG_Render();

private:
    void InitBuffer();
    void CalculateSkiesWeights(cvec3 pos);

private:
    SkyParams m_Interpolated;
	uint32 __vb;
	uint32 __geom;
	uint32 __vertsSize;

	vector<Sky*> skies;
	//MDX* stars;  // BOUZI FIXME ENABLE ME
};

