#pragma once

 extern bool supportShaders;

 extern void initShaders();
 extern void reloadShaders();

class ShaderARB
{
	GLenum target;
	GLuint id;
public:
	bool ok;

	 ShaderARB(GLenum target, const char *program);
	~ShaderARB();

	 void bind();
	 void unbind();
};



class ShaderPair
{
	ShaderARB *vertex;
	ShaderARB *fragment;
public:

	 ShaderPair() :vertex(0), fragment(0) {}
	 ShaderPair(ShaderARB *vs, ShaderARB *ps) :vertex(vs), fragment(ps) {}
	 ShaderPair(const char *vprog, const char *fprog);

	 void bind();
	 void unbind();
};

 extern ShaderPair *terrainShaders[4], *wmoShader, *waterShaders[1];
