#pragma once

class GBuffer {
public:
	enum GBUFFER_TEXTURE_TYPE {
		GBUFFER_TEXTURE_TYPE_POSITION,
		GBUFFER_TEXTURE_TYPE_DIFFUSE,
		GBUFFER_TEXTURE_TYPE_NORMAL,
		GBUFFER_TEXTURE_TYPE_SPECULAR
	};

	 GBuffer();
	 ~GBuffer();

	 bool Init(unsigned int WindowWidth, unsigned int WindowHeight);

	 void StartFrame();
	 void BindForGeomPass();
	 void BindForStencilPass();
	 void BindForLightPass();
	 void BindForFinalPass(GLint _color = 0x8CE5);

private:
	GLuint gBuffer;
	GLuint textures[4]; // Position, diffuse, normal, specular
	/*GLuint texturePosition;
	GLuint textureDiffuse;
	GLuint textureNormal;
	GLuint textureSpecular;*/
	GLuint depthTexture; // Depth

	GLuint finalTexture;
};