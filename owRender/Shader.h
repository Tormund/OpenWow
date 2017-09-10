#pragma once

class Shader
{
public:
	Shader(GLenum _type, cstring _shaderFileName);
	~Shader();

	bool Compile();

	inline GLenum GetShaderType() const { return shaderType; }
	inline GLuint GetOpenglId() const { return shaderOglObj; }

private:
	GLenum shaderType;
	GLuint shaderOglObj;
};