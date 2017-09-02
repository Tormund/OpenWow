#ifndef DS_POINT_LIGHT_PASS_TECH_H
#define	DS_POINT_LIGHT_PASS_TECH_H

#include "ds_light_pass_tech.h"
#include "lights_common.h"

class DSPointLightPassTech : public DSLightPassTech {
public:
	OW_RENDER_DLL_API DSPointLightPassTech();
	OW_RENDER_DLL_API bool Init();

	OW_RENDER_DLL_API void SetPointLight(const PointLight& Lights);
};


#endif