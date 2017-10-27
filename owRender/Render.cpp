#include "stdafx.h"

// Include
#include "FontsMgr.h"
#include "TexturesMgr.h"

// General
#include "Render.h"

// Additional
#include "RenderStorage.h"
#include "TechniquesManager.h"

bool RenderGL::Init()
{
	if (r == 0)
	{
		r = new RenderDevice();
	}

	if (!r)
	{
		Modules::log().Fatal("CAN'T CREATE RENDER DEVICE!!!.");
		return false;
	}

	if (!r->init())
	{
		Modules::log().Fatal("CAN'T INIT RENDER DEVICE!!!.");
		return false;
	}

	r->setViewport(0, 0, Modules::config().windowSizeX, Modules::config().windowSizeY);

	m_OrhoMatrix = Matrix4f::OrthoMat(0.0f, Modules::config().windowSizeX, Modules::config().windowSizeY, 0.0f, -1.0f, 1.0f);

	_RenderStorage->Init();

	return true;
}

void RenderGL::Destroy()
{}

void RenderGL::Set3D()
{
	// Cull face
	r->setCullMode(R_CullMode::RS_CULL_BACK);

	// Depth settings
	r->setDepthMask(true);
	r->setDepthTest(true);

	// Blending settings
	r->setBlendMode(false);
}

void RenderGL::Set2D()
{
	//wglMakeCurrent(dc, glrc1);

	//-----------

	// Cull face
	r->setCullMode(R_CullMode::RS_CULL_NONE);

	// Depth settings
	r->setDepthTest(false);

	// Blending settings
	r->setBlendMode(true, R_BlendFunc::BS_BLEND_SRC_ALPHA, R_BlendFunc::BS_BLEND_INV_SRC_ALPHA);
}

// UI

void RenderGL::RenderImage(cvec2 _pos, Image* _image)
{
	RenderTexture(_pos - _image->offset, _image->texture, _image->size, _image->coords);
}

void RenderGL::RenderImage(cvec2 _pos, Image* _image, cvec2 _size)
{
	RenderTexture(_pos - _image->offset, _image->texture, _size, _image->coords);
}

//

void RenderGL::RenderTexture(cvec2 _pos, Texture* _texture, cvec2 _size, const Rect& _coords)
{
	RenderTexture(_pos, _texture->GetObj(), _size, _coords);
}

void RenderGL::RenderTexture(cvec2 _pos, uint32 _texture, cvec2 _size, const Rect& _coords)
{
	_TechniquesMgr->m_UI_Texture->BindS();
	_TechniquesMgr->m_UI_Texture->SetProjectionMatrix(m_OrhoMatrix);

	vector<Texture_Vertex> vertices;
	vertices.push_back({vec2(_pos.x + 0.0f,          _pos.y + 0.0f),       vec2(_coords.p0.x, _coords.p0.y)});
	vertices.push_back({vec2(_pos.x + _size.x,       _pos.y + 0.0f),       vec2(_coords.p1.x, _coords.p0.y)});
	vertices.push_back({vec2(_pos.x + 0.0f,          _pos.y + _size.y),     vec2(_coords.p0.x, _coords.p1.y)});
	vertices.push_back({vec2(_pos.x + _size.x,       _pos.y + _size.y),     vec2(_coords.p1.x, _coords.p1.y)});
	
	r->updateBufferData(_RenderStorage->__vb, 0, 4 * sizeof(Texture_Vertex), &vertices[0]);
	r->setTexture(0, _texture, 0, 0);

	r->setGeometry(_RenderStorage->__geom);
	r->drawIndexed(PRIM_TRILIST, 0, 6, 0, 4);

	_TechniquesMgr->m_UI_Texture->Unbind();
}

//

void RenderGL::RenderRectangle(cvec2 _pos, cvec2 _size, const Color& _color)
{
	_TechniquesMgr->m_UI_Color->BindS();
	_TechniquesMgr->m_UI_Color->SetProjectionMatrix(m_OrhoMatrix);
	_TechniquesMgr->m_UI_Color->SetColor(_color);

	vector<vec2> vertices;
	vertices.push_back(vec2(_pos.x + 0.0f,       _pos.y + 0.0f));
	vertices.push_back(vec2(_pos.x + _size.x,    _pos.y + 0.0f));
	vertices.push_back(vec2(_pos.x + 0.0f,       _pos.y + _size.y));
	vertices.push_back(vec2(_pos.x + _size.x,    _pos.y + _size.y));

	r->updateBufferData(_RenderStorage->__vbPos2, 0, vertices.size() * sizeof(vec2), &vertices[0]);
	
	r->setGeometry(_RenderStorage->__geomPos2);
	r->drawIndexed(PRIM_TRILIST, 0, 6, 0, 4);

	_TechniquesMgr->m_UI_Color->Unbind();
}

void RenderGL::RenderRectangleOutline(cvec2 _pos, cvec2 _size, const Color& _color)
{
	/*_TechniquesMgr->m_UI_Color->BindS();
	_TechniquesMgr->m_UI_Color->SetProjectionMatrix(m_OrhoMatrix);
	_TechniquesMgr->m_UI_Color->SetColor(_color);

	vector<vec2> vertices;
	vertices.push_back(vec2(_pos.x + 0.0f, _pos.y + 0.0f));
	vertices.push_back(vec2(_pos.x + _size.x, _pos.y + 0.0f));
	vertices.push_back(vec2(_pos.x + 0.0f, _pos.y + _size.y));
	vertices.push_back(vec2(_pos.x + _size.x, _pos.y + _size.y));

	r->updateBufferData(__vbPos3, 0, vertices.size() * sizeof(vec2), &vertices[0]);
	
	r->setGeometry(__geomPos3);
	r->drawIndexed(PRIM_LINES, 0, 8, 0, 4);

	_TechniquesMgr->m_UI_Color->Unbind();*/
}

//

void RenderGL::RenderText(cvec2 _pos, cstring _string, const Color& _color) const
{
	RenderText(_pos, _string, TextAlignW::TEXT_ALIGNW_LEFT, TextAlignH::TEXT_ALIGNH_BOTTOM, _FontsMgr->GetMainFont(), _color);
}

void RenderGL::RenderText(cvec2 _pos, cstring _string, Font* _font, const Color& _color) const
{
	RenderText(_pos, _string, TextAlignW::TEXT_ALIGNW_LEFT, TextAlignH::TEXT_ALIGNH_BOTTOM, _font, _color);
}

void RenderGL::RenderText(cvec2 _pos, cstring _string, TextAlignW _alignW, TextAlignH _alignH, const Color& _color) const
{
	RenderText(_pos, _string, _alignW, _alignH, _FontsMgr->GetMainFont(), _color);
}

void RenderGL::RenderText(cvec2 _pos, cstring _string, TextAlignW _alignW, TextAlignH _alignH, Font* _font, const Color& _color) const
{
	auto stringWidth = _font->GetStringWidth(_string);
	auto fontHeight = _font->GetHeight();

	vec2 offset = VECTOR_ZERO;

	switch (_alignW)
	{
		case TEXT_ALIGNW_LEFT:
		offset.x = 0;
		break;

		case TEXT_ALIGNW_CENTER:
		offset.x = -static_cast<float>(stringWidth / 2);
		break;

		case TEXT_ALIGNW_RIGHT:
		offset.x = -static_cast<float>(stringWidth);
		break;
	}

	switch (_alignH)
	{
		case TEXT_ALIGNH_TOP:
		offset.y = -static_cast<float>(fontHeight);
		break;

		case TEXT_ALIGNH_CENTER:
		offset.y = -static_cast<float>(fontHeight / 2);
		break;

		case TEXT_ALIGNH_BOTTOM:
		offset.y = 0;
		break;
	}

	_TechniquesMgr->m_UI_Font->BindS();
	_TechniquesMgr->m_UI_Font->SetProjectionMatrix(m_OrhoMatrix);
	_TechniquesMgr->m_UI_Font->SetFontColor(vec3(_color.red, _color.green, _color.blue));

	_font->Render(_string, _pos + offset);

	_TechniquesMgr->m_UI_Font->Unbind();
}

//

void RenderGL::RenderQuad()
{
	vector<vec2> vertices;
	vertices.push_back(vec2(-1.0f, -1.0f));
	vertices.push_back(vec2( 1.0f, -1.0f));
	vertices.push_back(vec2(-1.0f,  1.0f));
	vertices.push_back(vec2( 1.0f,  1.0f));

	r->updateBufferData(_RenderStorage->__vbPos2, 0, vertices.size() * sizeof(vec2), vertices.data());

	r->setGeometry(_RenderStorage->__geomPos2);
	r->drawIndexed(PRIM_TRILIST, 0, 6, 0, 4);
}

//

void RenderGL::OnWindowResized(uint32 _width, uint32 _height)
{
	// Window size
	Modules::config().windowSizeX = _width;
	Modules::config().windowSizeY = _height;

	// Aspect
	Modules::config().CalculateAspectFactor();

	// Set viewport
	r->setViewport(0, 0, Modules::config().windowSizeX, Modules::config().windowSizeY);

	// Projection matix
	m_OrhoMatrix = Matrix4f::OrthoMat(0.0f, Modules::config().windowSizeX, Modules::config().windowSizeY, 0.0f, -1.0f, 1.0f);
}


