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
	void RenderTexture(cvec2 _pos, uint32 _texture, cvec2 _size, const Rect& _coords = Rect(0,0,1,1));
	void RenderTexture(cvec2 _pos, Texture* _texture, cvec2 _size, const Rect& _coords);

	void RenderRectangle(cvec2 _pos, cvec2 _size, const Color& _color = COLOR_GREEN);
	void RenderRectangleOutline(cvec2 _pos, cvec2 _size, const Color& _color = COLOR_GREEN);

	void RenderText(cvec2 _pos, cstring _string, const Color& _color = COLOR_WHITE) const;
	void RenderText(cvec2 _pos, cstring _string, Font* _font, const Color& _color = COLOR_WHITE) const;
	void RenderText(cvec2 _pos, cstring _string, TextAlignW _alignW, TextAlignH _alignH, const Color& _color = COLOR_WHITE) const;
	void RenderText(cvec2 _pos, cstring _string, TextAlignW _alignW, TextAlignH _alignH, Font* _font, const Color& _color = COLOR_WHITE) const;

	void RenderQuad();

private:
	void OnWindowResized(uint32 _width, uint32 _height);

private:
	mat4 m_OrhoMatrix;

	uint32 __quadGeom;

public:
	RenderDevice* r;

	uint32 __layoutPos2;
	uint32 __layoutFont;
	

	// World
	uint32 __layoutSky;
	uint32 __layoutMapLowResolution;
	uint32 __layoutMapChunk;
	uint32 __layoutWMO;
	uint32 __layoutWMO_VC;
	uint32 __layoutMDX;
	uint32 __layoutWater;

	uint32 __vbPos2;
	uint32 __geomPos2;

	uint32 __vb;
	uint32 __geom;

	uint32 __vbPos3;
	uint32 __geomPos3;
};

#define _Render RenderGL::instance()