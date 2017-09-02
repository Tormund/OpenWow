#pragma once

class ShadowMapFBO {
public:
	OW_RENDER_DLL_API ShadowMapFBO();

	OW_RENDER_DLL_API ~ShadowMapFBO();

	OW_RENDER_DLL_API bool Init(unsigned int WindowWidth, unsigned int WindowHeight);

	OW_RENDER_DLL_API void BindForWriting();

	OW_RENDER_DLL_API void BindForReading(GLenum TextureUnit);

private:
	GLuint m_fbo;
	GLuint m_shadowMap;
};