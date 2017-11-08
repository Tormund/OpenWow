#pragma once

#include "../Pipeline.h"
#include "../Render.h"

inline void Technique::InitBaseUniforms()
{
    gProjection = getLocation("gProjection");
    gView = getLocation("gView");
    gWorld = getLocation("gWorld");
}

inline void Technique::BindS()
{
	_Render->r->bindShader(shaderId);
}

inline void Technique::Unbind()
{
	_Render->r->bindShader(0);
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

inline void Technique::setTexture(int32 _loc, uint32 value) const
{
    _Render->r->setShaderSampler(_loc, value);
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

inline void Technique::setVec2(const char* name, cvec2 value) const
{
	_Render->r->setShaderConst(getLocation(name), CONST_FLOAT2, &value.x);
}
inline void Technique::setVec2(const char* name, float x, float y) const
{
	_Render->r->setShaderConst(getLocation(name), CONST_FLOAT2, vec2(x, y));
}

//

inline void Technique::setVec3(const char* name, cvec3 value) const
{
	_Render->r->setShaderConst(getLocation(name), CONST_FLOAT3, &value.x);
}
inline void Technique::setVec3(const char* name, float x, float y, float z) const
{
	_Render->r->setShaderConst(getLocation(name), CONST_FLOAT3, vec3(x, y, z));
}

//

inline void Technique::setVec4(const char* name, cvec4 value) const
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

inline void Technique::setMat4(const char* name, cmat4 mat) const
{
    setMat4(getLocation(name), mat);
}

/*inline void Technique::setMat2(int32 _loc, const mat2& mat) const
{
glUniformMatrix2fv(glGetUniformLocation(programOglObj, name), 1, GL_FALSE, &mat[0][0]);
}

inline void Technique::setMat3(int32 _loc, const mat3& mat) const
{
glUniformMatrix3fv(glGetUniformLocation(programOglObj, name), 1, GL_FALSE, &mat[0][0]);
}*/

inline void Technique::setMat4(int32 _loc, cmat4 mat) const
{
    _Render->r->setShaderConst(_loc, CONST_FLOAT44, &mat.x[0]);
}

//

inline void Technique::SetPVW()
{
    _Render->r->setShaderConst(gProjection, CONST_FLOAT44, &_PipelineGlobal->GetProjection().x[0]);
    _Render->r->setShaderConst(gView, CONST_FLOAT44, &_PipelineGlobal->GetView().x[0]);
    _Render->r->setShaderConst(gWorld, CONST_FLOAT44, &_PipelineGlobal->GetWorld().x[0]);
}

inline void Technique::SetPV_W()
{
	setMat4("gPV", _PipelineGlobal->GetPV());
	setMat4("gWorld", _PipelineGlobal->GetWorld());
}

//--

inline void Technique::SetWorldMatrix(cmat4 WorldInverse)
{
    _Render->r->setShaderConst(gWorld, CONST_FLOAT44, &WorldInverse.x[0]);
}

inline void Technique::SetViewMatrix(cmat4 WorldInverse)
{
    _Render->r->setShaderConst(gView, CONST_FLOAT44, &WorldInverse.x[0]);
}

inline void Technique::SetProjectionMatrix(cmat4 WorldInverse)
{
    _Render->r->setShaderConst(gProjection, CONST_FLOAT44, &WorldInverse.x[0]);
}

//--

inline void Technique::BindToPostprocess()
{
	setTexture("gbuf0", 0);//GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
	setTexture("gbuf1", 1);//GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
	setTexture("gbuf2", 2);//GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
	setTexture("gbuf3", 3);//GBuffer::GBUFFER_TEXTURE_TYPE_SPECULAR);
}