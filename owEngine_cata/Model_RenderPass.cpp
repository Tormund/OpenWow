#include "stdafx.h"

// Include
#include "Model.h"

// General
//#include "Model_RenderPass.h"

// Additional
#include "World.h"

bool ModelRenderPass::init(Model * m)
{
	// May aswell check that we're going to render the geoset before doing all this crap.
	if (m->showGeosets[geoset])
	{

		// COLOUR
		// Get the colour and transparency and check that we should even render
		ocol = Vec4D(1.0f, 1.0f, 1.0f, m->trans);
		ecol = Vec4D(0.0f, 0.0f, 0.0f, 0.0f);

		//if (m->trans == 1.0f)
		//	return false;

		// emissive colors
		if (color != -1 && m->colors[color].color.uses(0))
		{
			vec3 c = m->colors[color].color.getValue(0, m->animtime);
			if (m->colors[color].opacity.uses(m->anim))
			{
				float o = m->colors[color].opacity.getValue(m->anim, m->animtime);
				ocol.w = o;
			}

			if (unlit)
			{
				ocol.x = c.x; ocol.y = c.y; ocol.z = c.z;
			}
			else
			{
				ocol.x = ocol.y = ocol.z = 0;
			}

			ecol = Vec4D(c, ocol.w);
			glMaterialfv(GL_FRONT, GL_EMISSION, ecol);
		}

		// opacity
		if (opacity != -1)
		{
			if (m->transparency[opacity].trans.uses(0))
				ocol.w *= m->transparency[opacity].trans.getValue(0, m->animtime);
		}

		// exit and return false before affecting the opengl render state
		if (!((ocol.w > 0) && (color == -1 || ecol.w > 0)))
		{
			return false;
		}

		// TEXTURE
		// bind to our texture
		Texture* bindtex = 0;
		if (m->specialTextures[tex] == -1)
			bindtex = m->textures[tex];
		else
			bindtex = m->replaceTextures[m->specialTextures[tex]];

		//glBindTexture(GL_TEXTURE_2D, bindtex);
		bindtex->Bind();
		// --

		// TODO: Add proper support for multi-texturing.

		// blend mode
		switch (blendmode)
		{
			case BM_OPAQUE:	// 0
			break;

			case BM_TRANSPARENT: // 1
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GEQUAL, 0.7f);
			break;

			case BM_ALPHA_BLEND: // 2
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;

			case BM_ADDITIVE: // 3
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_COLOR, GL_ONE);
			break;

			case BM_ADDITIVE_ALPHA: // 4
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			break;

			case BM_MODULATE: // 5
			glEnable(GL_BLEND);
			glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);
			break;

			case BM_MODULATE2: // 6
			glEnable(GL_BLEND);
			glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);
			break;

			default:
			Debug::Info("Error: Unknown blendmode: %d", blendmode);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		//if (cull)
		//	glEnable(GL_CULL_FACE);

		// Texture wrapping around the geometry
		if (swrap)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

		if (twrap)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// no writing to the depth buffer.
		if (noZWrite)
			glDepthMask(GL_FALSE);

		if (unlit)
		{
			glDisable(GL_LIGHTING);
			// unfogged = unlit?
			glDisable(GL_FOG);
		}

		// Environmental mapping, material, and effects
		if (useEnvMap)
		{
			// Turn on the 'reflection' shine, using 18.0f as that is what WoW uses based on the reverse engineering
			// This is now set in InitGL(); - no need to call it every render.
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 18.0f);

			// env mapping
			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);

			const GLint maptype = GL_SPHERE_MAP;
			//const GLint maptype = GL_REFLECTION_MAP_ARB;

			glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, maptype);
			glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, maptype);
		}

		if (texanim != -1)
		{
			glMatrixMode(GL_TEXTURE);
			glPushMatrix();

			m->texAnims[texanim].setup(texanim);
		}

		// color
		glColor4fv(ocol);
		//glMaterialfv(GL_FRONT, GL_SPECULAR, ocol);

		// don't use lighting on the surface
		if (unlit)
			glDisable(GL_LIGHTING);

		if (blendmode <= 1 && ocol.w < 1.0f)
			glEnable(GL_BLEND);

		return true;
	}

	return false;
}

void ModelRenderPass::deinit()
{
	switch (blendmode)
	{
		case BM_OPAQUE:
		break;

		case BM_TRANSPARENT:
		break;

		case BM_ALPHA_BLEND:
		//glDepthMask(GL_TRUE);
		break;

		default:
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // default blend func
		break;
	}

	if (noZWrite)
	{
		glDepthMask(GL_TRUE);
	}

	if (texanim != -1)
	{
		glPopMatrix(); // BOUZI
		glMatrixMode(GL_MODELVIEW);
	}

	if (unlit)
	{
		glEnable(GL_LIGHTING);
		if (_World && _World->drawfog) glEnable(GL_FOG);
	}

	if (useEnvMap)
	{
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
	}

	if (usetex2)
	{
		glDisable(GL_TEXTURE_2D);
		glActiveTextureARB(GL_TEXTURE0);
	}
	//glColor4f(1,1,1,1); //???
}