#pragma once

class gMapDBRecord;

class World
{
	CLASS_INSTANCE(World);

public:
	World();
	~World();

	void initDisplay();

	void tick(float dt);
	void draw(GLint _color);
	void drawShader(GLint _color);

	void DSGeometryPassBegin();
	void DSGeometryPassEnd();

	void DSDirectionalLightPass(DirectionalLight& _light);
	void DSSimpleRenderPass();

public:
	// Fog params
	float l_const, l_linear, l_quadratic;

public:
	GBuffer* m_gbuffer;
};

#define _World World::instance()
