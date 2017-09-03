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

void ParticleSystem::init(File& f, ModelParticleEmitterDef& mta, uint32_t * globals)
{
	speed.init(mta.EmissionSpeed, f, globals);
	variation.init(mta.SpeedVariation, f, globals);
	spread.init(mta.VerticalRange, f, globals);
	lat.init(mta.HorizontalRange, f, globals);
	gravity.init(mta.Gravity, f, globals);
	lifespan.init(mta.Lifespan, f, globals);
	rate.init(mta.EmissionRate, f, globals);
	areal.init(mta.EmissionAreaLength, f, globals);
	areaw.init(mta.EmissionAreaWidth, f, globals);
	deacceleration.init(mta.Gravity2, f, globals);
	enabled.init(mta.en, f, globals);

	vec3 colors2[3];
	memcpy(colors2, f.GetData() + mta.p.colors.ofsKeys, sizeof(vec3) * 3);
	for (size_t i = 0; i < 3; i++)
	{
		float opacity = *(short*)(f.GetData() + mta.p.opacity.ofsKeys + i * 2);
		colors[i] = vec4(colors2[i].x / 255.0f, colors2[i].y / 255.0f, colors2[i].z / 255.0f, opacity / 32767.0f);
		sizes[i] = (*(float*)(f.GetData() + mta.p.sizes.ofsKeys + i * 4))*mta.p.scales[i];
	}

	mid = 0.5;
	slowdown = mta.p.slowdown;
	rotation = mta.p.rotation;
	pos = fixCoordSystem(mta.pos);
	texture = model->textures[mta.texture];
	blend = mta.blend;
	rows = mta.rows;
	cols = mta.cols;
	type = mta.ParticleType;
	//order = mta.s2;
	order = mta.ParticleType > 0 ? -1 : 0;
	parent = model->bones + mta.bone;

	switch (mta.EmitterType)
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

	billboard = !(mta.flags & MODELPARTICLE_DONOTBILLBOARD);


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
	/*
	// just draw points:
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glColor4f(1,1,1,1);
	glBegin(GL_POINTS);
	for (ParticleList::iterator it = particles.begin(); it != particles.end(); ++it) {
	glVertex3fv(it->tpos);
	}
	glEnd();
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	*/
	//blend = 1;
	// setup blend mode
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
		// originally
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
		// Changed blending mode to this in 0.5.08, seems to fix a few of the render problems
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
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

		default:
		break;
	}

	//glDisable(GL_LIGHTING);
	//glDisable(GL_CULL_FACE);
	//glDepthMask(GL_FALSE);

	texture->Bind();

	/*
	if (supportPointSprites && rows==1 && cols==1) {
	// This is how will our point sprite's size will be modified by
	// distance from the viewer
	float quadratic[] = {0.1f, 0.0f, 0.5f};
	//float quadratic[] = {0.88f, 0.001f, 0.000004f};
	glPointParameterfvARB( GL_POINT_DISTANCE_ATTENUATION_ARB, quadratic);

	// Query for the max point size supported by the hardware
	float maxSize = 512.0f;
	//glGetFloatv(GL_POINT_SIZE_MAX_ARB, &maxSize );

	// Clamp size to 100.0f or the sprites could get a little too big on some
	// of the newer graphic cards. My ATI card at home supports a max point
	// size of 1024.0f!
	//if( maxSize > 100.0f )
	//	maxSize = 100.0f;

	glPointSize(maxSize);

	// The alpha of a point is calculated to allow the fading of points
	// instead of shrinking them past a defined threshold size. The threshold
	// is defined by GL_POINT_FADE_THRESHOLD_SIZE_ARB and is not clamped to
	// the minimum and maximum point sizes.
	glPointParameterfARB(GL_POINT_FADE_THRESHOLD_SIZE_ARB, 60.0f);

	glPointParameterfARB(GL_POINT_SIZE_MIN_ARB, 1.0f );
	glPointParameterfARB(GL_POINT_SIZE_MAX_ARB, maxSize );

	// Specify point sprite texture coordinate replacement mode for each texture unit
	glTexEnvf(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
	// Render point sprites...
	glEnable(GL_POINT_SPRITE_ARB);

	glBegin(GL_POINTS);
	{
	for (ParticleList::iterator it = particles.begin(); it != particles.end(); ++it) {
	glPointSize(it->size);
	glTexCoord2fv(tiles[it->m_TileExists].tc[0]);
	glColor4fv(it->color);
	glVertex3fv(it->pos);
	}
	}
	glEnd();

	glDisable(GL_POINT_SPRITE_ARB);
	glTexEnvf(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_FALSE);

	} else { // Old slow method */

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
		vUp = vec3(modelview[1], modelview[5], modelview[9]); // Spherical billboarding
															  //vUp = vec3(0,1,0); // Cylindrical billboarding
	}
	/*
	* type:
	* 0	 "normal" particle
	* 1	large quad from the particle's origin to its position (used in Moonwell water effects)
	* 2	seems to be the same as 0 (found some in the Deeprun Tram blinky-lights-sign thing)
	*/
	if (type == 0 || type == 2)
	{
		// TODO: figure out type 2 (deeprun tram subway sign)
		// - doesn't seem to be any different from 0 -_-
		// regular particles

		if (billboard)
		{
			glBegin(GL_QUADS);
			// TODO: per-particle rotation in a non-expensive way?? :|
			for (ParticleList::iterator it = particles.begin(); it != particles.end(); ++it)
			{
				if (tiles.size() - 1 < it->m_TileExists) // Alfred, 2009.08.07, error prevent
					break;
				const float size = it->size;// / 2;
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
			for (ParticleList::iterator it = particles.begin(); it != particles.end(); ++it)
			{
				if (tiles.size() - 1 < it->m_TileExists) // Alfred, 2009.08.07, error prevent
					break;
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
		for (ParticleList::iterator it = particles.begin(); it != particles.end(); ++it)
		{
			if (tiles.size() - 1 < it->m_TileExists) // Alfred, 2009.08.07, error prevent
				break;
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
	//}

	//glEnable(GL_LIGHTING);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glDepthMask(GL_TRUE);
	//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
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