#pragma once

#include "../GBuffer.h"
#include "../Pipeline.h"

inline void Technique::Bind()
{
	glUseProgram(programOglObj);
}

inline void Technique::Unbind()
{
	glUseProgram(0);
}


inline GLint Technique::getLocation(const char* name) const
{
	return glGetUniformLocation(programOglObj, name);
}

inline GLint Technique::getParam(GLint param) const
{
	GLint ret;
	glGetProgramiv(programOglObj, param, &ret);
	return ret;
}

//

inline void Technique::setBool(const char* name, bool value) const
{
	glUniform1i(glGetUniformLocation(programOglObj, name), (GLint)value);
}

inline void Technique::setInt(const char* name, GLint value) const
{
	glUniform1i(glGetUniformLocation(programOglObj, name), value);
}

inline void Technique::setFloat(const char* name, GLfloat value) const
{
	glUniform1f(glGetUniformLocation(programOglObj, name), value);
}

//

inline void Technique::setVec2(const char* name, cvec2 value) const
{
	glUniform2fv(glGetUniformLocation(programOglObj, name), 1, &value.x);
}
inline void Technique::setVec2(const char* name, GLfloat x, GLfloat y) const
{
	glUniform2f(glGetUniformLocation(programOglObj, name), x, y);
}

//

inline void Technique::setVec3(const char* name, const vec3& value) const
{
	glUniform3fv(glGetUniformLocation(programOglObj, name), 1, &value.x);
}
inline void Technique::setVec3(const char* name, GLfloat x, GLfloat y, GLfloat z) const
{
	glUniform3f(glGetUniformLocation(programOglObj, name), x, y, z);
}

//

inline void Technique::setVec4(const char* name, const vec4& value) const
{
	glUniform4fv(glGetUniformLocation(programOglObj, name), 1, &value.x);
}
inline void Technique::setVec4(const char* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) const
{
	glUniform4f(glGetUniformLocation(programOglObj, name), x, y, z, w);
}

//

/*inline void Technique::setMat2(const char* name, const mat2& mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(programOglObj, name), 1, GL_FALSE, &mat[0][0]);
}

inline void Technique::setMat3(const char* name, const mat3& mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(programOglObj, name), 1, GL_FALSE, &mat[0][0]);
}*/

inline void Technique::setMat4(const char* name, const mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(programOglObj, name), 1, GL_FALSE, &mat.x[0]);
}

//

inline void Technique::SetPVW()
{
	setMat4("gProjection", _PipelineGlobal->GetProjection());
	setMat4("gView", _PipelineGlobal->GetView());
	setMat4("gWorld", _PipelineGlobal->GetWorld());
}

inline void Technique::SetWorldMatrix(cmat4 WorldInverse)
{
	setMat4("gWorld", WorldInverse);
}

inline void Technique::SetViewMatrix(cmat4 WorldInverse)
{
	setMat4("gView", WorldInverse);
}

inline void Technique::SetProjectionMatrix(cmat4 WorldInverse)
{
	setMat4("gProjection", WorldInverse);
}

inline void Technique::BindToPostprocess()
{
	setInt("gbuffer.gWorldSpacePosMap", GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
	setInt("gbuffer.gNormalMap", GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);

	setInt("gbuffer.gAmbientMap", GBuffer::GBUFFER_TEXTURE_TYPE_AMBIENT);
	setInt("gbuffer.gDiffuseMap", GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
	setInt("gbuffer.gSpecularMap", GBuffer::GBUFFER_TEXTURE_TYPE_SPECULAR);
	setInt("gbuffer.gSpecularShininessMap", GBuffer::GBUFFER_TEXTURE_TYPE_SPECULAR_SHININESS);
}