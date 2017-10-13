#include "stdafx.h"

// Include
#include "FontsMgr.h"
#include "TexturesMgr.h"

// General
#include "Render.h"

// Additional
#include "TechniquesManager.h"

void glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	Modules::log().Error("---------------");
	Modules::log().Error("OpenGL Debug message (%d): [%s]", id, message);

	switch (source)
	{
		case GL_DEBUG_SOURCE_API:             Modules::log().Error("Source: OpenGL API"); break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   Modules::log().Error("Source: Window System API"); break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: Modules::log().Error("Source: Shader Compiler"); break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:     Modules::log().Error("Source: Third Party"); break;
		case GL_DEBUG_SOURCE_APPLICATION:     Modules::log().Error("Source: Application"); break;
		case GL_DEBUG_SOURCE_OTHER:           Modules::log().Error("Source: Other"); break;
	}

	switch (type)
	{
		case GL_DEBUG_TYPE_ERROR:               Modules::log().Error("Type: Error"); break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: Modules::log().Error("Type: Deprecated Behaviour"); break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  Modules::log().Error("Type: Undefined Behaviour"); break;
		case GL_DEBUG_TYPE_PORTABILITY:         Modules::log().Error("Type: Portability"); break;
		case GL_DEBUG_TYPE_PERFORMANCE:         Modules::log().Error("Type: Performance"); break;
		case GL_DEBUG_TYPE_MARKER:              Modules::log().Error("Type: Marker"); break;
		case GL_DEBUG_TYPE_PUSH_GROUP:          Modules::log().Error("Type: Push Group"); break;
		case GL_DEBUG_TYPE_POP_GROUP:           Modules::log().Error("Type: Pop Group"); break;
		case GL_DEBUG_TYPE_OTHER:               Modules::log().Error("Type: Other"); break;
	}

	switch (severity)
	{
		case GL_DEBUG_SEVERITY_HIGH:         Modules::log().Error("Severity: high"); break;
		case GL_DEBUG_SEVERITY_MEDIUM:       Modules::log().Error("Severity: medium"); break;
		case GL_DEBUG_SEVERITY_LOW:          Modules::log().Error("Severity: low"); break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: Modules::log().Error("Severity: notification"); break;
	}


	//system("pause");
	//Modules::log().Exit(-1);
}

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


	// EngineLog output
	/*GLint flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{*/
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(glDebugOutput, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	//fail1();
	//}


	/*dc = wglGetCurrentDC();
	glrc1 = wglCreateContext(dc);
	glrc2 = wglCreateContext(dc);
	glrc3 = wglCreateContext(dc);
	glrc4 = wglCreateContext(dc);

	if (wglShareLists(glrc1, glrc2) == FALSE)
	{
		Modules::log().Error("Mega error !!!!1111");
	}

	if (wglShareLists(glrc1, glrc3) == FALSE)
	{
		Modules::log().Error("Mega error !!!!2222");
	}

	if (wglShareLists(glrc1, glrc4) == FALSE)
	{
		Modules::log().Error("Mega error !!!!3333");
	}

	wglMakeCurrent(dc, glrc1);*/

	// GL settings
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Viewport
	glViewport(0, 0, Modules::config().windowSizeX, Modules::config().windowSizeY);

	m_OrhoMatrix = Matrix4f::OrthoMat(0.0f, Modules::config().windowSizeX, Modules::config().windowSizeY, 0.0f, -1.0f, 1.0f);


	// Font vertex buffer
	glGenBuffers(1, &m_ImageBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_ImageBuffer);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Texture_Vertex), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &m_ColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_ColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(vec2), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return true;
}

void RenderGL::Destroy()
{}

void RenderGL::Set3D()
{
	//wglMakeCurrent(dc, glrc1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Cull face
	glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Depth settings
	glDisable(GL_DEPTH_TEST);

	// Blending settings
	glDisable(GL_BLEND);
}

void RenderGL::Set2D()
{
	//wglMakeCurrent(dc, glrc1);

	//-----------

	// Cull face
	glDisable(GL_CULL_FACE);

	// Depth settings
	glDisable(GL_DEPTH_TEST);

	// Blending settings
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glActiveTexture(GL_TEXTURE0);
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

void RenderGL::RenderTexture(cvec2 _pos, GLuint _texture, cvec2 _size, const Rect& _coords)
{
	_TechniquesMgr->m_UI_Texture->Bind();
	_TechniquesMgr->m_UI_Texture->SetProjectionMatrix(m_OrhoMatrix);

	glBindBuffer(GL_ARRAY_BUFFER, m_ImageBuffer);

	vector<Texture_Vertex> vertices;

	vertices.push_back({vec2(_pos.x + 0.0f,          _pos.y + 0.0f),       vec2(_coords.p0.x, _coords.p0.y)});
	vertices.push_back({vec2(_pos.x + _size.x,       _pos.y + 0.0f),       vec2(_coords.p1.x, _coords.p0.y)});
	vertices.push_back({vec2(_pos.x + 0.0f,          _pos.y + _size.y),     vec2(_coords.p0.x, _coords.p1.y)});

	vertices.push_back({vec2(_pos.x + 0.0f,          _pos.y + _size.y),     vec2(_coords.p0.x, _coords.p1.y)});
	vertices.push_back({vec2(_pos.x + _size.x,       _pos.y + 0.0f),       vec2(_coords.p1.x, _coords.p0.y)});
	vertices.push_back({vec2(_pos.x + _size.x,       _pos.y + _size.y),     vec2(_coords.p1.x, _coords.p1.y)});

	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Texture_Vertex), vertices.data());

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const GLvoid*)(0));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const GLvoid*)(sizeof(vec2)));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texture);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindTexture(GL_TEXTURE_2D, 0);

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	_TechniquesMgr->m_UI_Texture->Unbind();
}

//

void RenderGL::RenderRectangle(cvec2 _pos, cvec2 _size, const Color& _color)
{
	_TechniquesMgr->m_UI_Color->Bind();
	_TechniquesMgr->m_UI_Color->SetProjectionMatrix(m_OrhoMatrix);
	_TechniquesMgr->m_UI_Color->SetColor(_color);

	glBindBuffer(GL_ARRAY_BUFFER, m_ImageBuffer);

	vector<vec2> vertices;

	vertices.push_back(vec2(_pos.x + 0.0f, _pos.y + 0.0f));
	vertices.push_back(vec2(_pos.x + _size.x, _pos.y + 0.0f));
	vertices.push_back(vec2(_pos.x + 0.0f, _pos.y + _size.y));

	vertices.push_back(vec2(_pos.x + 0.0f, _pos.y + _size.y));
	vertices.push_back(vec2(_pos.x + _size.x, _pos.y + 0.0f));
	vertices.push_back(vec2(_pos.x + _size.x, _pos.y + _size.y));

	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(vec2), vertices.data());

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), (const GLvoid*)(0));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	_TechniquesMgr->m_UI_Color->Unbind();
}

void RenderGL::RenderRectangleOutline(cvec2 _pos, cvec2 _size, const Color& _color)
{
	_TechniquesMgr->m_UI_Color->Bind();
	_TechniquesMgr->m_UI_Color->SetProjectionMatrix(m_OrhoMatrix);
	_TechniquesMgr->m_UI_Color->SetColor(_color);

	glBindBuffer(GL_ARRAY_BUFFER, m_ImageBuffer);

	vector<vec2> vertices;

	vertices.push_back(vec2(_pos.x + 0.0f,          _pos.y + 0.0f));
	vertices.push_back(vec2(_pos.x + _size.x,       _pos.y + 0.0f));
	vertices.push_back(vec2(_pos.x + _size.x,       _pos.y + _size.y));
	vertices.push_back(vec2(_pos.x + 0.0f,          _pos.y + _size.y));

	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(vec2), vertices.data());

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), (const GLvoid*)(0));

	glDrawArrays(GL_LINE_LOOP, 0, 4);

	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	_TechniquesMgr->m_UI_Color->Unbind();
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

	_TechniquesMgr->m_UI_Font->Bind();
	_TechniquesMgr->m_UI_Font->SetProjectionMatrix(m_OrhoMatrix);
	_TechniquesMgr->m_UI_Font->SetFontColor(vec3(_color.red, _color.green, _color.blue));

	_font->Render(_string, _pos + offset);

	_TechniquesMgr->m_UI_Font->Unbind();
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
	glViewport(0, 0, Modules::config().windowSizeX, Modules::config().windowSizeY);

	// Projection matix
	m_OrhoMatrix = Matrix4f::OrthoMat(0.0f, Modules::config().windowSizeX, Modules::config().windowSizeY, 0.0f, -1.0f, 1.0f);
}
