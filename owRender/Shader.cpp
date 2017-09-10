#include "stdafx.h"

// General
#include "Shader.h"

Shader::Shader(GLenum _shaderType, cstring _shaderFileName) : shaderType(_shaderType), shaderOglObj(0)
{
	File f = _shaderFileName;
	if (!f.Open())
	{
		Debug::Error("Shader[%s]: Error while open shader file.", _shaderFileName.c_str());
		return;
	}

	shaderOglObj = glCreateShader(shaderType);

	if (shaderOglObj == 0)
	{
		Debug::Error("Shader[%s]: Error create shader OGL object! Shader type [%d].", _shaderFileName.c_str(), _shaderType);
		return;
	}

	const GLchar* data = (GLchar*)f.GetData();
	glShaderSource(shaderOglObj, 1, &data, 0);

	Debug::Info("Shader[%s]: Loaded successfully.", _shaderFileName.c_str());
}

Shader::~Shader()
{
	if (shaderOglObj != 0)
	{
		glDeleteShader(shaderOglObj);
	}
}

bool Shader::Compile()
{
	GLint status;
	GLint length;
	GLchar info[512];

	// Compile
	glCompileShader(shaderOglObj);

	// Get status
	glGetShaderiv(shaderOglObj, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		glGetShaderiv(shaderOglObj, GL_INFO_LOG_LENGTH, &length);
		memset(info, 0, 512);
		glGetShaderInfoLog(shaderOglObj, length, 0, info);
		Debug::Error("Shader[]: Compiled failed [%s].", info);
		return false;
	}

	Debug::Info("Shader[]: Compiled successfully.");

	return true;
}
