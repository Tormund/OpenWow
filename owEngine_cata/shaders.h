#ifndef SHADERS_H
#define SHADERS_H

extern bool supportShaders;

/*extern PFNGLPROGRAMSTRINGARBPROC glProgramStringARB;
extern PFNGLBINDPROGRAMARBPROC glBindProgramARB;
extern PFNGLDELETEPROGRAMSARBPROC glDeleteProgramsARB;
extern PFNGLGENPROGRAMSARBPROC glGenProgramsARB;
extern PFNGLPROGRAMLOCALPARAMETER4FARBPROC glProgramLocalParameter4fARB;*/


void initShaders();
void reloadShaders();

class ShaderARB
{
	GLenum target;
	GLuint id;
public:
	bool ok;

	ShaderARB(GLenum target, const char *program, bool fromFile = false);
	~ShaderARB();

	virtual void bind();
	virtual void unbind();
};

class ShaderPair
{
	ShaderARB *vertex;
	ShaderARB *fragment;
public:

	ShaderPair() :vertex(0), fragment(0) {}
	ShaderPair(ShaderARB *vs, ShaderARB *ps) :vertex(vs), fragment(ps) {}
	ShaderPair(const char *vprog, const char *fprog, bool fromFile = false);

	void bind();
	void unbind();
};

extern ShaderPair *terrainShaders[4], *wmoShader, *waterShaders[1];


#endif
