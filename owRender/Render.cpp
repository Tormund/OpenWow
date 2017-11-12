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
		Log::Fatal("CAN'T CREATE RENDER DEVICE!!!.");
		return false;
	}

	if (!r->init())
	{
		Log::Fatal("CAN'T INIT RENDER DEVICE!!!.");
		return false;
	}

	r->setViewport(0, 0, _Config.windowSizeX, _Config.windowSizeY);

	m_OrhoMatrix = Matrix4f::OrthoMat(0.0f, _Config.windowSizeX, _Config.windowSizeY, 0.0f, -1.0f, 1.0f);

	_RenderStorage->Init();

    rb = r->createRenderBuffer(_Config.windowSizeX, _Config.windowSizeY, R_TextureFormats::RGBA16F, true, 4, 0);


    // Main game camera
    mainCamera = new Camera;
    mainCamera->setupViewParams(45.0f, _Config.aspectRatio, 2.0f, 15000.0f);
    _PipelineGlobal->SetCamera(mainCamera);

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
	// Cull face
	r->setCullMode(R_CullMode::RS_CULL_NONE);

	// Depth settings
	r->setDepthTest(false);

	// Blending settings
	r->setBlendMode(true, R_BlendFunc::BS_BLEND_SRC_ALPHA, R_BlendFunc::BS_BLEND_INV_SRC_ALPHA);
}

// UI

void RenderGL::RenderImage(vec2 _pos, Image* _image)
{
    RenderImage(_pos, _image, _image->GetSize());
}

void RenderGL::RenderImage(vec2 _pos, Image* _image, vec2 _size)
{
    // Transform
    _Pipeline->Clear();
    _Pipeline->Translate(_pos.x + _size.x / 2.0f, _pos.y + _size.y / 2.0f, 0.0f);
    _Pipeline->Scale(_size.x / 2.0f, _size.y / 2.0f, 0.0f);

    // Update buffer
    vector<vec2> texCoordsQuad;
    texCoordsQuad.push_back(_image->GetP0());
    texCoordsQuad.push_back(_image->GetP1());
    texCoordsQuad.push_back(_image->GetP3());
    texCoordsQuad.push_back(_image->GetP2());
    _Render->r->updateBufferData(_RenderStorage->__vbQuadVTDynamic, 4 * sizeof(vec2), 4 * sizeof(vec2), texCoordsQuad.data());

    // Shader
    _TechniquesMgr->m_UI_Texture->BindS();
    _TechniquesMgr->m_UI_Texture->SetProjectionMatrix(m_OrhoMatrix * _Pipeline->GetWorld());

    // State
    r->setTexture(10, _image->GetTexture()->GetObj(), SS_FILTER_BILINEAR | SS_ANISO16 | SS_ADDR_CLAMP, 0);
    r->setGeometry(_RenderStorage->__QuadVTDynamic);
    r->drawIndexed(PRIM_TRILIST, 0, 6, 0, 4);

    _TechniquesMgr->m_UI_Texture->Unbind();
}

//

void RenderGL::RenderTexture(vec2 _pos, Texture* _texture)
{
    RenderTexture(_pos, _texture->GetObj(), _texture->GetSize());
}

void RenderGL::RenderTexture(vec2 _pos, Texture* _texture, vec2 _size)
{
	RenderTexture(_pos, _texture->GetObj(), _size);
}

void RenderGL::RenderTexture(vec2 _pos, uint32 _texture, vec2 _size)
{
	// Transform
	_Pipeline->Clear();
	_Pipeline->Translate(_pos.x + _size.x / 2.0f, _pos.y + _size.y / 2.0f, 0.0f);
	_Pipeline->Scale(_size.x / 2.0f, _size.y / 2.0f, 0.0f);

	// Shader
	_TechniquesMgr->m_UI_Texture->BindS();
	_TechniquesMgr->m_UI_Texture->SetProjectionMatrix(m_OrhoMatrix * _Pipeline->GetWorld());

	// State
	r->setTexture(10, _texture, SS_FILTER_BILINEAR | SS_ANISO16 | SS_ADDR_CLAMP, 0);
	r->setGeometry(_RenderStorage->__QuadVT);
	r->drawIndexed(PRIM_TRILIST, 0, 6, 0, 4);

	_TechniquesMgr->m_UI_Texture->Unbind();
}

//

void RenderGL::RenderRectangle(vec2 _pos, vec2 _size, const Color& _color)
{
	// Transform
	_Pipeline->Clear();
	_Pipeline->Translate(_pos.x + _size.x / 2.0f, _pos.y + _size.y / 2.0f, 0.0f);
	_Pipeline->Scale(_size.x / 2.0f, _size.y / 2.0f, 0.0f);

	// Shader
	_TechniquesMgr->m_UI_Color->BindS();
	_TechniquesMgr->m_UI_Color->SetProjectionMatrix(m_OrhoMatrix * _Pipeline->GetWorld());
	_TechniquesMgr->m_UI_Color->SetColor(_color);
	
	r->setGeometry(_RenderStorage->__Quad);
	r->drawIndexed(PRIM_TRILIST, 0, 6, 0, 4);

	_TechniquesMgr->m_UI_Color->Unbind();
}

void RenderGL::RenderRectangleOutline(vec2 _pos, vec2 _size, const Color& _color)
{
	// Transform
	/*_Pipeline->Clear();
	_Pipeline->Translate(_pos.x + _size.x / 2.0f, _pos.y + _size.y / 2.0f, 0.0f);
	_Pipeline->Scale(_size.x / 2.0f, _size.y / 2.0f, 0.0f);

	// Shader
	_TechniquesMgr->m_UI_Color->BindS();
	_TechniquesMgr->m_UI_Color->SetProjectionMatrix(m_OrhoMatrix * _Pipeline->GetWorld());
	_TechniquesMgr->m_UI_Color->SetColor(_color);

	r->setFillMode(R_FillMode::RS_FILL_WIREFRAME);

	r->setGeometry(_RenderStorage->__Quad);
	r->drawIndexed(PRIM_TRILIST, 0, 6, 0, 4);

	r->setFillMode(R_FillMode::RS_FILL_SOLID);

	_TechniquesMgr->m_UI_Color->Unbind();*/



	r->setFillMode(R_FillMode::RS_FILL_WIREFRAME);
	RenderRectangle(_pos, _size, _color);
	r->setFillMode(R_FillMode::RS_FILL_SOLID);
}

//

void RenderGL::RenderText(vec2 _pos, cstring _string, const Color& _color) const
{
	RenderText(_pos, _string, TextAlignW::TEXT_ALIGNW_LEFT, TextAlignH::TEXT_ALIGNH_BOTTOM, _FontsMgr->GetMainFont(), _color);
}

void RenderGL::RenderText(vec2 _pos, cstring _string, Font* _font, const Color& _color) const
{
	RenderText(_pos, _string, TextAlignW::TEXT_ALIGNW_LEFT, TextAlignH::TEXT_ALIGNH_BOTTOM, _font, _color);
}

void RenderGL::RenderText(vec2 _pos, cstring _string, TextAlignW _alignW, TextAlignH _alignH, const Color& _color) const
{
	RenderText(_pos, _string, _alignW, _alignH, _FontsMgr->GetMainFont(), _color);
}

void RenderGL::RenderText(vec2 _pos, cstring _string, TextAlignW _alignW, TextAlignH _alignH, Font* _font, const Color& _color) const
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
	r->setGeometry(_RenderStorage->__Quad);

	r->drawIndexed(PRIM_TRILIST, 0, 6, 0, 4);
}

//

void RenderGL::OnWindowResized(uint32 _width, uint32 _height)
{
	// Window size
	_Config.windowSizeX = _width;
	_Config.windowSizeY = _height;

	// Aspect
	_Config.CalculateAspectFactor();

	// Set viewport
	r->setViewport(0, 0, _Config.windowSizeX, _Config.windowSizeY);

	// Projection matix
	m_OrhoMatrix = Matrix4f::OrthoMat(0.0f, _Config.windowSizeX, _Config.windowSizeY, 0.0f, -1.0f, 1.0f);
}


