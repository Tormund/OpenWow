#pragma once

#include "Camera.h"
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

	void RenderImage(vec2 _pos, Image* _image);
	void RenderImage(vec2 _pos, Image* _image, vec2 _size);

    void RenderTexture(vec2 _pos, Texture* _texture);
    void RenderTexture(vec2 _pos, Texture* _texture, vec2 _size);
	void RenderTexture(vec2 _pos, uint32 _texture, vec2 _size);

	void RenderRectangle(vec2 _pos, vec2 _size, const Color& _color = COLOR_GREEN);
	void RenderRectangleOutline(vec2 _pos, vec2 _size, const Color& _color = COLOR_GREEN);

	void RenderText(vec2 _pos, cstring _string, const Color& _color = COLOR_WHITE) const;
	void RenderText(vec2 _pos, cstring _string, Font* _font, const Color& _color = COLOR_WHITE) const;
	void RenderText(vec2 _pos, cstring _string, TextAlignW _alignW, TextAlignH _alignH, const Color& _color = COLOR_WHITE) const;
	void RenderText(vec2 _pos, cstring _string, TextAlignW _alignW, TextAlignH _alignH, Font* _font, const Color& _color = COLOR_WHITE) const;

	void RenderQuad();
    void RenderQuadVT();

private:
	void OnWindowResized(uint32 _width, uint32 _height);

private:
	mat4 m_OrhoMatrix;

public:
	RenderDevice* r;
    uint32 rb;
    uint32 rbFinal;
    Camera* mainCamera;
};

#define _Render RenderGL::instance()