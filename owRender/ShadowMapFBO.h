#pragma once

class ShadowMapFBO {
public:
	 ShadowMapFBO();

	 ~ShadowMapFBO();

	 bool Init(uint32 WindowWidth, uint32 WindowHeight);

	 void BindForWriting();

	 void BindForReading(GLenum TextureUnit);

private:
	GLuint m_fbo;
	GLuint m_shadowMap;
};