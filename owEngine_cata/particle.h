#pragma once

class ParticleSystem;
class RibbonEmitter;

#include "model.h"
#include "animated.h"

#include <list>

struct Particle
{
	vec3 pos, speed, down, origin, dir;
	vec3	corners[4];
	//vec3 tpos;
	float size, life, maxlife;
	uint32_t m_TileExists;
	Vec4D color;
};

typedef list<Particle> ParticleList;

class ParticleEmitter
{
protected:
	ParticleSystem *sys;
public:
	ParticleEmitter(ParticleSystem *sys) : sys(sys) {}
	virtual Particle newParticle(int anim, int time, float w, float l, float spd, float var, float spr, float spr2) = 0;
};

class PlaneParticleEmitter : public ParticleEmitter
{
public:
	PlaneParticleEmitter(ParticleSystem *sys) : ParticleEmitter(sys) {}
	Particle newParticle(int anim, int time, float w, float l, float spd, float var, float spr, float spr2);
};

class SphereParticleEmitter : public ParticleEmitter
{
public:
	SphereParticleEmitter(ParticleSystem *sys) : ParticleEmitter(sys) {}
	Particle newParticle(int anim, int time, float w, float l, float spd, float var, float spr, float spr2);
};

struct TexCoordSet
{
	vec2 tc[4];
};

class ParticleSystem
{
	Animated<float> speed, variation, spread, lat, gravity, lifespan, rate, areal, areaw, deacceleration;
	Animated<uint8_t> enabled;
	Vec4D colors[3];
	float sizes[3];
	float mid, slowdown, rotation;
	vec3 pos;
	Texture* texture;
	ParticleEmitter *emitter;
	ParticleList particles;
	int blend, order, type;
	int manim, mtime;
	int rows, cols;
	vector<TexCoordSet> tiles;
	void initTile(vec2 *tc, int num);
	bool billboard;

	float rem;
	//bool transform;

	// unknown parameters omitted for now ...
	int32_t flags;
	int16_t pType;

	Bone *parent;

public:
	Model *model;
	float tofs;

	ParticleSystem() : emitter(0), mid(0), rem(0)
	{
		blend = 0;
		order = 0;
		type = 0;
		manim = 0;
		mtime = 0;
		rows = 0;
		cols = 0;

		model = 0;
		parent = 0;
		texture = 0;

		slowdown = 0;
		rotation = 0;
		tofs = 0;
	}
	~ParticleSystem() { delete emitter; }

	void init(File &f, ModelParticleEmitterDef &mta, uint32_t *globals);
	void update(float dt);

	void setup(int anim, int time);
	void draw();

	friend class PlaneParticleEmitter;
	friend class SphereParticleEmitter;
};


struct RibbonSegment
{
	vec3 pos, up, back;
	float len, len0;
};

class RibbonEmitter
{
public:
	Model *model;

	void init(File &f, ModelRibbonEmitterDef &mta, uint32_t *globals);
	void setup(int anim, int time);
	void draw();

private:
	Animated<vec3> color;
	AnimatedShort opacity;
	Animated<float> above, below;

	Bone *parent;
	float f1, f2;

	vec3 pos;

	int manim, mtime;
	float length, seglen;
	int numsegs;

	vec3 tpos;
	Vec4D tcolor;
	float tabove, tbelow;

	Texture* texture;

	list<RibbonSegment> segs;
};