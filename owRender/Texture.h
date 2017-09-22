#pragma once

class Texture : public RefItem
{
public:
	Texture() : RefItem(), oglObj(0), size(VECTOR_ZERO), inited(false)
	{}

	Texture(GLuint _openglId) : RefItem(), oglObj(_openglId), size(VECTOR_ZERO), inited(false)
	{}

	Texture(GLuint _openglId, cvec2 _size) : RefItem(), oglObj(_openglId), size(_size), inited(false)
	{}

	~Texture()
	{
		if (glIsTexture(oglObj))
		{
			glDeleteTextures(1, &oglObj);
		}
	}

	//

	bool IsZeroSized() const { return (size == VECTOR_ZERO); }

	cvec2 GetSize() const { return size; }
	void SetSize(cvec2 _value) { size = _value; }

	//

	OW_RENDER_DLL_API void GenerateTexture();
	OW_RENDER_DLL_API void Bind(uint8_t _textureUnit = 0) const;
	OW_RENDER_DLL_API void Unbind() const;
	bool inited;

private:
	GLuint oglObj;
	vec2 size;
};