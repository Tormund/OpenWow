#pragma once

class Technique
{
protected:
	 Technique(cstring _fileName);
	 ~Technique();

public:
	bool CompileProgram(cstring _programName);

	 void Bind();
	 void Unbind();

protected:
	 GLint getLocation(cstring name) const;
	 GLint getParam(GLint param) const;

	 void setBool(cstring name, bool value) const;
	 void setInt(cstring name, int value) const;

	 void setFloat(cstring name, float value) const;

	 void setVec2(cstring name, const vec2 &value) const;
	 void setVec2(cstring name, float x, float y) const;

	 void setVec3(cstring name, const vec3 &value) const;
	 void setVec3(cstring name, float x, float y, float z) const;

	 void setVec4(cstring name, const vec4 &value) const;
	 void setVec4(cstring name, float x, float y, float z, float w) const;

	 void setMat2(cstring name, const glm::mat2 &mat) const;
	 void setMat3(cstring name, const glm::mat3 &mat) const;
	 void setMat4(cstring name, const glm::mat4 &mat) const;

public:
	 void SetWVP(cmat4 WVP);

private:
	GLint glfxEffectObj;
	GLint programOglObj;
	string fileName;
};