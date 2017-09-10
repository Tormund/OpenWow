#pragma once

class ShadowMapFBO {
public:
	OW_RENDER_DLL_API ShadowMapFBO();

	OW_RENDER_DLL_API ~ShadowMapFBO();

	OW_RENDER_DLL_API bool Init(uint32_t WindowWidth, uint32_t WindowHeight);

	OW_RENDER_DLL_API void BindForWriting();

	OW_RENDER_DLL_API void BindForReading(GLenum TextureUnit);

private:
	GLuint m_fbo;
	GLuint m_shadowMap;
};