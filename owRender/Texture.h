#pragma once

class Texture : public RefItem
{
public:
	Texture() : RefItem(), oglObj(0), size(VECTOR_ZERO)
	{}

	Texture(GLuint _openglId) : RefItem(), oglObj(_openglId), size(VECTOR_ZERO)
	{}

	Texture(GLuint _openglId, cvec2 _size) : RefItem(), oglObj(_openglId), size(_size)
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

	 void GenerateTexture();
	 void Bind(uint8 _textureUnit = 0) const;
	 void Unbind() const;

private:
	GLuint oglObj;
	vec2 size;
};