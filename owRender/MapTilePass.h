#pragma once

#include "technique.h"

class MapTilePass : public Technique {
public:
	MapTilePass();
	 virtual bool Init();

	 void SetWVP(cmat4 WVP);
	 void SetWorldMatrix(cmat4 WVP);

	 void SetLayersCount(int _layersCount);

	 void SetColorTextureUnit0(int TextureUnit);
	 void SetColorTextureUnit1(int TextureUnit);
	 void SetColorTextureUnit2(int TextureUnit);
	 void SetColorTextureUnit3(int TextureUnit);

	 void SetBlendBuffer(int TextureUnit);

	 void SetShadowMapExists(bool _exists);
	 void SetShadowColor(vec3 _shadowColor);

	 void SetSpecularTextureUnit(int TextureUnit);


	 void SetMCCVExists(bool _exists);
	 void SetMCLVExists(bool _exists);
};