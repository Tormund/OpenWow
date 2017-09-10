#include "stdafx.h"

// General
#include "Wmo_Material.h"

// Additional
#include "world.h"

WMOMaterial::WMOMaterial(File& _file)
{
	data = new WMOMaterial::__WMOMaterial;
	_file.ReadBytes(data, sizeof(WMOMaterial::__WMOMaterial));
}

WMOMaterial::~WMOMaterial()
{
	_TexturesMgr->Delete(texture);
	delete data;
}

void WMOMaterial::initTexture(const char* _texturesFileNames)
{
	string texpath(_texturesFileNames + data->diffuseNameIndex);

	texture = _TexturesMgr->Add(texpath);
}

void WMOMaterial::setup()
{
	//SetBlendMode();
	if (IsTwoSided())
		glDisable(GL_CULL_FACE);
	else
		glEnable(GL_CULL_FACE);

	texture->Bind();
}

void GLSetBlend(bool _enable, GLenum _srcColor, GLenum _descColor, GLenum _srcAlpha, GLenum _destAlpha)
{
	if (_enable)
		glEnable(GL_BLEND);
	else
		glDisable(GL_BLEND);

	glBlendFunc(GL_SRC_COLOR, _srcColor);
	glBlendFunc(GL_DST_COLOR, _descColor);

	glBlendFunc(GL_SRC_ALPHA, _srcAlpha);
	glBlendFunc(GL_DST_ALPHA, _destAlpha);
}

void WMOMaterial::SetBlendMode()
{
	switch (data->blendMode)
	{
		case 0:
		//GxBlend_Opaque	
		GLSetBlend(false, GL_ONE, GL_ZERO, GL_ONE, GL_ZERO);
		break;

		case 1:
		//GxBlend_AlphaKey	
		GLSetBlend(false, GL_ONE, GL_ZERO, GL_ONE, GL_ZERO);
		break;

		case 2:
		//GxBlend_Alpha	
		GLSetBlend(true, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		break;

		case 3:
		//GxBlend_Add	
		GLSetBlend(true, GL_SRC_ALPHA, GL_ONE, GL_ZERO, GL_ONE);
		break;

		case 4:
		//GxBlend_Mod	
		GLSetBlend(true, GL_DST_COLOR, GL_ZERO, GL_DST_ALPHA, GL_ZERO);
		break;

		case 5:
		//GxBlend_Mod2x	
		GLSetBlend(true, GL_DST_COLOR, GL_SRC_COLOR, GL_DST_ALPHA, GL_SRC_ALPHA);
		break;

		case 6:
		//GxBlend_ModAdd	
		GLSetBlend(true, GL_DST_COLOR, GL_ONE, GL_DST_ALPHA, GL_ONE);
		break;

		case 7:
		//GxBlend_InvSrcAlphaAdd	
		GLSetBlend(true, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA, GL_ONE);
		break;

		case 8:
		//GxBlend_InvSrcAlphaOpaque	
		GLSetBlend(true, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO);
		break;

		case 9:
		//GxBlend_SrcAlphaOpaque	
		GLSetBlend(true, GL_SRC_ALPHA, GL_ZERO, GL_SRC_ALPHA, GL_ZERO);
		break;

		case 10:
		//GxBlend_NoAlphaAdd	
		GLSetBlend(true, GL_ONE, GL_ONE, GL_ZERO, GL_ONE);
		break;

		case 11:
		//GxBlend_ConstantAlpha	
		GLSetBlend(true, GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA, GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);
		break;

		case 12:
		//GxBlend_Screen	
		GLSetBlend(true, GL_ONE_MINUS_DST_COLOR, GL_ONE, GL_ONE, GL_ZERO);
		break;

		case 13:
		//GxBlend_BlendAdd	
		GLSetBlend(true, GL_ONE, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		break;
	}
}
