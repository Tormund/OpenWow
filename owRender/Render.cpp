#include "stdafx.h"

// Include
#include "FontsMgr.h"
#include "TexturesMgr.h"

// General
#include "Render.h"

// Additional
#include "TechniquesManager.h"

struct Texture_Vertex
{
	vec2 vertex;
	vec2 textureCoord;
};

struct RenderBackendType
{
	enum List
	{
		OpenGL2 = 2,
		OpenGL4 = 4
	};
};

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

	// GL settings
	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Viewport
	//glViewport(0, 0, Modules::config().windowSizeX, Modules::config().windowSizeY);

	m_OrhoMatrix = Matrix4f::OrthoMat(0.0f, Modules::config().windowSizeX, Modules::config().windowSizeY, 0.0f, -1.0f, 1.0f);


	R_VertexLayoutAttrib attribsPos2[1] = {
		{"VertexPosition",      0, 2, 0}
	};
	__layoutPos2 = _Render->r->registerVertexLayout(1, attribsPos2);

	//

	R_VertexLayoutAttrib attribsModel3[2] = {
		{"vertexPos",      0, 2, 0},
		{"textureCoords",  1, 2, 0}
	};
	__layoutFont = _Render->r->registerVertexLayout(2, attribsModel3);

	//

	R_VertexLayoutAttrib attribsSky[2] = {
		{"vertexPos",      0, 3, 0},
		{"color",          1, 3, 0}
	};
	__layoutSky = _Render->r->registerVertexLayout(2, attribsSky);
	//--------------------------------------------------------------------------------------------
	R_VertexLayoutAttrib attribsMapLowResolution[1] = {
		{"vertexPos",      0, 3, 0}
	};
	__layoutMapLowResolution = _Render->r->registerVertexLayout(1, attribsMapLowResolution);
	//--------------------------------------------------------------------------------------------
	R_VertexLayoutAttrib attribsModel333[6] = {
		{"vertexPos",            0, 3, 0},
		{"textureCoordsDetail",  1, 2, 0},
		{"textureCoordsAlpha",   2, 2, 0},
		{"normal",               3, 3, 0},
	    {"colorMCCV",            4, 3, 0},
	    {"colorMCLV",            5, 4, 0}
	};
	__layoutMapChunk = _Render->r->registerVertexLayout(6, attribsModel333);
	//--------------------------------------------------------------------------------------------
	R_VertexLayoutAttrib attribsWMO[3] = {
		{"vertexPos",      0, 3, 0},
		{"textureCoords",  1, 2, 0},
		{"normal",         2, 3, 0}
	};
	__layoutWMO = _Render->r->registerVertexLayout(3, attribsWMO);

	R_VertexLayoutAttrib attribsWMO_VC[4] = {
		{"vertexPos",      0, 3, 0},
		{"textureCoords",  1, 2, 0},
		{"normal",         2, 3, 0},
		{"color",          3, 4, 0}
	};
	__layoutWMO_VC = _Render->r->registerVertexLayout(4, attribsWMO_VC);
	//--------------------------------------------------------------------------------------------
	R_VertexLayoutAttrib attribsMDX[3] = {
		{"vertexPos",      0, 3, 0},
		{"textureCoords",  1, 2, 0},
		{"normal",         2, 3, 0}
	};
	__layoutMDX = _Render->r->registerVertexLayout(3, attribsMDX);
	//--------------------------------------------------------------------------------------------
	R_VertexLayoutAttrib attribsWater[3] = {
		{"vertexPos",      0, 3, 0},
		{"textureCoords",  1, 3, 0},
		{"normal",         2, 3, 0}
	};
	__layoutWater = _Render->r->registerVertexLayout(3, attribsWater);
	
	//

	// Indexes
	uint16 indexes[6] = {0, 1, 2, 2, 1, 3};
	uint32 __ib = r->createIndexBuffer(6 * sizeof(uint16), indexes);

	//

	//-- Pos2

	__vbPos2 = _Render->r->createVertexBuffer(4 * sizeof(vec2), nullptr);
	__geomPos2 = _Render->r->beginCreatingGeometry(_Render->__layoutPos2);

	r->setGeomVertexParams(__geomPos2, __vbPos2, 0, 0, 0);
	r->setGeomIndexParams(__geomPos2, __ib, R_IndexFormat::IDXFMT_16);

	_Render->r->finishCreatingGeometry(__geomPos2);


	//-- Pos3

	__vbPos3 = _Render->r->createVertexBuffer(4 * sizeof(vec2), nullptr);
	__geomPos3 = _Render->r->beginCreatingGeometry(_Render->__layoutPos2);

	r->setGeomVertexParams(__geomPos3, __vbPos3, 0, 0, 0);

	uint16 indexes3[8] = {0, 1, 1, 2, 2, 3, 3, 0};
	uint32 __ibPos3 = r->createIndexBuffer(8 * sizeof(uint16), indexes3);
	r->setGeomIndexParams(__geomPos3, __ibPos3, R_IndexFormat::IDXFMT_16);

	_Render->r->finishCreatingGeometry(__geomPos3);

	//

	//-- Pos2 Texture2
	
	__vb = _Render->r->createVertexBuffer(4 * sizeof(Texture_Vertex), nullptr);
	__geom = _Render->r->beginCreatingGeometry(_Render->__layoutFont);

	r->setGeomVertexParams(__geom, __vb, 0, 0,            2 * sizeof(vec2));
	r->setGeomVertexParams(__geom, __vb, 1, sizeof(vec2), 2 * sizeof(vec2));
	r->setGeomIndexParams(__geom, __ib, R_IndexFormat::IDXFMT_16);

	_Render->r->finishCreatingGeometry(__geom);


	return true;
}

void RenderGL::Destroy()
{}

void RenderGL::Set3D()
{
	r->clear(0xFFFFFFFF);

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

	r->updateBufferData(__vb, 0, vertices.size() * sizeof(Texture_Vertex), &vertices[0]);

	r->setTexture(0, _texture, 0, 0);
	
	r->setGeometry(__geom);
	r->drawIndexed(PRIM_TRILIST, 0, 6, 0, 6);

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

	r->updateBufferData(__vbPos2, 0, vertices.size() * sizeof(vec2), &vertices[0]);
	
	r->setGeometry(__geomPos2);
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

	r->updateBufferData(__vbPos2, 0, vertices.size() * sizeof(vec2), vertices.data());

	r->setGeometry(__geomPos2);
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


