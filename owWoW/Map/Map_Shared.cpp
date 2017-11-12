#include "../stdafx.h"

// General
#include "Map_Shared.h"

// Static
vec2            Map_Shared::m_TCDetailMap[C_MapBufferSize];
vec2            Map_Shared::m_TCAlphaMap[C_MapBufferSize];
vector<uint16>  Map_Shared::m_DefaultMapStrip;
vector<uint16>  Map_Shared::m_LowResMapStrip;

bool isHole(uint16 holes, uint16 i, uint16 j)
{
    const uint16 holetab_h[4] = {0x1111, 0x2222, 0x4444, 0x8888};
    const uint16 holetab_v[4] = {0x000F, 0x00F0, 0x0F00, 0xF000};

    return (holes & holetab_h[i] & holetab_v[j]) != 0;
}

void Map_Shared::CreateMapArrays()
{
    m_DefaultMapStrip = GenarateDefaultMapArray();
    m_LowResMapStrip = GenarateLowResMapArray();

    // init texture coordinates for detail map:
    vec2* detailTextureCoord = m_TCDetailMap;
    const float detail_half = 0.5f * C_DetailSize / 8.0f;
    for (int j = 0; j < 17; j++)
    {
        for (int i = 0; i < ((j % 2) ? 8 : 9); i++)
        {
            float tx = C_DetailSize / 8.0f * i;
            float ty = C_DetailSize / 8.0f * j * 0.5f;
            if (j % 2)
            {
                tx += detail_half;
            }
            *detailTextureCoord++ = vec2(tx, ty);
        }
    }

    // init texture coordinates for alpha map:
    vec2* alphaTextureCoord = m_TCAlphaMap;
    const float alpha_half = 0.5f * 1.0f / 8.0f;
    for (int j = 0; j < 17; j++)
    {
        for (int i = 0; i < ((j % 2) ? 8 : 9); i++)
        {
            float tx = 1.0f / 8.0f * i;
            float ty = 1.0f / 8.0f * j * 0.5f;
            if (j % 2)
            {
                tx += alpha_half;
            }
            *alphaTextureCoord++ = vec2(tx, ty);
        }
    }
}

vector<uint16> Map_Shared::GenarateDefaultMapArray(uint16 _holes)
{
    if (_holes == 0 && !m_DefaultMapStrip.empty())
    {
        return m_DefaultMapStrip;
    }

    int16 outerArray[9][9];
    for (uint16 i = 0; i < 9; i++)
        for (uint16 j = 0; j < 9; j++)
            outerArray[i][j] = (i * 17) + j;

    int16 innerArray[8][8];
    for (uint16 i = 0; i < 8; i++)
        for (uint16 j = 0; j < 8; j++)
            innerArray[i][j] = 9 + (i * 17) + j;

    vector<uint16> myIndexes;
    for (uint16 i = 0; i < 8; i++)
    {
        for (uint16 j = 0; j < 8; j++)
        {
            if (isHole(_holes, j / 2, i / 2))
            {
                continue;
            }

            myIndexes.push_back(outerArray[i][j]);
            myIndexes.push_back(outerArray[i][j + 1]);
            myIndexes.push_back(innerArray[i][j]);

            myIndexes.push_back(outerArray[i][j + 1]);
            myIndexes.push_back(outerArray[i + 1][j + 1]);
            myIndexes.push_back(innerArray[i][j]);

            myIndexes.push_back(outerArray[i + 1][j + 1]);
            myIndexes.push_back(outerArray[i + 1][j]);
            myIndexes.push_back(innerArray[i][j]);

            myIndexes.push_back(outerArray[i + 1][j]);
            myIndexes.push_back(outerArray[i][j]);
            myIndexes.push_back(innerArray[i][j]);
        }
    }

    return myIndexes;
}


vector<uint16> Map_Shared::GenarateLowResMapArray(uint16 _holes)
{
    if (_holes == 0 && !m_LowResMapStrip.empty())
    {
        return m_LowResMapStrip;
    }

    int16 outerArray[9][9];
    for (uint16 i = 0; i < 9; i++)
        for (uint16 j = 0; j < 9; j++)
            outerArray[i][j] = (i * 17) + j;

    vector<uint16> myIndexes;
    for (uint16 i = 0; i < 8; i++)
    {
        for (uint16 j = 0; j < 8; j++)
        {
            if (isHole(_holes, j / 2, i / 2))
            {
                continue;
            }

            myIndexes.push_back(outerArray[i][j]);
            myIndexes.push_back(outerArray[i][j + 1]);
            myIndexes.push_back(outerArray[i + 1][j]);

            myIndexes.push_back(outerArray[i + 1][j]);
            myIndexes.push_back(outerArray[i][j + 1]);
            myIndexes.push_back(outerArray[i + 1][j + 1]);
        }
    }

    return myIndexes;
}