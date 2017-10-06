#pragma once

class GLFWBackend;

class RenderGL : public Module
{
	friend GLFWBackend;
public:
	DEF_MODULE(RenderGL);

	void Set3D();
	void Set2D();

	void RenderImage(cvec2 _pos, Image* _image);
	void RenderImage(cvec2 _pos, Image* _image, cvec2 _size);
	void RenderTexture(cvec2 _pos, Texture* _texture, cvec2 _size, const Rect& _coords);
	void RenderRectangle(cvec2 _pos, cvec2 _size, bool _filled, const Color& _color = COLOR_GREEN);

	void RenderText(cvec2 _pos, cstring _string, const Color& _color = COLOR_WHITE) const;
	void RenderText(cvec2 _pos, cstring _string, Font* _font, const Color& _color = COLOR_WHITE) const;
	void RenderText(cvec2 _pos, cstring _string, TextAlignW _alignW, TextAlignH _alignH, const Color& _color = COLOR_WHITE) const;
	void RenderText(cvec2 _pos, cstring _string, TextAlignW _alignW, TextAlignH _alignH, Font* _font, const Color& _color = COLOR_WHITE) const;

	/*public:
		HDC dc;
		HGLRC glrc1;
		HGLRC glrc2;
		HGLRC glrc3;
		HGLRC glrc4;*/

private:
	void OnWindowResized(uint32 _width, uint32 _height);

public:
	Frustum frustum;

private:
	mat4 m_OrhoMatrix;
};

#define _Render RenderGL::instance()