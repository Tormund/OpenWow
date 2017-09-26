#include "../stdafx.h"

#include "null_technique.h"

NullTechnique::NullTechnique() : Technique("shaders/null_technique.glsl") { }

bool NullTechnique::Init() {
	if(!CompileProgram("NullTechnique")) {
		return false;
	}

	return true;
}
