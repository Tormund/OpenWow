#pragma once

#include "Texture.h"

class Image
{
public:
    struct CoordsRotation
    {
        enum List
        {
            Coords_ROT_90,
            Coords_ROT_180,
            Coords_ROT_270
        };
    };

public:
    Image() : m_Texture(nullptr), m_Start(VECTOR_ZERO), m_Size(VECTOR_ZERO), m_CoordsCalculated(false)
    {}

    Image(Texture* _texture) : m_Texture(_texture), m_Start(VECTOR_ZERO), m_Size(_texture->GetSize())
    {
        assert1((m_Texture != nullptr) && (m_Size != VECTOR_ZERO));
        CalculateCoords();
    }

    Image(Texture* _texture, cvec2 _size) : m_Texture(_texture), m_Start(VECTOR_ZERO), m_Size(_size)
    {
        assert1((m_Texture != nullptr) && (m_Size != VECTOR_ZERO));
        CalculateCoords();
    }

    Image(Texture* _texture, cvec2 _start, cvec2 _size) : m_Texture(_texture), m_Start(_start), m_Size(_size)
    {
        assert1((m_Texture != nullptr) && (m_Size != VECTOR_ZERO));
        CalculateCoords();
    }

    //

    Texture* GetTexture()
    {
        return m_Texture;
    }

    void SetStart(cvec2 _start)
    {
        m_Start = _start;
        CalculateCoords();
    }

    void SetSize(cvec2 _size)
    {
        m_Size = _size;
        CalculateCoords();
    }

    vec2 GetSize()
    {
        return m_Size;
    }

    vec2 GetP0() { return m_P0; }
    vec2 GetP1() { return m_P1; }
    vec2 GetP2() { return m_P2; }
    vec2 GetP3() { return m_P3; }

    void RotateCoords(CoordsRotation::List _rotation)
    {
        vec2 buf = m_P0;

        switch (_rotation)
        {
            case CoordsRotation::Coords_ROT_90:
            m_P0 = m_P3;
            m_P3 = m_P2;
            m_P2 = m_P1;
            m_P1 = buf;
            break;

            case CoordsRotation::Coords_ROT_180:
            std::swap(m_P0, m_P2);
            std::swap(m_P1, m_P3);
            break;

            case CoordsRotation::Coords_ROT_270:
            m_P0 = m_P1;
            m_P1 = m_P2;
            m_P2 = m_P3;
            m_P3 = buf;
            break;
        }
    }

private:
    void CalculateCoords()
    {
        m_P0 = m_Start / m_Texture->GetSize();
        m_P2 = m_P0 + m_Size / m_Texture->GetSize();

        m_P1 = vec2(m_P2.x, m_P0.y);
        m_P3 = vec2(m_P0.x, m_P2.y);
        

        m_CoordsCalculated = true;
    }

private:
    Texture*  m_Texture;
    vec2      m_Start;
    vec2      m_Size;

    bool      m_CoordsCalculated;
    vec2      m_P0;
    vec2      m_P1;
    vec2      m_P2;
    vec2      m_P3;
};
