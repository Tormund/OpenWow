#include "../stdafx.h"

// Include
#include "MDX.h"

// General
#include "Model_RenderPass.h"

bool ModelRenderPass::init(MDX* m)
{
	// COLOUR
	// Get the colour and transparency and check that we should even render
	ocol = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	ecol = vec4(0.0f, 0.0f, 0.0f, 0.0f);

	//if (m->trans == 1.0f)
	//	return false;

	// emissive colors
	if (color != -1 && m->colors[color].color.uses(0))
	{
		vec3 c = m->colors[color].color.getValue(0, m->animtime);
		if (m->colors[color].opacity.uses(m->m_CurrentAnimationIndex))
		{
			float o = m->colors[color].opacity.getValue(m->m_CurrentAnimationIndex, m->animtime);
			ocol.w = o;
		}

		if (unlit)
		{
			ocol.x = c.x;
			ocol.y = c.y;
			ocol.z = c.z;
		}
		else
		{
			ocol.x = ocol.y = ocol.z = 0;
		}

		ecol = vec4(c, ocol.w);
	//***	glMaterialfv(GL_FRONT, GL_EMISSION, glm::value_ptr(ecol));
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
	Texture* bindtex = nullptr;
	if (m->specialTextures[tex] == -1)
		bindtex = m->textures[tex];
	else
		bindtex = m->replaceTextures[m->specialTextures[tex]];

	if (bindtex != nullptr)
	{
		bindtex->Bind();
		bindtex->Bind(5);
	}

	// --

	// TODO: Add proper support for multi-texturing.

	// blend mode
	/*switch (blendmode)
	{
		case BM_OPAQUE:	// 0
		glDisable(GL_BLEND);
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
		Modules::log().Info("Error: Unknown blendmode: %d", blendmode);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}*/

	if (cull)
	{
		glEnable(GL_CULL_FACE);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}

	// Texture wrapping around the geometry
	/*if (swrap)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	}

	if (twrap)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}*/

	// no writing to the depth buffer.
	glDepthMask(noZWrite ? GL_FALSE : GL_TRUE);

	/*if (texanim != -1)
	{
		glMatrixMode(GL_TEXTURE);
		glPushMatrix();

		m->m_TexturesAnims[texanim].setup(texanim);
	}*/

	// color
	//**glColor4fv(glm::value_ptr(ocol));
	//glMaterialfv(GL_FRONT, GL_SPECULAR, ocol);


	if (blendmode <= 1 && ocol.w < 1.0f)
	{
		glEnable(GL_BLEND);
	}

	return true;
}

void ModelRenderPass::deinit()
{
	/*switch (blendmode)
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
	}*/

	if (noZWrite)
	{
		glDepthMask(GL_TRUE);
	}

	/*if (texanim != -1)
	{
		glPopMatrix(); // BOUZI
		glMatrixMode(GL_MODELVIEW);
	}*/

	if (unlit)
	{
	//***	glEnable(GL_LIGHTING);
	//***	if (Modules::config().drawfog) glEnable(GL_FOG);
	}

	if (useEnvMap)
	{
	//***	glDisable(GL_TEXTURE_GEN_S);
	//***	glDisable(GL_TEXTURE_GEN_T);
	}

	if (usetex2)
	{
	//***	glDisable(GL_TEXTURE_2D);
	//***	glActiveTextureARB(GL_TEXTURE0);
	}
}