#include "../stdafx.h"

// General
#include "MDX.h"

// Additional
#include "Model_RenderPass.h"

MDX::MDX(cstring name) : RefItemNamed(name), m_Loaded(false)
{
	//Debug::Info("MDX[%s]: Loading...", m_ModelFileName.c_str());
	
	// Replace .MDX with .M2
	m_ModelFileName = name;
	if (m_ModelFileName.back() != '2')
	{
		m_ModelFileName[m_ModelFileName.length() - 2] = '2';
		m_ModelFileName[m_ModelFileName.length() - 1] = '\0';
		m_ModelFileName.resize(m_ModelFileName.length() - 1);
	}

	m_ModelName = m_ModelFileName.substr(0, m_ModelFileName.length() - 3);

	//

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		specialTextures[i] = -1;
		replaceTextures[i] = 0;
		useReplaceTextures[i] = false;
	}

	m_GlobalLoops = nullptr;
	animtime = 0;
	m_CurrentAnimationIndex = 0;
	colors = nullptr;
	m_Lights = nullptr;
	transparency = nullptr;

#ifdef MDX_PARTICLES_ENABLE
	particleSystems = nullptr;
	ribbons = nullptr;
#endif
}

MDX::~MDX()
{
	if (!m_Loaded)
	{
		return;
	}

	Debug::Info("MDX[%s]: Unloading...", m_ModelFileName.c_str());

	if (header.textures.size)
	{
		for (uint32 i = 0; i < header.textures.size; i++)
		{
			if (textures[i] != nullptr)
			{
				_TexturesMgr->Delete(textures[i]);
			}
		}
		delete[] textures;
	}

	delete[] m_GlobalLoops;

	if (colors) delete[] colors;
	if (transparency) delete[] transparency;


	delete[] m_Vertices;
	delete[] m_Texcoords;
	delete[] m_Normals;



	if (animated)
	{
		// unload all sorts of crap
		delete[] m_Sequences;
		delete[] m_OriginalVertexes;
		if (animBones)
		{
			delete[] m_Part_Bones;
		}

		//if (!animGeometry)
		//{
		//	glDeleteBuffersARB(1, &nbuf);
		//}
		//glDeleteBuffersARB(1, &vbuf);
		//glDeleteBuffersARB(1, &tbuf);

		if (animTextures) delete[] m_TexturesAnims;
		
		
		if (m_Lights) delete[] m_Lights;

#ifdef MDX_PARTICLES_ENABLE
		if (particleSystems) delete[] particleSystems;
		if (ribbons) delete[] ribbons;
#endif
	}
	else
	{
		//glDeleteLists(dlist, 1);
	}
}

//

void MDX::Init(bool forceAnim)
{
	File f = m_ModelFileName;
	if (!f.Open())
	{
		Debug::Info("MDX[%s]: Unable to open file.", m_ModelFileName.c_str());
		return;
	}

	// Header
	memcpy(&header, f.GetData(), sizeof(ModelHeader));

	m_Bounds.set(header.bounding_box.min, header.bounding_box.max, true);

	// Is animated
	animated = isAnimated(f) || forceAnim;

	if (header.global_loops.size)
	{
		m_GlobalLoops = new uint32[header.global_loops.size];
		memcpy(m_GlobalLoops, (f.GetData() + header.global_loops.offset), sizeof(M2Loop) * header.global_loops.size);
	}

	if (animated)
	{
		initAnimated(f);
	}
	else
	{
		initStatic(f);
	}

	m_Loaded = true;
}



void MDX::initCommon(File& f)
{
	// assume: m_OriginalVertexes already set
	//if (!animGeometry)
	//{
	m_Vertices = new vec3[header.vertices.size];
	m_Texcoords = new vec2[header.vertices.size];
	m_Normals = new vec3[header.vertices.size];
	//}

	// vertices, m_Normals
	for (uint32 i = 0; i < header.vertices.size; i++)
	{
		m_OriginalVertexes[i].pos = From_XYZ_To_XZminusY_RET(m_OriginalVertexes[i].pos);
		m_OriginalVertexes[i].normal = From_XYZ_To_XZminusY_RET(m_OriginalVertexes[i].normal);

		//if (!animGeometry)
		//{
		m_Vertices[i] = m_OriginalVertexes[i].pos;
		m_Texcoords[i] = m_OriginalVertexes[i].tex_coords[0];
		m_Normals[i] = m_OriginalVertexes[i].normal.normalized();
		//}
	}

	// textures
	if (header.textures.size)
	{
		textures = new Texture*[header.textures.size];
		texdef = (M2Texture*)(f.GetData() + header.textures.offset);

		char texname[256];
		for (uint32 i = 0; i < header.textures.size; i++)
		{
			// Error check
			if (i > TEXTURE_MAX - 1)
			{
				Debug::Error("MDX[%s]: Model Texture [%d] over [%d]", m_ModelName, header.textures.size, TEXTURE_MAX);
				break;
			}

			if (texdef[i].type == 0)
			{
				strncpy_s(texname, (const char*)(f.GetData() + texdef[i].filename.offset), texdef[i].filename.size);
				texname[texdef[i].filename.size] = 0;
				textures[i] = _TexturesMgr->Add(texname);
			}
			else
			{
				// special texture - only on characters and such...
				textures[i] = 0;
				specialTextures[i] = texdef[i].type;

				if (texdef[i].type < TEXTURE_MAX)
					useReplaceTextures[texdef[i].type] = true;

				if (texdef[i].type == 3)
				{
					// a fix for weapons with type-3 textures.
					replaceTextures[texdef[i].type] = _TexturesMgr->Add("Item\\ObjectComponents\\Weapon\\ArmorReflect4.BLP");
				}
			}
		}
	}

	// init colors
	if (header.colors.size)
	{
		colors = new MDX_Part_Color[header.colors.size];
		M2Color* colorDefs = (M2Color*)(f.GetData() + header.colors.offset);
		for (uint32 i = 0; i < header.colors.size; i++)
		{
			colors[i].init(f, colorDefs[i], m_GlobalLoops);
		}
	}

	// init transparency
	if (header.texture_weights.size)
	{
		transLookup = (uint16*)(f.GetData() + header.transparency_lookup_table.offset);

		transparency = new MDX_Part_Transparency[header.texture_weights.size];
		M2TextureWeight* trDefs = (M2TextureWeight*)(f.GetData() + header.texture_weights.offset);
		for (uint32 i = 0; i < header.texture_weights.size; i++)
		{
			transparency[i].init(f, trDefs[i], m_GlobalLoops);
		}
	}

	// just use the first LOD/view
	if (header.num_skin_profiles > 0)
	{
		File skinFile = m_ModelName + "00.skin";

		Model_Skin* skin = new Model_Skin(this, f, skinFile);

		m_Skins.push_back(skin);
	}


	// Buffer
	glGenBuffers(1, &globalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, globalBuffer);

	glBufferData(GL_ARRAY_BUFFER, header.vertices.size * 8 * sizeof(float), NULL, GL_DYNAMIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, header.vertices.size * 0 * sizeof(float), header.vertices.size * sizeof(vec3), m_Vertices);
	glBufferSubData(GL_ARRAY_BUFFER, header.vertices.size * 3 * sizeof(float), header.vertices.size * sizeof(vec2), m_Texcoords);
	glBufferSubData(GL_ARRAY_BUFFER, header.vertices.size * 5 * sizeof(float), header.vertices.size * sizeof(vec3), m_Normals);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//

void MDX::drawModel()
{
	for (auto it = m_Skins.begin(); it != m_Skins.end(); ++it)
	{
		(*it)->Draw();
	}
	// done with all render ops

	//--------------GLfloat czero[4] = {0,0,0,1};
	//--------------glMaterialfv(GL_FRONT, GL_EMISSION, czero);
	//--------------glColor4f(1, 1, 1, 1);
	//--------------glDepthMask(GL_TRUE);
}

void MDX::draw()
{
	if (!m_Loaded)
	{
		return;
	}

	if (!animated)
	{
		drawModel();
	}
	else
	{
		if (m_IsBillboard)
		{
			animate(0);
		}
		else
		{
			if (!animcalc)
			{
				animate(0);
				animcalc = true;
			}
		}

		//--------------lightsOn(GL_LIGHT4);
		drawModel();
		//--------------lightsOff(GL_LIGHT4);

		// effects are unfogged..?
		//--------------glDisable(GL_FOG);

		// draw particle systems
#ifdef MDX_PARTICLES_ENABLE
		for (uint32 i = 0; i < header.particle_emitters.size; i++)
		{
			particleSystems[i].draw();
		}

		// draw ribbons
		for (uint32 i = 0; i < header.ribbon_emitters.size; i++)
		{
			ribbons[i].draw();
		}
#endif

		//--------------if (Settings::drawfog)
		//--------------{
		//--------------	glEnable(GL_FOG);
		//--------------}
	}
}

//

void MDX::drawShaderBegin()
{
	glBindBuffer(GL_ARRAY_BUFFER, globalBuffer);

	// Vertex
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Texture
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)(header.vertices.size * 3 * sizeof(float)));

	// Normal
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)(header.vertices.size * 5 * sizeof(float)));
}

void MDX::drawShaderEnd()
{
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

//

void MDX::lightsOn(GLuint lbase)
{
	/*if (!m_Loaded)
	{
		return;
	}*/

	// setup lights
	for (uint32 i = 0, l = lbase; i < header.lights.size; i++)
	{
		m_Lights[i].setup(animtime, l++);
	}
}

void MDX::lightsOff(GLuint lbase)
{
	/*if (!m_Loaded)
	{
		return;
	}*/

	for (uint32 i = 0, l = lbase; i < header.lights.size; i++)
	{
		glDisable(l++);
	}
}

void MDX::updateEmitters(float dt)
{
	if (!m_Loaded)
	{
		return;
	}

#ifdef MDX_PARTICLES_ENABLE
	for (uint32 i = 0; i < header.particle_emitters.size; i++)
	{
		particleSystems[i].update(dt);
	}
#endif
}
