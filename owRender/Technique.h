#pragma once

class Technique {
protected:
	OW_RENDER_DLL_API Technique(cstring _fileName);
	OW_RENDER_DLL_API ~Technique();

public:
	bool CompileProgram(cstring _programName);

	OW_RENDER_DLL_API void Bind();
	OW_RENDER_DLL_API void Unbind();

protected:
	OW_RENDER_DLL_API GLint getLocation(cstring name) const;
	OW_RENDER_DLL_API GLint getParam(GLint param) const;

	OW_RENDER_DLL_API void setBool(cstring name, bool value) const;
	OW_RENDER_DLL_API void setInt(cstring name, int value) const;

	OW_RENDER_DLL_API void setFloat(cstring name, float value) const;

	OW_RENDER_DLL_API void setVec2(cstring name, const vec2 &value) const;
	OW_RENDER_DLL_API void setVec2(cstring name, float x, float y) const;

	OW_RENDER_DLL_API void setVec3(cstring name, const vec3 &value) const;
	OW_RENDER_DLL_API void setVec3(cstring name, float x, float y, float z) const;

	OW_RENDER_DLL_API void setVec4(cstring name, const vec4 &value) const;
	OW_RENDER_DLL_API void setVec4(cstring name, float x, float y, float z, float w) const;

	OW_RENDER_DLL_API void setMat2(cstring name, const glm::mat2 &mat) const;
	OW_RENDER_DLL_API void setMat3(cstring name, const glm::mat3 &mat) const;
	OW_RENDER_DLL_API void setMat4(cstring name, const glm::mat4 &mat) const;

public:
	OW_RENDER_DLL_API void SetWVP(cmat4 WVP);

private:
	GLint glfxEffectObj;
	GLint programOglObj;
	string fileName;
};