#include "stdafx.h"

#include "shaders.h"

OW_RENDER_DLL_API bool supportShaders = false;

OW_RENDER_DLL_API ShaderPair *terrainShaders[4] = {0,0,0,0}, *wmoShader = 0, *waterShaders[1] = {0};

void initShaders()
{
	reloadShaders();
}

void reloadShaders()
{
	for (int i = 0; i < 4; i++) delete terrainShaders[i];
	delete wmoShader;
	delete waterShaders[0];

	terrainShaders[0] = new ShaderPair(0, "shaders_wow/terrain1.fs");
	terrainShaders[1] = new ShaderPair(0, "shaders_wow/terrain2.fs");
	terrainShaders[2] = new ShaderPair(0, "shaders_wow/terrain3.fs");
	terrainShaders[3] = new ShaderPair(0, "shaders_wow/terrain4.fs");
	wmoShader = new ShaderPair(0, "shaders_wow/wmospecular.fs");
	waterShaders[0] = new ShaderPair(0, "shaders_wow/wateroutdoor.fs");
}

ShaderARB::ShaderARB(GLenum target, const char *program) :id(0), target(target)
{
	File f = program;

	if (!f.Open())
	{
		Debug::Error("ShaderARB[%s]: Error while open!", program);
		return;
	}

	glGenProgramsARB(1, &id);
	glBindProgramARB(target, id);
	glProgramStringARB(target, GL_PROGRAM_FORMAT_ASCII_ARB, (GLsizei)f.GetSize(), (char*)f.GetData());
	if (glGetError() != 0)
	{
		int errpos;
		glGetIntegerv(GL_PROGRAM_ERROR_POSITION_ARB, &errpos);
		Debug::Error("ShaderARB[%s]: Loading shader: [%s] Error position: [%d]", program, glGetString(GL_PROGRAM_ERROR_STRING_ARB), glGetError());
		ok = false;
	}
	else
	{
		ok = true;
	}
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

//

ShaderPair::ShaderPair(const char *vprog, const char *fprog)
{
	if (vprog && strlen(vprog))
	{
		vertex = new ShaderARB(GL_VERTEX_PROGRAM_ARB, vprog);
		if (!vertex->ok)
		{
			delete vertex;
			vertex = 0;
		}
	}
	else
	{
		vertex = 0;
	}

	if (fprog && strlen(fprog))
	{
		fragment = new ShaderARB(GL_FRAGMENT_PROGRAM_ARB, fprog);
		if (!fragment->ok)
		{
			delete fragment;
			fragment = 0;
		}
	}
	else
	{
		fragment = 0;
	}
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
	if (vertex)
	{
		vertex->unbind();
	}

	if (fragment)
	{
		fragment->unbind();
	}
}
