#include "stdafx.h"

// General
#include "Image.h"

void Image::CalculateCoords()
{
	if (texture == nullptr)
	{
		Debug::Error("Can't calculate coord for null texture.");
		return;
	}

	coords.p0 = start / texture->GetSize();
	coords.p1 = coords.p0 + size / texture->GetSize();
}


