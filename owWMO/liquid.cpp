#include "stdafx.h"

// Includes
#include "Wmo_Material.h"

// General
#include "liquid.h"



//

Liquid::Liquid(uint32_t x, uint32_t y, vec3 base, float tilesize) : m_TilesX(x), m_TilesY(y), pos(base), tilesize(tilesize), shader(-1), ydir(1.0f)
{
	m_TilesCount = (m_TilesX + 1) * (m_TilesY + 1);
}

Liquid::~Liquid()
{
	for (size_t i = 0; i < textures.size(); i++)
	{
		_TexturesMgr->Delete(textures[i]);
	}
}

void Liquid::initFromTerrain(File& f, int flags)
{
	texRepeats = 4.0f;
	/*
	flags:
	8 - ocean
	4 - river
	16 - magma
	*/
	ydir = 1.0f;
	if (flags & 16)
	{
		// magma:
		initTextures("XTextures\\lava\\lava", 1, 30);
		type = 0; // not colored
	}
	else if (flags & 4)
	{
		// river/lake
		initTextures("XTextures\\river\\lake_a", 1, 30); // TODO: rivers etc.?
		type = 2; // dynamic colors
		shader = 0;
	}
	else
	{
		// ocean
		initTextures("XTextures\\ocean\\ocean_h", 1, 30);
		/*
		type = 1; // static color
		col = vec3(0.0f, 0.1f, 0.4f); // TODO: figure out real ocean colors?
		*/
		type = 2;
		shader = 0;
	}
	initGeometry(f);
	trans = false;
}

void Liquid::initFromWMO(File& f, WMOMaterial* mat, bool indoor)
{
	texRepeats = 4.0f;
	ydir = -1.0f;

	initGeometry(f);

	trans = false;

	if (m_LastFlag.liquid & 0x01)
	{
		initTextures("XTextures\\slime\\slime", 1, 30);
		type = 0;
		texRepeats = 2.0f;
	}
	else if (m_LastFlag.liquid & 0x02)
	{
		initTextures("XTextures\\lava\\lava", 1, 30);
		type = 0;
	}
	else
	{
		initTextures("XTextures\\river\\lake_a", 1, 30);
		if (indoor)
		{
			trans = true;
			type = 1;
			col = vec3(((mat->GetDiffuseColor() & 0xFF0000) >> 16) / 255.0f, ((mat->GetDiffuseColor() & 0xFF00) >> 8) / 255.0f, (mat->GetDiffuseColor() & 0xFF) / 255.0f);
			shader = 0;
		}
		else
		{
			type = 2; // outdoor water (...?)
			shader = 0;
		}
	}
}


void Liquid::initGeometry(File& f)
{
	Liquid_Vertex* map = (Liquid_Vertex*)f.GetDataFromCurrent();
	Liquid_Flag* flags = (Liquid_Flag*)(f.GetDataFromCurrent() + m_TilesCount * sizeof(Liquid_Vertex));

	// generate vertices
	vec3* verts = new vec3[m_TilesCount];
	float* col = new float[m_TilesCount];

	for (int j = 0; j < m_TilesY + 1; j++)
	{
		for (int i = 0; i < m_TilesX + 1; i++)
		{
			uint32_t p = j*(m_TilesX + 1) + i;
			float h = map[p].magmaVert.height;
			if (h > 100000)
			{
				h = pos.y;
			}
			verts[p] = vec3(pos.x + tilesize * i, h, pos.z + ydir * tilesize * j);
			col[p] = (map[p].magmaVert.s / 255.0f) * 0.5f + 0.5f;
		}
	}

	m_OGLList = glGenLists(1);
	glNewList(m_OGLList, GL_COMPILE);

	// TODO: handle light/dark liquid colors
	glNormal3f(0, 1, 0);
	glBegin(GL_QUADS);

	// draw tiles
	for (int j = 0; j < m_TilesY; j++)
	{
		for (int i = 0; i < m_TilesX; i++)
		{
			Liquid_Flag f = flags[j * m_TilesX + i];
			if ((f.liquid & 0x08) == 0)
			{
				m_LastFlag = f;

				// 15 seems to be "don't draw"
				uint32_t p = j * (m_TilesX + 1) + i;

				glTexCoord3f(i / texRepeats, j / texRepeats, col[p]);
				glVertex3fv(glm::value_ptr(verts[p]));

				glTexCoord3f((i + 1) / texRepeats, j / texRepeats, col[p + 1]);
				glVertex3fv(glm::value_ptr(verts[p + 1]));

				glTexCoord3f((i + 1) / texRepeats, (j + 1) / texRepeats, col[p + m_TilesX + 1 + 1]);
				glVertex3fv(glm::value_ptr(verts[p + m_TilesX + 1 + 1]));

				glTexCoord3f(i / texRepeats, (j + 1) / texRepeats, col[p + m_TilesX + 1]);
				glVertex3fv(glm::value_ptr(verts[p + m_TilesX + 1]));

			}
		}
	}
	glEnd();

	/*
	// debug triangles:
	//glColor4f(1,1,1,1);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glBegin(GL_TRIANGLES);
	for (int j=0; j<ytiles+1; j++) {
		for (int i=0; i<xtiles+1; i++) {
			uint32_t p = j*(xtiles+1)+i;
			vec3 v = verts[p];
			//short s = *( (short*) (f.GetDataFromCurrent() + p*8) );
			//float f = s / 255.0f;
			//glColor4f(f,(1.0f-f),0,1);
			uint8_t c[4];
			c[0] = 255-map[p].c[3];
			c[1] = 255-map[p].c[2];
			c[2] = 255-map[p].c[1];
			c[3] = map[p].c[0];
			glColor4ubv(c);

			glVertex3fv(v + vec3(-0.5f, 1.0f, 0));
			glVertex3fv(v + vec3(0.5f, 1.0f, 0));
			glVertex3fv(v + vec3(0.0f, 2.0f, 0));
		}
	}
	glEnd();
	glColor4f(1,1,1,1);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	*/

	/*
	// temp: draw outlines
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINE_LOOP);
	vec3 wx = vec3(tilesize*xtiles,0,0);
	vec3 wy = vec3(0,0,tilesize*ytiles*ydir);
	glColor4f(1,0,0,1);
	glVertex3fv(pos);
	glColor4f(1,1,1,1);
	glVertex3fv(pos+wx);
	glVertex3fv(pos+wx+wy);
	glVertex3fv(pos+wy);
	glEnd();
	glEnable(GL_TEXTURE_2D);
	*/

	glEndList();


	delete[] verts;
	delete[] col;
}

void Liquid::draw()
{
	glDisable(GL_CULL_FACE);
	glDepthFunc(GL_LESS);
	size_t texidx = (size_t)(_TimeManager->animtime / 60.0f) % textures.size();
	textures[texidx]->Bind();

	const float tcol = trans ? 0.9f : 1.0f;
	if (trans)
	{
		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);
	}

	if (_Settings->useshaders && (shader >= 0))
	{
		// SHADER-BASED
		vec3 col2;
		//waterShaders[shader]->bind();
		if (type == 2)
		{
			col = _EnvironmentManager->GetSkyColor(RIVER_COLOR_LIGHT);
			col2 = _EnvironmentManager->GetSkyColor(RIVER_COLOR_DARK);
		}
		else
		{
			col2 = col;
		}

		glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 0, col.x, col.y, col.z, tcol);
		glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 1, col2.x, col2.y, col2.z, tcol);

		glCallList(m_OGLList);

		//waterShaders[shader]->unbind();
	}
	else
	{
		// FIXED-FUNCTION

		if (type == 0)
		{
			glColor4f(1, 1, 1, tcol);
		}
		else
		{
			if (type == 2)
			{
				col = _EnvironmentManager->GetSkyColor(RIVER_COLOR_LIGHT); // TODO: add variable water colo
			}
			glColor4f(col.x, col.y, col.z, tcol);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD); // TODO: check if ARB_texture_env_add is supported? :(
		}
		glCallList(m_OGLList);

		if (type != 0) glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	}

	glDepthFunc(GL_LEQUAL);
	glColor4f(1, 1, 1, 1);
	if (trans)
	{
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
	}
}

void Liquid::initTextures(const char* basename, int first, int last)
{
	char buf[256];
	for (int i = first; i <= last; i++)
	{
		sprintf_s(buf, "%s.%d.blp", basename, i);
		textures.push_back(_TexturesMgr->Add(buf));
	}
}




