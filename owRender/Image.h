#pragma once

#include "Texture.h"

class Image
{
public:
	Image() : m_Texture(nullptr), m_Start(VECTOR_ZERO), m_Size(VECTOR_ZERO), m_CoordsCalculated(false), m_Coords(RECT_ZERO)
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

    Rect& GetCoords()
    {
        assert1(m_CoordsCalculated);
        return m_Coords;
    }

private:
    void CalculateCoords()
    {
        m_Coords.p0 = m_Start / m_Texture->GetSize();
        m_Coords.p1 = m_Coords.p0 + m_Size / m_Texture->GetSize();
        m_CoordsCalculated = true;
    }

private:
	Texture*  m_Texture;
	vec2      m_Start;
	vec2      m_Size;

    bool      m_CoordsCalculated;
	Rect      m_Coords;
};
