#pragma once

#include "ds_light_pass_tech.h"
#include "../lights_common.h"

class DSPointLightPassTech : public DSLightPassTech {
public:
	 //DSPointLightPassTech();

	 void SetPointLight(const PointLight& Light)
	 {
		 setVec3("gPointLight.Base.ambient", Light.ambient);
		 setVec3("gPointLight.Base.diffuse", Light.diffuse);
		 setVec3("gPointLight.Base.specular", Light.specular);

		 setVec3("gPointLight.Position", Light.Position);
		 setFloat("gPointLight.Radius", Light.Radius);

		 setFloat("gPointLight.Atten.Constant", Light.Attenuation.Constant);
		 setFloat("gPointLight.Atten.Linear", Light.Attenuation.Linear);
		 setFloat("gPointLight.Atten.Quadratic", Light.Attenuation.Quadratic);
	 }
};