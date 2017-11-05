#pragma once

class World
{
	CLASS_INSTANCE(World);

public:
	World();

	void tick(float dt);
	void Render();
	void RenderGeom();
	void RenderPostprocess();

	void DSDirectionalLightPass(DirectionalLight& _light);
	void DSSimpleRenderPass();

public:
	// Fog params
	float l_const, l_linear, l_quadratic;

public:
	Camera* testCamera;

	uint32 rb2;
};

#define _World World::instance()
