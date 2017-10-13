#pragma once

class Technique
{
protected:
	Technique(cstring _fileName);
	Technique(cstring _fileNameVS, cstring _fileNameFS);
	~Technique();

public:
	bool CompileProgram(cstring _programName);

	inline void Bind();
	inline void Unbind();

protected:
	inline GLint getLocation(const char* name) const;

	inline void setTexture(const char* name, GLuint value) const;

	inline void setBool(const char* name, bool value) const;
	inline void setInt(const char* name, GLint value) const;
	inline void setFloat(const char* name, GLfloat value) const;

	inline void setVec2(const char* name, vec2 value) const;
	inline void setVec2(const char* name, GLfloat x, GLfloat y) const;

	inline void setVec3(const char* name, vec3 value) const;
	inline void setVec3(const char* name, GLfloat x, GLfloat y, GLfloat z) const;

	inline void setVec4(const char* name, vec4 value) const;
	inline void setVec4(const char* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) const;

	//inline void setMat2(const char* name, const mat2& mat) const;
	//inline void setMat3(const char* name, const mat3& mat) const;
	inline void setMat4(const char* name, mat4 mat) const;

public:
	inline void SetPVW();

	inline void SetWorldMatrix(cmat4 WorldInverse);
	inline void SetViewMatrix(cmat4 WorldInverse);
	inline void SetProjectionMatrix(cmat4 WorldInverse);

	inline void BindToPostprocess();

private:
	string fileName;

	uint32 shaderId;
};

#include "Technique.inl"