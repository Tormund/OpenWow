#pragma once

class Map_Shared
{
public:
    static void CreateMapArrays();

    //

    static vector<uint16> GenarateDefaultMapArray(uint16 _holes = 0);
    static vector<uint16> GenarateLowResMapArray(uint16 _holes = 0);

    static const vec2* GetTextureCoordDetail()
    {
        return &m_TCDetailMap[0];
    }

    static const vec2* GetTextureCoordAlpha()
    {
        return &m_TCAlphaMap[0];
    }  

private:
    static vec2            m_TCDetailMap[C_MapBufferSize];
    static vec2            m_TCAlphaMap[C_MapBufferSize];
    static vector<uint16>  m_DefaultMapStrip;
    static vector<uint16>  m_LowResMapStrip;
};