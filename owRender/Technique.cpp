#include "stdafx.h"

// General
#include "Technique.h"

Technique::Technique(cstring _fileName)
{
	fileName = _fileName;
	programOglObj = 0;
	glfxEffectObj = glfxGenEffect();

	Debug::Info("Technique[%s]: GLFX_OBJ [%d]", _fileName.c_str(), glfxEffectObj);
}

Technique::~Technique()
{
	if (programOglObj != 0)
	{
		glDeleteProgram(programOglObj);
		programOglObj = 0;
	}

	glfxDeleteEffect(glfxEffectObj);
}

bool Technique::CompileProgram(cstring _programName)
{
	File f = fileName;
	

	/*if (!f.Open())
	{
		Debug::Error("Technique[%s][%s]: Unable to open file.", fileName.c_str(), _programName.c_str());
		return false;
	}*/

	string log;

	if (!glfxParseEffectFromFile(glfxEffectObj, f.FullPath().c_str())) // FIXME
	{
		log = glfxGetEffectLog(glfxEffectObj);
		Debug::Error("Techique[%s]: Error creating program.", fileName.c_str());
		Debug::Error("Techique[%s]: Log [%s].", fileName.c_str(), log.c_str());
		return false;
	}

	programOglObj = glfxCompileProgram(glfxEffectObj, _programName.c_str());
	Debug::Info("Technique[%s]: Program [%s] OGLID [%d]", fileName.c_str(), _programName.c_str(), glfxEffectObj);

	if (programOglObj < 0)
	{
		log = glfxGetEffectLog(glfxEffectObj);
		Debug::Error("Techique[%s]: Error compiling program [%s].", fileName.c_str(), _programName.c_str());
		Debug::Error("Techique[%s]: Log [%s].", fileName, log.c_str());
		return false;
	}

	return true;
}

/*bool Technique::Init() {
	programOglObj = glCreateProgram();

	if(programOglObj == 0) {
		Debug::Error("Technique[]: Error init technique!");
		return false;
	}

	return true;
}

bool Technique::AddShader(GLenum _shaderType, cstring _shaderFileName) {
	// Check this shader type exists
	for(auto it = shadersList.begin(); it != shadersList.end(); ++it) {
		if((*it)->GetShaderType() == _shaderType) {
			Debug::Error("Technique[]: Error Add shader [%s]. Shader type [%d] already exists!", _shaderFileName.c_str(), _shaderType);
			return false;
		}
	}

	auto shader = new Shader(_shaderType, _shaderFileName);

	// Check OGL object create successfully
	if(shader->GetOpenglId() == 0) {
		Debug::Error("Technique[]: Error create shader! Name [%s]. Type [%d].", _shaderFileName.c_str(), _shaderType);
		return false;
	}

	// Add to list (delete later)
	shadersList.push_back(shader);

	// Compile shader
	if(!shader->Compile()) {
		Debug::Error("Technique[]: Error compile shader! Name [%s]. Type [%d].", _shaderFileName.c_str(), _shaderType);
		return false;
	}

	// Attach to program
	glAttachShader(programOglObj, shader->GetOpenglId());

	Debug::Print("Technique[]: Shader [%s] attached successfully! Type [%d].", _shaderFileName.c_str(), _shaderType);

	return true;
}

bool Technique::Finalize() {
	// Link shaders
	if(!Link())
		return false;

	// Validate shaders
	if(!Validate())
		return false;

	// Delete shader
	for(auto it = shadersList.begin(); it != shadersList.end(); it++)
		delete (*it);
	shadersList.clear();

	Debug::Info("Technique[]: Successfully linked and validating.");

	return true;
}

bool Technique::Link() {
	GLint status;
	GLint length;
	GLchar info[512];

	// Link program
	glLinkProgram(programOglObj);

	// Get status
	glGetProgramiv(programOglObj, GL_LINK_STATUS, &status);
	if(status == GL_FALSE) {
		glGetProgramiv(programOglObj, GL_INFO_LOG_LENGTH, &length);
		memset(info, 0, 512);
		glGetProgramInfoLog(programOglObj, length, NULL, info);
		Debug::Error("Technique[]: Linked failed [%s].", info);
		return false;
	}

	return true;
}

bool Technique::Validate() {
	GLint status;
	GLint length;
	GLchar info[512];

	// Validate program
	glValidateProgram(programOglObj);

	// Get status
	glGetProgramiv(programOglObj, GL_VALIDATE_STATUS, &status);
	if(status == GL_FALSE) {
		glGetProgramiv(programOglObj, GL_INFO_LOG_LENGTH, &length);
		memset(info, 0, 512);
		glGetProgramInfoLog(programOglObj, length, NULL, info);
		Debug::Error("Technique[]: Validation failed [%s].", info);
		return false;
	}

	return true;
}*/

//-----

void Technique::Bind()
{
	glUseProgram(programOglObj);
}

void Technique::Unbind()
{
	glUseProgram(0);
}

//-----

GLint Technique::getLocation(cstring name) const
{
	GLint location = glGetUniformLocation(programOglObj, name.c_str());
	//assert2(location != 0xFFFFFFFF, name.c_str());
	return location;
}

GLint Technique::getParam(GLint param) const
{
	GLint ret;
	glGetProgramiv(programOglObj, param, &ret);
	return ret;
}

//-----

void Technique::setBool(cstring name, bool value) const
{
	glUniform1i(getLocation(name), (int)value);
}

void Technique::setInt(cstring name, int value) const
{
	glUniform1i(getLocation(name), value);
}

void Technique::setFloat(cstring name, float value) const
{
	glUniform1f(getLocation(name), value);
}

void Technique::setVec2(cstring name, cvec2 value) const
{
	glUniform2fv(getLocation(name), 1, &value[0]);
}
void Technique::setVec2(cstring name, float x, float y) const
{
	glUniform2f(getLocation(name), x, y);
}

void Technique::setVec3(cstring name, const vec3& value) const
{
	glUniform3fv(getLocation(name), 1, &value[0]);
}
void Technique::setVec3(cstring name, float x, float y, float z) const
{
	glUniform3f(getLocation(name), x, y, z);
}

void Technique::setVec4(cstring name, const vec4& value) const
{
	glUniform4fv(getLocation(name), 1, &value[0]);
}
void Technique::setVec4(cstring name, float x, float y, float z, float w) const
{
	glUniform4f(getLocation(name), x, y, z, w);
}

void Technique::setMat2(cstring name, const glm::mat2& mat) const
{
	glUniformMatrix2fv(getLocation(name), 1, GL_FALSE, &mat[0][0]);
}

void Technique::setMat3(cstring name, const glm::mat3& mat) const
{
	glUniformMatrix3fv(getLocation(name), 1, GL_FALSE, &mat[0][0]);
}

void Technique::setMat4(cstring name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(getLocation(name), 1, GL_FALSE, &mat[0][0]);
}

void Technique::SetWVP(cmat4 WVP)
{
	setMat4("gWVP", WVP);
}