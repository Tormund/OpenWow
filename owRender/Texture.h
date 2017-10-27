#pragma once

class Texture : public RefItem
{
public:
	Texture() : RefItem(), m_Object(0), size(VECTOR_ZERO)
	{}

	Texture(uint32 _Object) : RefItem(), m_Object(_Object), size(VECTOR_ZERO)
	{}

	Texture(uint32 _Object, cvec2 _size) : RefItem(), m_Object(_Object), size(_size)
	{}

	~Texture()
	{
		// _Render->r->delete
	}

	//

	bool IsZeroSized() const { return (size == VECTOR_ZERO); }

	cvec2 GetSize() const { return size; }
	void SetSize(cvec2 _value) { size = _value; }

	uint32 GetObj() const { return m_Object; }
	void SetObj(uint32 _value) { m_Object = _value; }

	//

	 void GenerateTexture();

private:
	vec2 size;
	uint32 m_Object;
};