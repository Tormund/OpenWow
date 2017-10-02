#include "../stdafx.h"

#include "UI_Font.h"

UI_Font::UI_Font() : Technique("shaders/UI_Font.glsl") {}

bool UI_Font::Init()
{
	if (!CompileProgram("UI_Font"))
	{
		return false;
	}

	return true;
}