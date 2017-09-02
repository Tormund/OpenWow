#include "stdafx.h"

// Include
#include "quaternion.h"

// General
#include "Wmo_Fog.h"

// Additional
#include "World.h"

void WMOFog::init(File &f) {
	f.ReadBytes(this, __size);
	color = Vec4D(fog.color.r, fog.color.g, fog.color.b, fog.color.a);
	//color = Vec4D(((color1 & 0x00FF0000) >> 16) / 255.0f, ((color1 & 0x0000FF00) >> 8) / 255.0f, ((color1 & 0x000000FF)) / 255.0f, ((color1 & 0xFF000000) >> 24) / 255.0f);
	float temp;
	temp = position.y;
	position.y = position.z;
	position.z = -temp;
	fog.startScalar = fog.startScalar * fog.end;
}

void WMOFog::setup() {
	if(_World->drawfog) {
		glFogfv(GL_FOG_COLOR, color);
		glFogf(GL_FOG_START, fog.startScalar);
		glFogf(GL_FOG_END, fog.end);

		glEnable(GL_FOG);
	}
	else {
		glDisable(GL_FOG);
	}
}