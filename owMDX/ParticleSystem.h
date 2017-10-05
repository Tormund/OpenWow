#pragma once

// Includes
#include "Particle.h"

// Typedefs
typedef list<Particle> ParticleList;

// Defines
#define MAX_PARTICLES 10000

// Classes
class MDX_Part_Bone;
class MDX;
class ParticleSystem;

class ParticleEmitter
{
public:
	ParticleEmitter(ParticleSystem *sys) : sys(sys) {}
	virtual Particle newParticle(int anim, int time, float w, float l, float spd, float var, float spr, float spr2) = 0;

protected:
	ParticleSystem* sys;
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

//

struct TexCoordSet
{
	vec2 tc[4];
};

class ParticleSystem
{
	friend class PlaneParticleEmitter;
	friend class SphereParticleEmitter;
public:
	ParticleSystem();
	~ParticleSystem();

	void init(File& f, M2Particle& mta, uint32 * globals);
	void update(float dt);
	void setup(int anim, int time);
	void draw();

public:
	MDX* model;
	float tofs;

private:
	void initTile(vec2 *tc, int num);

private:
	Animated<float> speed, variation, spread, lat, gravity, lifespan, rate, areal, areaw, deacceleration;
	Animated<uint8> enabled;
	vec4 colors[3];
	float sizes[3];
	float mid, slowdown, rotation;
	vec3 pos;
	Texture* texture;
	ParticleEmitter* emitter;
	ParticleList particles;
	int blend, order, type;
	int manim, mtime;
	int rows, cols;
	vector<TexCoordSet> tiles;

	bool billboard;

	float rem;
	//bool transform;

	// unknown parameters omitted for now ...
	int32 flags;
	int16 pType;

	MDX_Part_Bone* parent;

};

template<class T>
inline T lifeRamp(float life, float mid, const T &a, const T &b, const T &c)
{
	if (life <= mid)
		return interpolate<T>(life / mid, a, b);
	else
		return interpolate<T>((life - mid) / (1.0f - mid), b, c);
}