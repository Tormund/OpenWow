#pragma once

#include "ds_light_pass_tech.h"
#include "lights_common.h"

class DSDirLightPassTech : public DSLightPassTech {
public:
	OW_RENDER_DLL_API DSDirLightPassTech();
	OW_RENDER_DLL_API virtual bool Init();

	OW_RENDER_DLL_API void SetDirectionalLight(const DirectionalLight& Light);

};