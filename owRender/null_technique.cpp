#include "stdafx.h"

#include "null_technique.h"

NullTechnique::NullTechnique() : Technique("shaders/null_technique.glsl") { }

bool NullTechnique::Init() {
	if(!CompileProgram("NullTechnique")) {
		return false;
	}

	return true;
}

void NullTechnique::SetWVP(cmat4 WVP) {
	setMat4("gWVP", WVP);
}