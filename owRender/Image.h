#pragma once

#include "Texture.h"

struct Image
{
	Image()
	{
		texture = nullptr;
		start = VECTOR_ZERO;
		size = VECTOR_ZERO;
		offset = VECTOR_ZERO;

		coords = ZERO_RECT;
	}

	Image(Texture* _texture)
	{
		assert1((_texture != nullptr) && (_texture->GetSize() != VECTOR_ZERO));

		texture = _texture;
		start = VECTOR_ZERO;
		size = _texture->GetSize();
		offset = VECTOR_ZERO;

		CalculateCoords();
	}

	Image(Texture* _texture, cvec2 _textureStart, cvec2 _textureSize, const vec2 _renderOffset = VECTOR_ZERO)
	{
		assert1((_texture != nullptr) && (_texture->GetSize() != VECTOR_ZERO));

		texture = _texture;
		start = _textureStart;
		size = _textureSize;
		offset = _renderOffset;

		CalculateCoords();
	}

	OW_RENDER_DLL_API void CalculateCoords();

	Texture* texture;
	vec2 start;
	vec2 size;
	vec2 offset;

	Rect<double> coords;
};
