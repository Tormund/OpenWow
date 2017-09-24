#pragma once

#include "ds_light_pass_tech.h"
#include "lights_common.h"

class DSDirLightPassTech : public DSLightPassTech {
public:
	 DSDirLightPassTech();
	 virtual bool Init();

	 void SetDirectionalLight(const DirectionalLight& Light);

};