#pragma once

class ShadowMapFBO {
public:
	 ShadowMapFBO();

	 ~ShadowMapFBO();

	 bool Init(uint32_t WindowWidth, uint32_t WindowHeight);

	 void BindForWriting();

	 void BindForReading(GLenum TextureUnit);

private:
	GLuint m_fbo;
	GLuint m_shadowMap;
};