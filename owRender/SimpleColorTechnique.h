#pragma once

#include "technique.h"

class SimpleColorTechnique : public Technique {
public:
	OW_RENDER_DLL_API SimpleColorTechnique();
	OW_RENDER_DLL_API virtual bool Init();

	OW_RENDER_DLL_API void SetWVP(cmat4 WVP);
	OW_RENDER_DLL_API void SetColor(cvec3 _color);
};