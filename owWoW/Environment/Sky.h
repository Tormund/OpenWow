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

enum LightColors
{
    LIGHT_COLOR_GLOBAL_DIFFUSE = 0,
    LIGHT_COLOR_GLOBAL_AMBIENT,

    LIGHT_COLOR_SKY_0,
    LIGHT_COLOR_SKY_1,
    LIGHT_COLOR_SKY_2,
    LIGHT_COLOR_SKY_3,
    LIGHT_COLOR_SKY_4,

    LIGHT_COLOR_FOG,
    LIGHT_COLOR_UNK0,
    LIGHT_COLOR_SUN,
    LIGHT_COLOR_SUN_HALO,
    LIGHT_COLOR_UNK1,
    LIGHT_COLOR_CLOUD,
    LIGHT_COLOR_UNK2,

    LIGHT_COLOR_OCEAN_LIGHT,
    LIGHT_COLOR_OCEAN_DARK,

    LIGHT_COLOR_RIVER_LIGHT,
    LIGHT_COLOR_RIVER_DARK,
    //--------------------
    LIGHT_COLORS_COUNT
};

enum LightFogs
{
    LIGHT_FOG_DISTANCE = 0,    // Fog distance multiplied by 36 - distance at which everything will be hidden by the fog
    LIGHT_FOG_MULTIPLIER,      // Fog multiplier - fog distance * fog multiplier = fog start distance. 0 - 0, 999...
    LIGHT_FOG_CELESTIAL_GLOW,  // Celestial Glow Through - the brightness of the sun and moon as it shows through cloud cover.Note that this effect only appears when the Sun or Moon is being obscured by clouds. 0 - 1
    LIGHT_FOG_CLOUD_DESINTY,   // Cloud Density - Controls the density of cloud cover in the area.Value range is 0.0 to 1.0.
    LIGHT_FOG_UNK0,
    LIGHT_FOG_UNK1,
    //--------------------
    LIGHT_FOGS_COUNT
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

class SkyParams
{
public:
    void Clear()
    {
        for (uint8 i = 0; i < LIGHT_COLORS_COUNT; i++)
        {
            m_InterpolatedColors[i] = vec3();
        }
        for (uint8 i = 0; i < LIGHT_FOGS_COUNT; i++)
        {
            m_InterpolatedFogs[i] = 0.0f;
        }

        m_glow = 0.0f;
        m_waterShallowAlpha = 0.0f;
        m_waterDeepAlpha = 0.0f;
        m_oceanShallowAlpha = 0.0f;
        m_oceanDeepAlpha = 0.0f;
    }

    SkyParams& operator+=(const SkyParams& _s)
    {
        for (uint8 i = 0; i < LIGHT_COLORS_COUNT; i++)
            this->m_InterpolatedColors[i] += _s.m_InterpolatedColors[i];

        for (uint8 i = 0; i < LIGHT_FOGS_COUNT; i++)
            this->m_InterpolatedFogs[i] += _s.m_InterpolatedFogs[i];

        this->m_glow += _s.m_glow;
        this->m_oceanShallowAlpha += _s.m_oceanShallowAlpha;
        this->m_oceanDeepAlpha += _s.m_oceanDeepAlpha;
        this->m_waterShallowAlpha += _s.m_waterShallowAlpha;
        this->m_waterDeepAlpha += _s.m_waterDeepAlpha;

        return *this;
    }

    SkyParams& operator*=(float _weight)
    {
        for (uint8 i = 0; i < LIGHT_COLORS_COUNT; i++)
            this->m_InterpolatedColors[i] *= _weight;

        for (uint8 i = 0; i < LIGHT_FOGS_COUNT; i++)
            this->m_InterpolatedFogs[i] *= _weight;

        this->m_glow *= _weight;
        this->m_oceanShallowAlpha *= _weight;
        this->m_oceanDeepAlpha *= _weight;
        this->m_waterShallowAlpha *= _weight;
        this->m_waterDeepAlpha *= _weight;

        return *this;
    }

public: // Params
    vec3                      m_InterpolatedColors[LIGHT_COLORS_COUNT];
    float                     m_InterpolatedFogs[LIGHT_FOGS_COUNT];
    float                     m_glow;
    float                     m_waterShallowAlpha;
    float                     m_waterDeepAlpha;
    float                     m_oceanShallowAlpha;
    float                     m_oceanDeepAlpha;
};


#include "../DBC__Storage.h"

class Sky : public SkyParams
{
public:
    Sky();
    Sky(DBC_LightRecord* data);
    void LoadParams(LightParamsNames _param);

    SkyParams& GetByTime(uint32 _time);

private:
    template<typename T>
    T GetByTimeTemplate(vector<SkyParam<T>>* _array, uint32 _paramNum, uint32 _time) const;

public:
    vec3             m_Position;
    CRange           m_Range;
    float            m_Wight;
    bool             m_IsGlobalSky;
    bool             m_highlightSky;
    __DBC_FOREIGN_KEY_ID(uint32, DBC_LightSkybox, m_lightSkyboxID);
    uint32_t         m_cloudTypeID; // always 0

private:
    vector<SkyParam<vec3>>    m_IntBand_Colors[LIGHT_COLORS_COUNT];
    vector<SkyParam<float>>   m_FloatBand_Fogs[LIGHT_FOGS_COUNT];

private:
    const DBC_LightRecord* m_LightRecord;
    const float skymul = 36.0f;
};

#include "Sky.inl"




