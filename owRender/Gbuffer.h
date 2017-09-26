#pragma once

#define COLOR_TEXTURE_UNIT              GL_TEXTURE0
#define COLOR_TEXTURE_UNIT_INDEX        0

#define SHADOW_TEXTURE_UNIT             GL_TEXTURE1
#define SHADOW_TEXTURE_UNIT_INDEX       1

#define NORMAL_TEXTURE_UNIT             GL_TEXTURE2
#define NORMAL_TEXTURE_UNIT_INDEX       2

#define RANDOM_TEXTURE_UNIT             GL_TEXTURE3
#define RANDOM_TEXTURE_UNIT_INDEX       3

#define DISPLACEMENT_TEXTURE_UNIT       GL_TEXTURE4
#define DISPLACEMENT_TEXTURE_UNIT_INDEX 4

#define SPECULAR_TEXTURE_UNIT           GL_TEXTURE5
#define SPECULAR_TEXTURE_UNIT_INDEX     5

class GBuffer
{
public:
	enum GBUFFER_TEXTURE_TYPE
	{
		GBUFFER_TEXTURE_TYPE_POSITION = 0,
		GBUFFER_TEXTURE_TYPE_NORMAL = 1,

		GBUFFER_TEXTURE_TYPE_AMBIENT = 2,
		GBUFFER_TEXTURE_TYPE_DIFFUSE = 3,
		GBUFFER_TEXTURE_TYPE_SPECULAR = 4,
		GBUFFER_TEXTURE_TYPE_SPECULAR_SHININESS = 5
	};

	GBuffer();
	~GBuffer();

	bool Init(unsigned int WindowWidth, unsigned int WindowHeight);

	void StartFrame();
	void BindForGeomPass();

	void Clear();

	void BindForStencilPass();
	void BindForLightPass();
	void BindForFinalPass(GLint _color = GL_COLOR_ATTACHMENT6);

private:
	GLuint gBuffer;

	GLuint textures[6];
	GLuint depthTexture; // Depth

	GLuint finalTexture;
};