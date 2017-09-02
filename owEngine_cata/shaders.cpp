#include "stdafx.h"

#include "shaders.h"

bool supportShaders = false;

/*PFNGLPROGRAMSTRINGARBPROC glProgramStringARB = NULL;
PFNGLBINDPROGRAMARBPROC glBindProgramARB = NULL;
PFNGLDELETEPROGRAMSARBPROC glDeleteProgramsARB = NULL;
PFNGLGENPROGRAMSARBPROC glGenProgramsARB = NULL;
PFNGLPROGRAMLOCALPARAMETER4FARBPROC glProgramLocalParameter4fARB;*/

ShaderPair *terrainShaders[4] = {0,0,0,0}, *wmoShader = 0, *waterShaders[1] = {0};

void initShaders()
{
	supportShaders = false;

	reloadShaders();
}

void reloadShaders()
{
	for (int i = 0; i < 4; i++) delete terrainShaders[i];
	delete wmoShader;
	delete waterShaders[0];

	terrainShaders[0] = new ShaderPair(0, "shaders/terrain1.fs", true);
	terrainShaders[1] = new ShaderPair(0, "shaders/terrain2.fs", true);
	terrainShaders[2] = new ShaderPair(0, "shaders/terrain3.fs", true);
	terrainShaders[3] = new ShaderPair(0, "shaders/terrain4.fs", true);
	wmoShader = new ShaderPair(0, "shaders/wmospecular.fs", true);
	waterShaders[0] = new ShaderPair(0, "shaders/wateroutdoor.fs", true);
}

ShaderARB::ShaderARB(GLenum target, const char *program, bool fromFile) :id(0), target(target)
{
	if (!program || !strlen(program))
	{
		ok = true;
		return;
	}

	const char *progtext;
	char *buf;
	if (fromFile)
	{
		FILE *f = fopen(program, "rb");
		if (!f)
		{
			ok = false;
			return;
		}
		fseek(f, 0, SEEK_END);
		size_t len = ftell(f);
		fseek(f, 0, SEEK_SET);

		buf = new char[len + 1];
		progtext = buf;
		fread(buf, len, 1, f);
		buf[len] = 0;
		fclose(f);
		//Debug::Info("Len: %d\nShader text:\n[%s]",len,progtext);
	}
	else
		progtext = program;

	glGenProgramsARB(1, &id);
	glBindProgramARB(target, id);
	glProgramStringARB(target, GL_PROGRAM_FORMAT_ASCII_ARB, (GLsizei)strlen(progtext), progtext);
	if (glGetError() != 0)
	{
		int errpos;
		glGetIntegerv(GL_PROGRAM_ERROR_POSITION_ARB, &errpos);
		Debug::Error("Error loading shader: %s Error position: %d", glGetString(GL_PROGRAM_ERROR_STRING_ARB), glGetError());
		ok = false;
	}
	else
		ok = true;

	if (fromFile) delete[] buf;
}

ShaderARB::~ShaderARB()
{
	if (ok && id) glDeleteProgramsARB(1, &id);
}

void ShaderARB::bind()
{
	glBindProgramARB(target, id);
	glEnable(target);
}

void ShaderARB::unbind()
{
	glDisable(target);
}

ShaderPair::ShaderPair(const char *vprog, const char *fprog, bool fromFile)
{
	if (vprog && strlen(vprog))
	{
		vertex = new ShaderARB(GL_VERTEX_PROGRAM_ARB, vprog, fromFile);
		if (!vertex->ok)
		{
			delete vertex;
			vertex = 0;
		}
	}
	else vertex = 0;
	if (fprog && strlen(fprog))
	{
		fragment = new ShaderARB(GL_FRAGMENT_PROGRAM_ARB, fprog, fromFile);
		if (!fragment->ok)
		{
			delete fragment;
			fragment = 0;
		}
	}
	else fragment = 0;
}

void ShaderPair::bind()
{
	if (vertex)
	{
		glEnable(GL_VERTEX_PROGRAM_ARB);
		vertex->bind();
	}
	else
	{
		glDisable(GL_VERTEX_PROGRAM_ARB);
	}

	if (fragment)
	{
		glEnable(GL_FRAGMENT_PROGRAM_ARB);
		fragment->bind();
	}
	else
	{
		glDisable(GL_FRAGMENT_PROGRAM_ARB);
	}
}

void ShaderPair::unbind()
{
	if (vertex) vertex->unbind();
	if (fragment) fragment->unbind();
}
