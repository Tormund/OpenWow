#include "stdafx.h"

// Include
#include "GLFWBackend.h"
#include "FontsMgr.h"
#include "TexturesMgr.h"

// General
#include "Render.h"

void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	Debug::Error("---------------");
	Debug::Error("Debug message (%d): %s", id, message);

	switch (source)
	{
		case GL_DEBUG_SOURCE_API:             Debug::Error("Source: API"); break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   Debug::Error("Source: Window System"); break;
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
}


bool RenderGL::Init()
{
	// Window size
	windowSize = vec2(1024, 768);

	// Aspect
	aspectFactor = 1.0;
	aspectRatio = static_cast<double>(windowSize.x) / static_cast<double>(windowSize.y);
	if (aspectRatio >= 1.0)
		aspectFactor = aspectRatio;

	// Debug output
	GLint flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}

	// GL settings
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Cull face
	glCullFace(GL_BACK);

	// Shoothing
	glEnable(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// Depth settings
	glEnable(GL_DEPTH_TEST);

	// Blending settings
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Alpha test
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.001f);

	// Smoothing line
	glLineWidth(1.5f);
	glEnable(GL_LINE_SMOOTH);

	// Shoothing point
	glPointSize(1.5f);
	glEnable(GL_POINT_SMOOTH);

	// Arrays
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

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
	glViewport(0, 0, windowSize.x, windowSize.y);
	return true;
}

void RenderGL::Destroy() {}

void RenderGL::Set3D()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0, aspectRatio, 1.0, 20480.0); //set the perspective (angle of sight, width, height, ,depth)

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//glDisable(GL_DEPTH_TEST);
	//glDisable(GL_BLEND);
	//glDisable(GL_CULL_FACE);
	//glDisable(GL_ALPHA_TEST);
}

void RenderGL::Set2D()
{
	// Disable depth test
	glDisable(GL_DEPTH_TEST);

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Disable cull face
	glDisable(GL_CULL_FACE);

	// Disable lighting
	glDisable(GL_LIGHTING);

	// Alpha test
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.001f);

	// Projection is orthographic
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	mat4 orho = glm::ortho(0.0f, static_cast<float>(windowSize.x), static_cast<float>(windowSize.y), 0.0f);
	glMultMatrixf(&orho[0][0]);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	//glActiveTexture(GL_TEXTURE5);
	//glBindTexture(GL_TEXTURE_2D, 0);
	//glDisable(GL_TEXTURE_2D);
}

// UI

void RenderGL::RenderImage(const vec2& _pos, Image* _image)
{
	RenderTexture(_pos - _image->offset, _image->texture, _image->size, _image->coords);
}

void RenderGL::RenderImage(const vec2& _pos, Image* _image, const vec2& _size)
{
	RenderTexture(_pos - _image->offset, _image->texture, _size, _image->coords);
}

void RenderGL::RenderTexture(const vec2& _pos, Texture* _texture, const vec2& _size, const Rect<double>& _coords)
{
	glEnable(GL_TEXTURE_2D);
	_texture->Bind();
	glPushMatrix();
	{
		glTranslated(static_cast<double>(_pos.x), static_cast<double>(_pos.y), 0.0);

		glPushAttrib(GL_COLOR_BUFFER_BIT);
		{
			glColor4d(COLOR_WHITE.red, COLOR_WHITE.green, COLOR_WHITE.blue, COLOR_WHITE.alpha);

			glBegin(GL_TRIANGLE_STRIP);
			{
				glTexCoord2d(_coords.p1.x, _coords.p0.y);
				glVertex2i(_size.x, 0);

				glTexCoord2d(_coords.p0.x, _coords.p0.y);
				glVertex2i(0, 0);

				glTexCoord2d(_coords.p1.x, _coords.p1.y);
				glVertex2i(_size.x, _size.y);

				glTexCoord2d(_coords.p0.x, _coords.p1.y);
				glVertex2i(0, _size.y);
			}
			glEnd();
		}
		glPopAttrib();
	}
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

void RenderGL::RenderRectangle(const vec2& _pos, const vec2& _size, bool _filled, const Color& _color)
{
	glPushMatrix();
	{
		glTranslated(static_cast<double>(_pos.x), static_cast<double>(_pos.y), 0.0);

		glPushAttrib(GL_COLOR_BUFFER_BIT);
		{
			glColor4d(_color.red, _color.green, _color.blue, _color.alpha);

			glBegin(_filled ? GL_POLYGON : GL_LINE_LOOP);
			{
				glVertex2i(0, 0);
				glVertex2i(_size.x, 0);
				glVertex2i(_size.x, _size.y);
				glVertex2i(0, _size.y);
			}
			glEnd();
		}
		glPopAttrib();
	}
	glPopMatrix();
}

void RenderGL::RenderText(const vec2& _pos, cstring _string, const Color & _color) const
{
	RenderText(_pos, _string, TextAlignW::TEXT_ALIGNW_LEFT, TextAlignH::TEXT_ALIGNH_BOTTOM, _FontsMgr->GetMainFont(), _color);
}

void RenderGL::RenderText(const vec2& _pos, cstring _string, Font* _font, const Color & _color) const
{
	RenderText(_pos, _string, TextAlignW::TEXT_ALIGNW_LEFT, TextAlignH::TEXT_ALIGNH_BOTTOM, _font, _color);
}

void RenderGL::RenderText(const vec2& _pos, cstring _string, TextAlignW _alignW, TextAlignH _alignH, const Color& _color) const
{
	RenderText(_pos, _string, _alignW, _alignH, _FontsMgr->GetMainFont(), _color);
}

void RenderGL::RenderText(const vec2& _pos, cstring _string, TextAlignW _alignW, TextAlignH _alignH, Font* _font, const Color& _color) const
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

	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	{
		glTranslatef(static_cast<float>(_pos.x) + offset.x, static_cast<float>(_pos.y) + offset.y, 0.0f);

		glPushAttrib(GL_COLOR_BUFFER_BIT);
		{
			glColor4d(_color.red, _color.green, _color.blue, _color.alpha);

			_font->Render(_string);
		}
		glPopAttrib();
	}
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

//

void RenderGL::OnWindowResized(uint32_t _width, uint32_t _height)
{
	// Window size
	windowSize.x = _width;
	windowSize.y = _height;

	// Aspect
	aspectFactor = 1.0f;
	aspectRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
	if (aspectRatio >= 1.0f)
		aspectFactor = aspectRatio;

	// Set viewport
	//glViewport(0, 0, windowSize.x, windowSize.y);
}
