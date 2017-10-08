#pragma once

#include "technique.h"

class MapChunk_GeometryPass : public Technique {
public:
	MapChunk_GeometryPass();
	 virtual bool Init();

	 void SetLayersCount(int _layersCount);

	 void SetColorTextureUnit0(int TextureUnit);
	 void SetColorTextureUnit1(int TextureUnit);
	 void SetColorTextureUnit2(int TextureUnit);
	 void SetColorTextureUnit3(int TextureUnit);

	 void SetBlendBuffer(int TextureUnit);

	 void SetShadowMapExists(bool _exists);
	 void SetShadowColor(vec3 _shadowColor);

	 void SetSpecularTextureUnit0(int TextureUnit);
	 void SetSpecularTextureUnit1(int TextureUnit);
	 void SetSpecularTextureUnit2(int TextureUnit);
	 void SetSpecularTextureUnit3(int TextureUnit);

	 void SetMCCVExists(bool _exists);
	 void SetMCLVExists(bool _exists);
};