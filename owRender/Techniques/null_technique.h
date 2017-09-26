#pragma once

#include "technique.h"

class NullTechnique : public Technique {
public:
	 NullTechnique();
	 virtual bool Init();
};