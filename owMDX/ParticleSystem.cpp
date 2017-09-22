#include "stdafx.h"

// Inlcudes
#include "Model.h"

// General
#include "ParticleSystem.h"

// Additional
#include "ModelsUtils.h"

ParticleSystem::ParticleSystem() : emitter(0), mid(0), rem(0)
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

ParticleSystem::~ParticleSystem()
{
	delete emitter;
}

void ParticleSystem::init(File& f, M2Particle& mta, uint32_t* globals)
{
	speed.init(mta.emissionSpeed, f, globals);
	variation.init(mta.speedVariation, f, globals);

	spread.init(mta.verticalRange, f, globals);
	lat.init(mta.horizontalRange, f, globals);

	gravity.init(mta.gravity, f, globals);
	lifespan.init(mta.lifespan, f, globals);
	rate.init(mta.emissionRate, f, globals);
	areal.init(mta.emissionAreaLength, f, globals);
	areaw.init(mta.emissionAreaWidth, f, globals);
	deacceleration.init(mta.zSource, f, globals);
	enabled.init(mta.enabledIn /*FIXME ???*/, f, globals);

	vec3 colors2[3];
	memcpy(colors2, f.GetData() + mta.colorTrack.values.offset, sizeof(vec3) * 3);
	for (size_t i = 0; i < 3; i++)
	{
		float opacity = *(short*)(f.GetData() + mta.alphaTrack.values.offset + i * 2);
		colors[i] = vec4(colors2[i].x / 255.0f, colors2[i].y / 255.0f, colors2[i].z / 255.0f, opacity / 32767.0f);
		sizes[i] = (*(float*)(f.GetData() + mta.scaleTrack.values.offset + i * 4)) * mta.scales[i];
	}

	mid = 0.5;
	slowdown = mta.slowdown; // FIXME
	rotation = mta.rotation; // FIXME
	pos = fixCoordSystem(mta.Position);
	texture = model->textures[mta.texture];
	blend = mta.blendingType;
	rows = mta.textureDimensions_rows;
	cols = mta.textureDimensions_columns;
	type = mta.particleColorIndex;
	//order = mta.s2;
	order = mta.particleColorIndex > 0 ? -1 : 0;
	parent = model->bones + mta.bone;

	switch (mta.emitterType)
	{
		case 1:
		emitter = new PlaneParticleEmitter(this);
		break;
		case 2:
		emitter = new SphereParticleEmitter(this);
		break;
		case 3: // Spline? (can't be bothered to find one)
		break;
	}

	//transform = mta.flags & 1024;

	// Type 2
	// 3145 = water ele
	// 1305 = water ele
	// 1049 = water elemental
	// 1033 = water elemental
	// 281 = water ele
	// 256 = Water elemental
	// 57 = Faith halo, ring?
	// 9 = water elemental

	billboard = !(mta.flags & M2PARTICLE_FLAGS_DONOTBILLBOARD);


	manim = mtime = 0;
	rem = 0;

	tofs = Random::Generate();

	// init tiles
	for (int i = 0; i < rows*cols; i++)
	{
		TexCoordSet tc;
		initTile(tc.tc, i);
		tiles.push_back(tc);
	}
}

void ParticleSystem::update(float dt)
{
	float grav = gravity.getValue(manim, mtime);
	float deaccel = deacceleration.getValue(manim, mtime);

	// spawn new particles
	if (emitter)
	{
		float frate = rate.getValue(manim, mtime);
		float flife = 1.0f;
		flife = lifespan.getValue(manim, mtime);

		float ftospawn = (dt * frate / flife) + rem;
		if (ftospawn < 1.0f)
		{
			rem = ftospawn;
			if (rem < 0)
				rem = 0;
		}
		else
		{
			int tospawn = (int)ftospawn;

			if ((tospawn + particles.size()) > MAX_PARTICLES) // Error check to prevent the program from trying to load insane amounts of particles.
				tospawn = (int)particles.size() - MAX_PARTICLES;

			rem = ftospawn - (float)tospawn;


			float w = areal.getValue(manim, mtime) * 0.5f;
			float l = areaw.getValue(manim, mtime) * 0.5f;
			float spd = speed.getValue(manim, mtime);
			float var = variation.getValue(manim, mtime);
			float spr = spread.getValue(manim, mtime);
			float spr2 = lat.getValue(manim, mtime);
			bool en = true;
			if (enabled.uses(manim))
				en = enabled.getValue(manim, mtime) != 0;

			//rem = 0;
			if (en)
			{
				for (int i = 0; i < tospawn; i++)
				{
					Particle p = emitter->newParticle(manim, mtime, w, l, spd, var, spr, spr2);
					// sanity check:
					//if (particles.size() < MAX_PARTICLES) // No need to check this every loop iteration. Already checked above.
					particles.push_back(p);
				}
			}
		}
	}

	float mspeed = 1.0f;

	for (ParticleList::iterator it = particles.begin(); it != particles.end(); )
	{
		Particle &p = *it;
		p.speed += p.down * grav * dt - p.dir * deaccel * dt;

		if (slowdown > 0)
		{
			mspeed = expf(-1.0f * slowdown * p.life);
		}
		p.pos += p.speed * mspeed * dt;

		p.life += dt;
		float rlife = p.life / p.maxlife;
		// calculate size and color based on lifetime
		p.size = lifeRamp<float>(rlife, mid, sizes[0], sizes[1], sizes[2]);
		p.color = lifeRamp<vec4>(rlife, mid, colors[0], colors[1], colors[2]);

		// kill off old particles
		if (rlife >= 1.0f)
			particles.erase(it++);
		else
			++it;
	}
}

void ParticleSystem::setup(int anim, int time)
{
	manim = anim;
	mtime = time;

	/*
	if (transform) {
	// transform every particle by the parent trans matrix   - apparently this isn't needed
	Matrix m = parent->mat;
	for (ParticleList::iterator it = particles.begin(); it != particles.end(); ++it) {
	it->tpos = m * it->pos;
	}
	} else {
	for (ParticleList::iterator it = particles.begin(); it != particles.end(); ++it) {
	it->tpos = it->pos;
	}
	}
	*/
}

void ParticleSystem::draw()
{
	switch (blend)
	{
		case 0:
		glDisable(GL_BLEND);
		glDisable(GL_ALPHA_TEST);
		break;

		case 1:
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_COLOR, GL_ONE);

		glDisable(GL_ALPHA_TEST);
		break;

		case 2:
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

		glDisable(GL_ALPHA_TEST);
		break;

		case 3:
		glDisable(GL_BLEND);
		glEnable(GL_ALPHA_TEST);
		break;

		case 4:
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		glDisable(GL_ALPHA_TEST);
		break;
	}

	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	glDepthMask(GL_FALSE);

	texture->Bind();

	vec3 vRight(1, 0, 0);
	vec3 vUp(0, 1, 0);

	// position stuff
	const float f = 1;//0.707106781f; // sqrt(2)/2
	vec3 bv0 = vec3(-f, +f, 0);
	vec3 bv1 = vec3(+f, +f, 0);
	vec3 bv2 = vec3(+f, -f, 0);
	vec3 bv3 = vec3(-f, -f, 0);

	if (billboard)
	{
		float modelview[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

		vRight = vec3(modelview[0], modelview[4], modelview[8]);
		vUp = vec3(modelview[1], modelview[5], modelview[9]);
	}

	/*
	* type:
	* 0	 "normal" particle
	* 1	large quad from the particle's origin to its position (used in Moonwell water effects)
	* 2	seems to be the same as 0 (found some in the Deeprun Tram blinky-lights-sign thing)
	*/
	if (type == 0 || type == 2)
	{
		if (billboard)
		{
			glBegin(GL_QUADS);
			for (auto it = particles.begin(); it != particles.end(); ++it)
			{
				if (tiles.size() - 1 < it->m_TileExists) break;

				const float size = it->size;
				glColor4fv(glm::value_ptr(it->color));

				glTexCoord2fv(glm::value_ptr(tiles[it->m_TileExists].tc[0]));
				glVertex3fv(glm::value_ptr(it->pos - (vRight + vUp) * size));

				glTexCoord2fv(glm::value_ptr(tiles[it->m_TileExists].tc[1]));
				glVertex3fv(glm::value_ptr(it->pos + (vRight - vUp) * size));

				glTexCoord2fv(glm::value_ptr(tiles[it->m_TileExists].tc[2]));
				glVertex3fv(glm::value_ptr(it->pos + (vRight + vUp) * size));

				glTexCoord2fv(glm::value_ptr(tiles[it->m_TileExists].tc[3]));
				glVertex3fv(glm::value_ptr(it->pos - (vRight - vUp) * size));
			}
			glEnd();

		}
		else
		{
			glBegin(GL_QUADS);
			for (auto it = particles.begin(); it != particles.end(); ++it)
			{
				if (tiles.size() - 1 < it->m_TileExists) break;

				glColor4fv(glm::value_ptr(it->color));

				glTexCoord2fv(glm::value_ptr(tiles[it->m_TileExists].tc[0]));
				glVertex3fv(glm::value_ptr(it->pos + it->corners[0] * it->size));

				glTexCoord2fv(glm::value_ptr(tiles[it->m_TileExists].tc[1]));
				glVertex3fv(glm::value_ptr(it->pos + it->corners[1] * it->size));

				glTexCoord2fv(glm::value_ptr(tiles[it->m_TileExists].tc[2]));
				glVertex3fv(glm::value_ptr(it->pos + it->corners[2] * it->size));

				glTexCoord2fv(glm::value_ptr(tiles[it->m_TileExists].tc[3]));
				glVertex3fv(glm::value_ptr(it->pos + it->corners[3] * it->size));
			}
			glEnd();
		}
	}
	else if (type == 1)
	{ // Sphere particles
	  // particles from origin to position
	  /*
	  bv0 = mbb * vec3(0,-1.0f,0);
	  bv1 = mbb * vec3(0,+1.0f,0);


	  bv0 = mbb * vec3(-1.0f,0,0);
	  bv1 = mbb * vec3(1.0f,0,0);
	  */

		glBegin(GL_QUADS);
		for (auto it = particles.begin(); it != particles.end(); ++it)
		{
			if (tiles.size() - 1 < it->m_TileExists) break;


			glColor4fv(glm::value_ptr(it->color));

			glTexCoord2fv(glm::value_ptr(tiles[it->m_TileExists].tc[0]));
			glVertex3fv(glm::value_ptr(it->pos + bv0 * it->size));

			glTexCoord2fv(glm::value_ptr(tiles[it->m_TileExists].tc[1]));
			glVertex3fv(glm::value_ptr(it->pos + bv1 * it->size));

			glTexCoord2fv(glm::value_ptr(tiles[it->m_TileExists].tc[2]));
			glVertex3fv(glm::value_ptr(it->origin + bv1 * it->size));

			glTexCoord2fv(glm::value_ptr(tiles[it->m_TileExists].tc[3]));
			glVertex3fv(glm::value_ptr(it->origin + bv0 * it->size));
		}
		glEnd();

	}

	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);

}

//

void ParticleSystem::initTile(vec2 *tc, int num)
{
	vec2 otc[4];
	vec2 a, b;
	int x = num % cols;
	int y = num / cols;

	a.x = x * (1.0f / cols);
	b.x = (x + 1) * (1.0f / cols);
	a.y = y * (1.0f / rows);
	b.y = (y + 1) * (1.0f / rows);

	otc[0] = a;
	otc[2] = b;
	otc[1].x = b.x;
	otc[1].y = a.y;
	otc[3].x = a.x;
	otc[3].y = b.y;

	for (int i = 0; i < 4; i++)
	{
		tc[(i + 4 - order) & 3] = otc[i];
	}
}