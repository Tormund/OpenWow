#pragma once

#include "../Pipeline.h"
#include "../Render.h"

inline void Technique::BindS()
{
	_Render->r->bindShader(shaderId);
}

inline void Technique::Unbind()
{
	//_Render->r->bindShader(0);
}


inline int32 Technique::getLocation(const char* name) const
{
	return _Render->r->getShaderConstLoc(shaderId, name);
}


//

inline void Technique::setTexture(const char* name, uint32 value) const
{
	_Render->r->setShaderSampler(getLocation(name), value);
}

//

inline void Technique::setInt(const char* name, int32 value) const
{
	_Render->r->setShaderConst(getLocation(name), CONST_INT, &value);
}

inline void Technique::setFloat(const char* name, float value) const
{
	_Render->r->setShaderConst(getLocation(name), CONST_FLOAT, &value);
}

//

inline void Technique::setVec2(const char* name, vec2 value) const
{
	_Render->r->setShaderConst(getLocation(name), CONST_FLOAT2, &value.x);
}
inline void Technique::setVec2(const char* name, float x, float y) const
{
	_Render->r->setShaderConst(getLocation(name), CONST_FLOAT2, vec2(x, y));
}

//

inline void Technique::setVec3(const char* name, vec3 value) const
{
	_Render->r->setShaderConst(getLocation(name), CONST_FLOAT3, &value.x);
}
inline void Technique::setVec3(const char* name, float x, float y, float z) const
{
	_Render->r->setShaderConst(getLocation(name), CONST_FLOAT3, vec3(x, y, z));
}

//

inline void Technique::setVec4(const char* name, vec4 value) const
{
	_Render->r->setShaderConst(getLocation(name), CONST_FLOAT4, &value.x);
}
inline void Technique::setVec4(const char* name, float x, float y, float z, float w) const
{
	_Render->r->setShaderConst(getLocation(name), CONST_FLOAT4, vec4(x, y, z, w));
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

inline void Technique::setMat4(const char* name, mat4 mat) const
{
	_Render->r->setShaderConst(getLocation(name), CONST_FLOAT44, &mat.x[0]);
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
	setTexture("gbuf0", 0);//GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
	setTexture("gbuf1", 1);//GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
	setTexture("gbuf2", 2);//GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
	setTexture("gbuf3", 3);//GBuffer::GBUFFER_TEXTURE_TYPE_SPECULAR);
}