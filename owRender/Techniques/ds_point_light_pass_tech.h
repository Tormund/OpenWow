#pragma once

#include "ds_light_pass_tech.h"
#include "../lights_common.h"

class DSPointLightPassTech : public DSLightPassTech {
public:
	 DSPointLightPassTech();
	 bool Init();

	 void SetPointLight(const PointLight& Lights);
};