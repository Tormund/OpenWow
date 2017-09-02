#pragma once

#include "technique.h"
#include "lights_common.h"

class DSLightPassTech : public Technique {
public:
	OW_RENDER_DLL_API DSLightPassTech();
	OW_RENDER_DLL_API virtual bool Init();

	OW_RENDER_DLL_API void SetWVP(cmat4 WVP);

	OW_RENDER_DLL_API void SetPositionTextureUnit(unsigned int TextureUnit);
	OW_RENDER_DLL_API void SetColorTextureUnit(unsigned int TextureUnit);
	OW_RENDER_DLL_API void SetNormalTextureUnit(unsigned int TextureUnit);
	OW_RENDER_DLL_API void SetSpecularTextureUnit(unsigned int TextureUnit);

	OW_RENDER_DLL_API void SetEyeWorldPos(const vec3& EyeWorldPos);
	OW_RENDER_DLL_API void SetMatSpecularIntensity(float Intensity);
	OW_RENDER_DLL_API void SetMatSpecularPower(float Power);
	OW_RENDER_DLL_API void SetScreenSize(unsigned int Width, unsigned int Height);
};