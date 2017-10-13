#include "../stdafx.h"

#include "UI_Color.h"

UI_Color::UI_Color() : Technique("shaders/UI_Color") {}

bool UI_Color::Init()
{
	if (!CompileProgram("UI_Color"))
	{
		return false;
	}

	return true;
}