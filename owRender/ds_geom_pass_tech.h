#ifndef DS_GEOM_PASS_TECH_H
#define	DS_GEOM_PASS_TECH_H

#include "technique.h"

class DSGeomPassTech : public Technique {
public:
	OW_RENDER_DLL_API DSGeomPassTech();
	OW_RENDER_DLL_API virtual bool Init();

	OW_RENDER_DLL_API void SetWVP(cmat4 WVP);
	OW_RENDER_DLL_API void SetWorldMatrix(cmat4 WVP);
	OW_RENDER_DLL_API void SetColorTextureUnit(int TextureUnit);
	OW_RENDER_DLL_API void SetSpecularTextureUnit(int TextureUnit);
};


#endif