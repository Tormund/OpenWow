#pragma once

enum LightParamsNames
{
    ParamsClear, 	// Used in clear weather.
    ParamsClearWat, // Used in clear weather while being underwater.
    ParamsStorm,    // Used in rainy / snowy / sandstormy weather.
    ParamsStormWat, // Used in rainy / snowy / sandstormy weather while being underwater.
    ParamsDeath,	// Only 4 and in newer ones 3 are used as value here(with some exceptions).Changing this seems to have no effect in 3.3.5a(is death light setting hardcoded ? )
    ParamsUnk1, 	// Following 3 Params are used only in WotLK records, and not very commonly.Perhaps used in different phases.
    ParamsUnk2,
    ParamsUnk3
};

enum SkyColorNames
{
    LIGHT_GLOBAL_DIFFUSE = 0,
    LIGHT_GLOBAL_AMBIENT,

    SKY_COLOR_0,
    SKY_COLOR_1,
    SKY_COLOR_2,
    SKY_COLOR_3,
    SKY_COLOR_4,

    FOG_COLOR,

    SKY_UNKNOWN_1,

    SUN_COLOR,
    SUN_HALO_COLOR,

    SKY_UNKNOWN_2,

    CLOUD_COLOR,

    SKY_UNKNOWN_3,

    OCEAN_COLOR_LIGHT,
    OCEAN_COLOR_DARK,

    RIVER_COLOR_LIGHT,
    RIVER_COLOR_DARK,
    //--------------------
    COLORS_PARAMS_COUNT
};

enum FloatBand_FogParams
{
    FOG_DISTANCE = 0,    // Fog distance multiplied by 36 - distance at which everything will be hidden by the fog
    FOG_MULTIPLIER,  // Fog multiplier - fog distance * fog multiplier = fog start distance. 0 - 0, 999...
    CELESTIAL_GLOW,  // Celestial Glow Through - the brightness of the sun and moon as it shows through cloud cover.Note that this effect only appears when the Sun or Moon is being obscured by clouds. 0 - 1
    CLOUD_DESINTY,   // Cloud Density - Controls the density of cloud cover in the area.Value range is 0.0 to 1.0.
    UNK0,
    UNK1,
    //--------------------
    FOGS_PARAMS_COUNT
};

template<typename T>
class SkyParam
{
public:
    SkyParam(uint32 _time, T _param)
    {
        time = _time;
        param = _param;
    }

    uint32 time;
    T param;
};

class SkyParam_Color : public SkyParam<vec3>
{
public:
    SkyParam_Color(uint32 _time, uint32 _color) : SkyParam<vec3>(_time, fromRGB(_color))
    {}
};

class SkyParam_Fog : public SkyParam<float>
{
public:
    SkyParam_Fog(uint32 _time, float _param) : SkyParam<float>(_time, _param)
    {}
};

//

#include "../DBC__Storage.h"

class Sky
{
public:
    Sky();
    Sky(DBC_LightRecord* data);
    void LoadParams(LightParamsNames _param);

    template<typename OUTPUT>
    OUTPUT Intepolate(vector<SkyParam<OUTPUT>>* _array, uint32 _paramNum, uint32 _time) const;

public:
    vec3             m_Position;
    CRange           m_Range;
    float            m_Wight;
    bool             m_IsGlobalSky;

    vector<SkyParam<vec3>>    m_IntBand_Colors[COLORS_PARAMS_COUNT];
    vector<SkyParam<float>>   m_FloatBand_Fogs[FOGS_PARAMS_COUNT];

    // Params
    bool             m_highlightSky;
    __DBC_FOREIGN_KEY_ID(uint32, DBC_LightSkybox, m_lightSkyboxID);
    uint32_t         m_cloudTypeID; // always 0

    float            m_glow;
    float            m_waterShallowAlpha;
    float            m_waterDeepAlpha;
    float            m_oceanShallowAlpha;
    float            m_oceanDeepAlpha;

private:
    const DBC_LightRecord* m_LightRecord;
    const float skymul = 36.0f;
};

template<typename OUTPUT>
inline OUTPUT Sky::Intepolate(vector<SkyParam<OUTPUT>>* _array, uint32 _paramNum, uint32 _time) const
{
    if (_array[_paramNum].empty())
    {
        return OUTPUT();
    }

    OUTPUT c1, c2;
    uint32 t1, t2;
    size_t last = _array[_paramNum].size() - 1;

    if (_time < _array[_paramNum][0].time)
    {
        // interpolate between last and first
        c1 = _array[_paramNum][last].param;
        t1 = _array[_paramNum][last].time;
        c2 = _array[_paramNum][0].param;
        t2 = _array[_paramNum][0].time + (2 * 60 * 24); // next day
        _time += (2 * 60 * 24);
    }
    else
    {
        for (uint32 i = last; i >= 0; i--)
        {
            if (_time >= _array[_paramNum][i].time)
            {
                c1 = _array[_paramNum][i].param;
                t1 = _array[_paramNum][i].time;

                if (i == last) // if current is last, then interpolate with first
                {
                    c2 = _array[_paramNum][0].param;
                    t2 = _array[_paramNum][0].time + (2 * 60 * 24);
                }
                else
                {
                    c2 = _array[_paramNum][i + 1].param;
                    t2 = _array[_paramNum][i + 1].time;
                }
                break;
            }
        }
    }

    float tt = (float)(_time - t1) / (float)(t2 - t1);
    return c1 * (1.0f - tt) + (c2 * tt);
}







