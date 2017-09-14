#include "stdafx.h"

// General
#include "Model_TextureAnim.h"


void ModelTextureAnim::init(File& f, M2TextureTransform& mta, uint32_t * global)
{
	trans.init(mta.translation, f, global);
	roll.init(mta.rotation, f, global);
	scale.init(mta.scaling, f, global);
}

void ModelTextureAnim::calc(int anim, int time)
{
	if (trans.uses(anim))
	{
		tval = trans.getValue(anim, time);
	}

	if (roll.uses(anim))
	{
		rval = roll.getValue(anim, time);
	}

	if (scale.uses(anim))
	{
		sval = scale.getValue(anim, time);
	}
}

void ModelTextureAnim::setup(int anim)
{
	glLoadIdentity();

	if (trans.uses(anim))
	{
		glTranslatef(tval.x, tval.y, tval.z);
	}

	if (roll.uses(anim))
	{
		glRotatef(rval.x, 0, 0, 1.0f); // this is wrong, I have no idea what I'm doing here ;)
	}

	if (scale.uses(anim))
	{
		glScalef(sval.x, sval.y, sval.z);
	}
}

