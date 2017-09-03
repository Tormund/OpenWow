#include "stdafx.h"

// Includes

// General
#include "Wmo_Light.h"

// Additional
#include "WowTypes.h"

struct __WMOLight {
	enum LightType {
		OMNI_LGT = 0,
		SPOT_LGT = 1,
		DIRECT_LGT = 2,
		AMBIENT_LGT = 3,
	};
	uint8_t type;
	uint8_t useAtten;
	uint8_t pad[2];
	CImVector color;
	C3Vector position;
	float intensity;
	float attenStart;
	float attenEnd;
	float unk[4];
};

void WMOLight::init(File &f) {
	f.ReadBytes(&LightType, 1);
	f.ReadBytes(&type, 1);
	f.ReadBytes(&useAtten, 1);
	f.ReadBytes(&pad, 1);
	f.ReadBytes(&color, 4);
	f.ReadBytes(glm::value_ptr(pos), 12); // BOUZI FIX ME
	f.ReadBytes(&intensity, 4);
	f.ReadBytes(&attenStart, 4);
	f.ReadBytes(&attenEnd, 4);
	f.ReadBytes(unk, 4 * 4);

	pos = vec3(pos.x, pos.z, -pos.y);

	fcolor = fromARGB(color);
	fcolor *= intensity;
	fcolor.w = 1.0f;

	/*
	// light logging
	Debug::Info("Light %08x @ (%4.2f,%4.2f,%4.2f)\t %4.2f, %4.2f, %4.2f, %4.2f, %4.2f, %4.2f, %4.2f\t(%d,%d,%d,%d)",
	color, pos.x, pos.y, pos.z, intensity,
	unk[0], unk[1], unk[2], unk[3], unk[4], r,
	type[0], type[1], type[2], type[3]);
	*/
}

void WMOLight::setup(GLint light) { // not used right now
	GLfloat LightAmbient[] = {0, 0, 0, 1.0f};
	GLfloat LightPosition[] = {pos.x, pos.y, pos.z, 0.0f};

	glLightfv(light, GL_AMBIENT, LightAmbient);
	glLightfv(light, GL_DIFFUSE, glm::value_ptr(fcolor));
	glLightfv(light, GL_POSITION, LightPosition);

	glEnable(light);
}

void WMOLight::setupOnce(GLint light, vec3 dir, vec4 lcol) {
	vec4 position(dir, 0);
	//vec4 position(0,1,0,0);

	vec4 ambient = lcol * 0.3f; //vec4(lcol * 0.3f, 1);
							   //vec4 ambient = vec4(0.101961f, 0.062776f, 0, 1);
	vec4 diffuse = lcol; //vec4(lcol, 1);
						  //vec4 diffuse = vec4(0.439216f, 0.266667f, 0, 1);

	glLightfv(light, GL_AMBIENT, glm::value_ptr(ambient));
	glLightfv(light, GL_DIFFUSE, glm::value_ptr(diffuse));
	glLightfv(light, GL_POSITION, glm::value_ptr(position));

	glEnable(light);
}