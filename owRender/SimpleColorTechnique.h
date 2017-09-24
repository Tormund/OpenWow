#pragma once

#include "technique.h"

class SimpleColorTechnique : public Technique {
public:
	 SimpleColorTechnique();
	 virtual bool Init();

	 void SetWVP(cmat4 WVP);
	 void SetColor(cvec3 _color);
};