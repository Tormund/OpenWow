#pragma once

#include "RenderDevice.h"

class GLFWBackend;

struct RenderTarget
{
	std::string           id;
	uint32                numColBufs;
	R_TextureFormats::List  format;
	uint32                width, height;
	uint32                samples;
	float                 scale;  // Scale factor for FB width and height
	bool                  hasDepthBuf;
	uint32                rendBuf;

	RenderTarget()
	{
		hasDepthBuf = false;
		numColBufs = 0;
		rendBuf = 0;
		width = height = 0;
		samples = 0;
		scale = 0;
		format = R_TextureFormats::Unknown;
	}
};

class RenderGL : public Module
{
	friend GLFWBackend;
public:
	DEF_MODULE(RenderGL);

	void Set3D();
	void Set2D();

	void RenderImage(cvec2 _pos, Image* _image);
	void RenderImage(cvec2 _pos, Image* _image, cvec2 _size);
	void RenderTexture(cvec2 _pos, GLuint _texture, cvec2 _size, const Rect& _coords = Rect(0,0,1,1));
	void RenderTexture(cvec2 _pos, Texture* _texture, cvec2 _size, const Rect& _coords);

	void RenderRectangle(cvec2 _pos, cvec2 _size, const Color& _color = COLOR_GREEN);
	void RenderRectangleOutline(cvec2 _pos, cvec2 _size, const Color& _color = COLOR_GREEN);

	void RenderText(cvec2 _pos, cstring _string, const Color& _color = COLOR_WHITE) const;
	void RenderText(cvec2 _pos, cstring _string, Font* _font, const Color& _color = COLOR_WHITE) const;
	void RenderText(cvec2 _pos, cstring _string, TextAlignW _alignW, TextAlignH _alignH, const Color& _color = COLOR_WHITE) const;
	void RenderText(cvec2 _pos, cstring _string, TextAlignW _alignW, TextAlignH _alignH, Font* _font, const Color& _color = COLOR_WHITE) const;

private:
	void OnWindowResized(uint32 _width, uint32 _height);

private:
	mat4 m_OrhoMatrix;
	GLuint m_ImageBuffer;
	GLuint m_ColorBuffer;

public:
	RenderDevice* r;

public:
	vector<RenderTarget>   _renderTargets;

	void addRenderTarget(const string &id, bool depthBuffer, uint32 numBuffers, R_TextureFormats::List format, uint32 samples, uint32 width, uint32 height, float scale);
	RenderTarget *findRenderTarget(const string &id) const;
	bool createRenderTargets();
	void releaseRenderTargets();
};

#define _Render RenderGL::instance()