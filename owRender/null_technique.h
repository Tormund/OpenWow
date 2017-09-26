#pragma once

#include "technique.h"

class NullTechnique : public Technique {
public:
	 NullTechnique();
	 virtual bool Init();

	 void SetWVP(cmat4 WVP);
};