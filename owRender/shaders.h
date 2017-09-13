#pragma once

OW_RENDER_DLL_API extern bool supportShaders;

OW_RENDER_DLL_API extern void initShaders();
OW_RENDER_DLL_API extern void reloadShaders();

class ShaderARB
{
	GLenum target;
	GLuint id;
public:
	bool ok;

	OW_RENDER_DLL_API ShaderARB(GLenum target, const char *program);
	~ShaderARB();

	OW_RENDER_DLL_API void bind();
	OW_RENDER_DLL_API void unbind();
};



class ShaderPair
{
	ShaderARB *vertex;
	ShaderARB *fragment;
public:

	OW_RENDER_DLL_API ShaderPair() :vertex(0), fragment(0) {}
	OW_RENDER_DLL_API ShaderPair(ShaderARB *vs, ShaderARB *ps) :vertex(vs), fragment(ps) {}
	OW_RENDER_DLL_API ShaderPair(const char *vprog, const char *fprog);

	OW_RENDER_DLL_API void bind();
	OW_RENDER_DLL_API void unbind();
};

OW_RENDER_DLL_API extern ShaderPair *terrainShaders[4], *wmoShader, *waterShaders[1];
