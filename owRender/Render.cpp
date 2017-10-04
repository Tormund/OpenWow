#include "stdafx.h"

// Include
#include "GLFWBackend.h"
#include "FontsMgr.h"
#include "TexturesMgr.h"

// General
#include "Render.h"

// Additional
#include "TechniquesManager.h"

void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	Debug::Error("---------------");
	Debug::Error("OpenGL Debug message (%d): [%s]", id, message);

	switch (source)
	{
		case GL_DEBUG_SOURCE_API:             Debug::Error("Source: OpenGL API"); break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   Debug::Error("Source: Window System API"); break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: Debug::Error("Source: Shader Compiler"); break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:     Debug::Error("Source: Third Party"); break;
		case GL_DEBUG_SOURCE_APPLICATION:     Debug::Error("Source: Application"); break;
		case GL_DEBUG_SOURCE_OTHER:           Debug::Error("Source: Other"); break;
	}

	switch (type)
	{
		case GL_DEBUG_TYPE_ERROR:               Debug::Error("Type: Error"); break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: Debug::Error("Type: Deprecated Behaviour"); break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  Debug::Error("Type: Undefined Behaviour"); break;
		case GL_DEBUG_TYPE_PORTABILITY:         Debug::Error("Type: Portability"); break;
		case GL_DEBUG_TYPE_PERFORMANCE:         Debug::Error("Type: Performance"); break;
		case GL_DEBUG_TYPE_MARKER:              Debug::Error("Type: Marker"); break;
		case GL_DEBUG_TYPE_PUSH_GROUP:          Debug::Error("Type: Push Group"); break;
		case GL_DEBUG_TYPE_POP_GROUP:           Debug::Error("Type: Pop Group"); break;
		case GL_DEBUG_TYPE_OTHER:               Debug::Error("Type: Other"); break;
	}

	switch (severity)
	{
		case GL_DEBUG_SEVERITY_HIGH:         Debug::Error("Severity: high"); break;
		case GL_DEBUG_SEVERITY_MEDIUM:       Debug::Error("Severity: medium"); break;
		case GL_DEBUG_SEVERITY_LOW:          Debug::Error("Severity: low"); break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: Debug::Error("Severity: notification"); break;
	}


	//system("pause");
	//Debug::Exit(-1);
}


bool RenderGL::Init()
{
	// Debug output
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
		Debug::Error("Mega error !!!!1111");
	}

	if (wglShareLists(glrc1, glrc3) == FALSE)
	{
		Debug::Error("Mega error !!!!2222");
	}

	if (wglShareLists(glrc1, glrc4) == FALSE)
	{
		Debug::Error("Mega error !!!!3333");
	}

	wglMakeCurrent(dc, glrc1);*/

	// GL settings
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);


	glHint(GL_FOG_HINT, GL_FASTEST);
	glHint(GL_GENERATE_MIPMAP_HINT, GL_FASTEST);
	
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);

	glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);

	// Arrays // BOUZI ENABLE IN NOOB MODE
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_NORMAL_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	// Light
	const GLfloat light_ambient[] = {0.0f, 0.0f, 0.0f, 1.0f};
	const GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
	const GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	const GLfloat light_position[] = {50.0f, 50.0f, 50.0f, 0.0f};

	//glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	//glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);

	// Material const
	//const GLfloat mat_ambient[] = {0.7f, 0.7f, 0.7f, 1.0f};
	//const GLfloat mat_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
	//const GLfloat mat_specular[] = {0.3f, 0.3f, 0.3f, 1.0f};
	//const GLfloat mat_shininess[] = {100.0};

	//glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	//glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	// Viewport
	glViewport(0, 0, _Settings->windowSizeX, _Settings->windowSizeY);

	m_OrhoMatrix = glm::ortho(0.0f, _Settings->windowSizeX, _Settings->windowSizeY, 0.0f);

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

	// Alpha test
	glDisable(GL_ALPHA_TEST);

	// Smoothing line
	glDisable(GL_LINE_SMOOTH);

	// Shoothing point
	glDisable(GL_POINT_SMOOTH);
}

void RenderGL::Set2D()
{
	//wglMakeCurrent(dc, glrc1);

	//-----------

	// Cull face
	glDisable(GL_CULL_FACE);

	// Shoothing
	//glEnable(GL_SMOOTH);
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// Depth settings
	glDisable(GL_DEPTH_TEST);

	// Blending settings
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Alpha test
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.001f);

	// Smoothing line
	glLineWidth(1.0f);
	glEnable(GL_LINE_SMOOTH);

	// Shoothing point
	glPointSize(1.0f);
	glEnable(GL_POINT_SMOOTH);

	//-----------

	// Projection is orthographic
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMultMatrixf(glm::value_ptr(m_OrhoMatrix));

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
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

void RenderGL::RenderTexture(cvec2 _pos, Texture* _texture, cvec2 _size, const Rect& _coords)
{
	glEnable(GL_TEXTURE_2D);
	_texture->Bind();
	glPushMatrix();
	{
		glTranslatef(_pos.x, _pos.y, 0.0f);

		glPushAttrib(GL_COLOR_BUFFER_BIT);
		{
			glColor4f(COLOR_WHITE.red, COLOR_WHITE.green, COLOR_WHITE.blue, COLOR_WHITE.alpha);

			glBegin(GL_TRIANGLE_STRIP);
			{
				glTexCoord2f(_coords.p1.x, _coords.p0.y);
				glVertex2f(_size.x, 0.0f);

				glTexCoord2f(_coords.p0.x, _coords.p0.y);
				glVertex2f(0.0f, 0.0f);

				glTexCoord2f(_coords.p1.x, _coords.p1.y);
				glVertex2f(_size.x, _size.y);

				glTexCoord2f(_coords.p0.x, _coords.p1.y);
				glVertex2f(0.0f, _size.y);
			}
			glEnd();
		}
		glPopAttrib();
	}
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

void RenderGL::RenderRectangle(cvec2 _pos, cvec2 _size, bool _filled, const Color& _color)
{
	glPushMatrix();
	{
		glTranslatef(static_cast<double>(_pos.x), static_cast<double>(_pos.y), 0.0);

		glPushAttrib(GL_COLOR_BUFFER_BIT);
		{
			glColor4f(_color.red, _color.green, _color.blue, _color.alpha);

			glBegin(_filled ? GL_POLYGON : GL_LINE_LOOP);
			{
				glTexCoord2f(0, 1);
				glVertex2f(0.0f, 0.0f);

				glTexCoord2f(1, 1);
				glVertex2f(_size.x, 0.0f);

				glTexCoord2f(1, 0);
				glVertex2f(_size.x, _size.y);

				glTexCoord2f(0, 0);
				glVertex2f(0.0f, _size.y);
			}
			glEnd();
		}
		glPopAttrib();
	}
	glPopMatrix();
}

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

void RenderGL::OnWindowResized(uint32_t _width, uint32_t _height)
{
	// Window size
	_Settings->windowSizeX = _width;
	_Settings->windowSizeY = _height;

	// Aspect
	_Settings->CalculateAspectFactor();

	// Set viewport
	glViewport(0, 0, _Settings->windowSizeX, _Settings->windowSizeY);

	// Projection matix
	m_OrhoMatrix = glm::ortho(0.0f, _Settings->windowSizeX, _Settings->windowSizeY, 0.0f);
}
