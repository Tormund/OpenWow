#pragma once

class World
{
	CLASS_INSTANCE(World);

public:
	World();
	~World();

	void initDisplay();

	void tick(float dt);
	void drawShader(GLint _color);
	void RenderGeom();
	void RenderPostprocess();

	void DSDirectionalLightPass(DirectionalLight& _light);
	void DSSimpleRenderPass();

public:
	// Fog params
	float l_const, l_linear, l_quadratic;

public:
	GBuffer* m_gbuffer;
	GLuint finalTexture1;
	GLuint finalTexture2;

	Camera* mainCamera;
	Camera* testCamera;
};

#define _World World::instance()
