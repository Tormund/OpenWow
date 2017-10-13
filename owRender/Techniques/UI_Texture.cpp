#include "../stdafx.h"

#include "UI_Texture.h"

UI_Texture::UI_Texture() : Technique("shaders/UI_Texture") {}

bool UI_Texture::Init()
{
	if (!CompileProgram("UI_Texture"))
	{
		return false;
	}

	return true;
}