#include "stdafx.h"

#include "SimpleColorTechnique.h"

SimpleColorTechnique::SimpleColorTechnique() : Technique("shaders/SimpleColor.glsl") { }

bool SimpleColorTechnique::Init() {
	if(!CompileProgram("SimpleColorTechnique")) {
		return false;
	}

	return true;
}

void SimpleColorTechnique::SetWVP(cmat4 WVP) {
	setMat4("gWVP", WVP);
}

void SimpleColorTechnique::SetColor(cvec3 _color) {
	setVec3("gColor", _color);
}