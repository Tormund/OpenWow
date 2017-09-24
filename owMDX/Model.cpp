#include "stdafx.h"

// General
#include "model.h"

// Additional
#include "Model_RenderPass.h"
#include "ModelsUtils.h"

int globalTime = 0;

Model::Model(cstring name) : RefItemNamed(name)
{
	ok = false;
}

Model::~Model()
{
	if (ok)
	{
		//Debug::Info("Unloading model %s", name.c_str());

		if (header.textures.size)
		{
			for (size_t i = 0; i < header.textures.size; i++)
			{
				if (textures[i] != nullptr)
				{
					_TexturesMgr->Delete(textures[i]);
				}
			}
			delete[] textures;
		}

		delete[] globalSequences;
		delete[] showGeosets;

		if (animated)
		{
			// unload all sorts of crap
			//delete[] vertices;
			//delete[] normals;
			delete[] indices;
			delete[] anims;
			delete[] origVertices;
			if (animBones)
			{
				delete[] bones;
			}

			if (!animGeometry)
			{
				glDeleteBuffersARB(1, &nbuf);
			}
			glDeleteBuffersARB(1, &vbuf);
			glDeleteBuffersARB(1, &tbuf);

			if (animTextures) delete[] texAnims;
			if (colors) delete[] colors;
			if (transparency) delete[] transparency;
			if (lights) delete[] lights;

			if (particleSystems) delete[] particleSystems;
			if (ribbons) delete[] ribbons;

		}
		else
		{
			glDeleteLists(dlist, 1);
		}
	}
}

void Model::Init(bool forceAnim)
{
	m_ModelFileName = GetName();

	Debug::Info("MDX[%s]: Loading model.", m_ModelFileName.c_str());

	// Replace .MDX with .M2
	if (m_ModelFileName.back() != '2')
	{
		//m_ModelFileName.resize(m_ModelFileName.length() - 1, 0x00);
		m_ModelFileName[m_ModelFileName.length() - 2] = '2';
		m_ModelFileName[m_ModelFileName.length() - 1] = '\0';
		m_ModelFileName.resize(m_ModelFileName.length() - 1);
	}

	m_ModelName = m_ModelFileName.substr(0, m_ModelFileName.length() - 3);

	File f(m_ModelFileName);
	if (!f.Open())
	{
		Debug::Info("MDX[%s]: Unable to open file.", m_ModelFileName.c_str());
		return;
	}



	memcpy(&header, f.GetData(), sizeof(ModelHeader));

	animated = isAnimated(f) || forceAnim;

	// Initiate our model variables.
	trans = 1.0f;

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		specialTextures[i] = -1;
		replaceTextures[i] = 0;
		useReplaceTextures[i] = false;
	}

	showGeosets = 0;
	vbuf = nbuf = tbuf = 0;

	globalSequences = nullptr;
	animtime = 0;
	anim = 0;
	colors = nullptr;
	lights = nullptr;
	transparency = nullptr;
	particleSystems = nullptr;
	ribbons = nullptr;

	if (header.global_loops.size)
	{
		globalSequences = new uint32_t[header.global_loops.size];
		memcpy(globalSequences, (f.GetData() + header.global_loops.offset), sizeof(M2Loop) * header.global_loops.size);
	}

	if (animated)
	{
		initAnimated(f);
	}
	else
	{
		initStatic(f);
	}

	ok = true;
}

bool Model::isAnimated(File& f)
{
	// see if we have any animated bones
	M2CompBone *bo = (M2CompBone*)(f.GetData() + header.bones.offset);

	animGeometry = false;
	animBones = false;
	ind = false;

	M2Vertex *verts = (M2Vertex*)(f.GetData() + header.vertices.offset);
	for (size_t i = 0; i < header.vertices.size && !animGeometry; i++)
	{
		for (size_t b = 0; b < 4; b++)
		{
			if (verts[i].bone_weights[b] > 0)
			{
				M2CompBone &bb = bo[verts[i].bone_indices[b]];
				if (bb.translation.interpolation_type || bb.rotation.interpolation_type || bb.scale.interpolation_type || (bb.flags & 8))
				{
					if (bb.flags & 8)
					{
						// if we have billboarding, the model will need per-instance animation
						ind = true;
					}
					animGeometry = true;
					break;
				}
			}
		}
	}

	if (animGeometry) 
		animBones = true;
	else
	{
		for (size_t i = 0; i < header.bones.size; i++)
		{
			M2CompBone &bb = bo[i];
			if (bb.translation.interpolation_type || bb.rotation.interpolation_type || bb.scale.interpolation_type)
			{
				animBones = true;
				break;
			}
		}
	}

	animTextures = header.texture_transforms.size > 0;

	bool animMisc = header.cameras.size > 0 || // why waste time, pretty much all models with cameras need animation
		header.lights.size > 0 || // same here
		header.particle_emitters.size > 0 ||
		header.ribbon_emitters.size > 0;

	if (animMisc) animBones = true;

	// animated colors
	if (header.colors.size)
	{
		M2Color* cols = (M2Color*)(f.GetData() + header.colors.offset);
		for (size_t i = 0; i < header.colors.size; i++)
		{
			if (cols[i].color.interpolation_type != 0 || cols[i].alpha.interpolation_type != 0)
			{
				animMisc = true;
				break;
			}
		}
	}

	// animated opacity
	if (header.texture_weights.size && !animMisc)
	{
		M2TextureWeight* trs = (M2TextureWeight*)(f.GetData() + header.texture_weights.offset);
		for (size_t i = 0; i < header.texture_weights.size; i++)
		{
			if (trs[i].weight.interpolation_type != 0)
			{
				animMisc = true;
				break;
			}
		}
	}

	// guess not...
	return animGeometry || animTextures || animMisc;
}

void Model::initCommon(File& f)
{
	// assume: origVertices already set
	if (!animGeometry)
	{
		vertices = new vec3[header.vertices.size];
		normals = new vec3[header.vertices.size];
	}

	//vec3 vmin = vec3( 9999999.0f, 9999999.0f, 9999999.0f);
	//vec3 vmax = vec3(-9999999.0f,-9999999.0f,-9999999.0f);
	// vertices, normals
	for (size_t i = 0; i < header.vertices.size; i++)
	{
		origVertices[i].pos = fixCoordSystem(origVertices[i].pos);
		origVertices[i].normal = fixCoordSystem(origVertices[i].normal);

		if (!animGeometry)
		{
			vertices[i] = origVertices[i].pos;
			normals[i] = glm::normalize(origVertices[i].normal);
		}

		float len = glm::length2(origVertices[i].pos);
		if (len > rad)
		{
			rad = len;
		}
		/*
		if (origVertices[i].pos.x < vmin.x) vmin.x = origVertices[i].pos.x;
		if (origVertices[i].pos.y < vmin.y) vmin.y = origVertices[i].pos.y;
		if (origVertices[i].pos.z < vmin.z) vmin.z = origVertices[i].pos.z;
		if (origVertices[i].pos.x > vmax.x) vmax.x = origVertices[i].pos.x;
		if (origVertices[i].pos.y > vmax.y) vmax.y = origVertices[i].pos.y;
		if (origVertices[i].pos.z > vmax.z) vmax.z = origVertices[i].pos.z;
		*/
	}
	rad = sqrtf(rad);
	//rad = max(vmin.length(),vmax.length());

	// textures
	M2Texture* texdef = nullptr;
	if (header.textures.size)
	{
		texdef = (M2Texture*)(f.GetData() + header.textures.offset);
		textures = new Texture*[header.textures.size];
		char texname[256];
		for (size_t i = 0; i < header.textures.size; i++)
		{
			// Error check
			if (i > TEXTURE_MAX - 1)
			{
				Debug::Error("Error: Model Texture %d over %d", header.textures.size, TEXTURE_MAX);
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
		colors = new ModelColor[header.colors.size];
		M2Color *colorDefs = (M2Color*)(f.GetData() + header.colors.offset);
		for (size_t i = 0; i < header.colors.size; i++)
			colors[i].init(f, colorDefs[i], globalSequences);
	}

	// init transparency
	int16_t* transLookup = nullptr;
	if (header.texture_weights.size)
	{
		transLookup = (int16_t*)(f.GetData() + header.transparency_lookup_table.offset);
		transparency = new ModelTransparency[header.texture_weights.size];
		M2TextureWeight *trDefs = (M2TextureWeight*)(f.GetData() + header.texture_weights.offset);
		for (size_t i = 0; i < header.texture_weights.size; i++)
		{
			transparency[i].init(f, trDefs[i], globalSequences);
		}
	}

	// just use the first LOD/view
	if (header.num_skin_profiles > 0)
	{
		// indices - allocate space, too
		string lodname = m_ModelName;
		lodname.append("00.skin");

		File g(lodname);
		if (!g.Open())
		{
			Debug::Info("Error: loading lod [%s]", lodname.c_str());
			return;
		}

		M2SkinProfile *view = (M2SkinProfile*)(g.GetData());

		uint16_t *indexLookup = (uint16_t*)(g.GetData() + view->vertices.offset);
		uint16_t *triangles = (uint16_t*)(g.GetData() + view->indices.offset);
		nIndices = view->indices.size;
		indices = new uint16_t[nIndices];
		for (size_t i = 0; i < nIndices; i++)
		{
			indices[i] = indexLookup[triangles[i]];
		}

		// render ops
		M2SkinSection *ops = (M2SkinSection*)(g.GetData() + view->submeshes.offset);
		M2Batch *tex = (M2Batch*)(g.GetData() + view->m_WMOBatchIndexes.offset);
		M2Material *renderFlags = (M2Material*)(f.GetData() + header.materials.offset);
		uint16_t *texlookup = (uint16_t*)(f.GetData() + header.texture_lookup_table.offset);
		uint16_t *texanimlookup = (uint16_t*)(f.GetData() + header.texture_transforms_lookup_table.offset);
		int16_t *texunitlookup = (int16_t*)(f.GetData() + header.tex_unit_lookup_table.offset);

		showGeosets = new bool[view->submeshes.size];
		for (size_t i = 0; i < view->submeshes.size; i++)
		{
			showGeosets[i] = true;
		}

		for (size_t j = 0; j < view->m_WMOBatchIndexes.size; j++)
		{
			ModelRenderPass pass;

			pass.usetex2 = false;
			pass.useEnvMap = false;
			pass.cull = false;
			pass.trans = false;
			pass.unlit = false;
			pass.noZWrite = false;
			pass.billboard = false;

			size_t geoset = tex[j].skinSectionIndex;

			pass.geoset = (int)geoset;

			pass.indexStart = ops[geoset].indexStart;
			pass.indexCount = ops[geoset].indexCount;
			pass.vertexStart = ops[geoset].vertexStart;
			pass.vertexEnd = pass.vertexStart + ops[geoset].vertexCount;

			pass.order = tex[j].shader_id;

			//Texture* texid = textures[texlookup[tex[j].textureid]];
			//pass.texture = texid;
			pass.tex = texlookup[tex[j].textureComboIndex];

			// TODO: figure out these flags properly -_-
			M2Material &rf = renderFlags[tex[j].materialIndex];


			pass.blendmode = rf.blending_mode;
			pass.color = tex[j].colorIndex;
			pass.opacity = transLookup[tex[j].textureWeightComboIndex];

			pass.unlit = (rf.flags & M2MATERIAL_FLAGS_UNLIT) != 0;
			pass.cull = (rf.flags & M2MATERIAL_FLAGS_TWOSIDED) == 0 && rf.blending_mode == 0;

			pass.billboard = (rf.flags & M2MATERIAL_FLAGS_DEPTHTESTBILLBOARD) != 0;

			pass.useEnvMap = (texunitlookup[tex[j].materialLayer] == -1) && pass.billboard && rf.blending_mode > 2;
			pass.noZWrite = (rf.flags & M2MATERIAL_FLAGS_DEPTHWRITE) != 0;

			// ToDo: Work out the correct way to get the true/false of transparency
			pass.trans = (pass.blendmode > 0) && (pass.opacity > 0);	// Transparency - not the correct way to get transparency

			pass.p = ops[geoset].centerPosition.x;

			// Texture flags
			pass.swrap = (texdef[pass.tex].flags & M2TEXTURE_FLAGS_WRAPX) != 0; // Texture wrap X
			pass.twrap = (texdef[pass.tex].flags & M2TEXTURE_FLAGS_WRAPY) != 0; // Texture wrap Y

			if (animTextures)
			{
				if (tex[j].flags & TEXTUREUNIT_STATIC)
				{
					pass.texanim = -1; // no texture animation
				}
				else
				{
					pass.texanim = texanimlookup[tex[j].textureTransformComboIndex];
				}
			}
			else
			{
				pass.texanim = -1; // no texture animation
			}

			passes.push_back(pass);
		}

		// transparent parts come later
		sort(passes.begin(), passes.end());
	}
}

void Model::initStatic(File& f)
{
	origVertices = (M2Vertex*)(f.GetData() + header.vertices.offset);

	initCommon(f);

	dlist = glGenLists(1);
	glNewList(dlist, GL_COMPILE);

	drawModel();

	glEndList();

	// clean up vertices, indices etc
	delete[] vertices;
	delete[] normals;
	delete[] indices;

	if (colors) delete[] colors;
	if (transparency) delete[] transparency;
}

void Model::initAnimated(File& f)
{
	origVertices = new M2Vertex[header.vertices.size];
	memcpy(origVertices, f.GetData() + header.vertices.offset, header.vertices.size * sizeof(M2Vertex));

	glGenBuffersARB(1, &vbuf);
	glGenBuffersARB(1, &tbuf);
	const size_t size = header.vertices.size * sizeof(float);
	vbufsize = 3 * size;

	initCommon(f);

	if (header.sequences.size > 0)
	{
		anims = new M2Sequence[header.sequences.size];
		memcpy(anims, f.GetData() + header.sequences.offset, header.sequences.size * sizeof(M2Sequence));

		animfiles = new File[header.sequences.size];
		char tempname[256];
		for (size_t i = 0; i < header.sequences.size; i++)
		{
			sprintf_s(tempname, "%s%04d-%02d.anim", m_ModelName.c_str(), anims[i].id, anims[i].variationIndex);
			if (MPQFile::GetFileSize(tempname) > 0)
			{
				animfiles[i].SetName(tempname);
				animfiles[i].Open();
			} 
			else
			{
				Debug::Warn("MDX[%s]: Animation doesn't exists.", tempname);
			}
		}
	}

	if (animBones)
	{
		// init bones...
		bones = new Bone[header.bones.size];
		M2CompBone *mb = (M2CompBone*)(f.GetData() + header.bones.offset);
		for (size_t i = 0; i < header.bones.size; i++)
		{
			bones[i].init(f, mb[i], globalSequences, animfiles);
		}
	}

	if (!animGeometry)
	{
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbuf);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, vbufsize, vertices, GL_STATIC_DRAW_ARB);
		glGenBuffersARB(1, &nbuf);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, nbuf);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, vbufsize, normals, GL_STATIC_DRAW_ARB);
		delete[] vertices;
		delete[] normals;
	}
	vec2 *texcoords = new vec2[header.vertices.size];
	for (size_t i = 0; i < header.vertices.size; i++)
		texcoords[i] = origVertices[i].tex_coords[0];
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, tbuf);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, 2 * size, texcoords, GL_STATIC_DRAW_ARB);
	delete[] texcoords;

	if (animTextures)
	{
		texAnims = new ModelTextureAnim[header.texture_transforms.size];
		M2TextureTransform *ta = (M2TextureTransform*)(f.GetData() + header.texture_transforms.offset);
		for (size_t i = 0; i < header.texture_transforms.size; i++)
		{
			texAnims[i].init(f, ta[i], globalSequences);
		}
	}

	// particle systems
	if (header.particle_emitters.size)
	{
		M2Particle* pdefs = (M2Particle*)(f.GetData() + header.particle_emitters.offset);
		particleSystems = new ParticleSystem[header.particle_emitters.size];
		for (size_t i = 0; i < header.particle_emitters.size; i++)
		{
			particleSystems[i].model = this;
			particleSystems[i].init(f, pdefs[i], globalSequences);
		}
	}

	// ribbons
	if (header.ribbon_emitters.size)
	{
		M2Ribbon* rdefs = (M2Ribbon*)(f.GetData() + header.ribbon_emitters.offset);
		ribbons = new RibbonEmitter[header.ribbon_emitters.size];
		for (size_t i = 0; i < header.ribbon_emitters.size; i++)
		{
			ribbons[i].model = this;
			ribbons[i].init(f, rdefs[i], globalSequences);
		}
	}

	// just use the first camera, meh
	if (header.cameras.size > 0)
	{
		M2Camera* camDefs = (M2Camera*)(f.GetData() + header.cameras.offset);
		m_ModelCamera = new ModelCamera();
		m_ModelCamera->init(f, camDefs[0], globalSequences);
	}

	// init lights
	if (header.lights.size)
	{
		lights = new ModelLight[header.lights.size];
		M2Light* lDefs = (M2Light*)(f.GetData() + header.lights.offset);
		for (size_t i = 0; i < header.lights.size; i++)
			lights[i].init(f, lDefs[i], globalSequences);
	}

	animcalc = false;
}

void Model::calcBones(int anim, int time)
{
	for (size_t i = 0; i < header.bones.size; i++)
	{
		bones[i].calc = false;
	}

	for (size_t i = 0; i < header.bones.size; i++)
	{
		bones[i].calcMatrix(bones, anim, time);
	}
}

void Model::animate(int anim)
{
	M2Sequence &a = anims[anim];
	int t = globalTime; //(int)(_World->animtime /* / a.playSpeed*/);
	int tmax = a.duration;
	t %= tmax;
	animtime = t;
	this->anim = anim;

	if (animBones)
	{
		calcBones(anim, t);
	}

	if (animGeometry)
	{

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbuf);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, 2 * vbufsize, NULL, GL_STREAM_DRAW_ARB);
		vertices = (vec3*)glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_WRITE_ONLY);

		// transform vertices
		M2Vertex *ov = origVertices;
		for (size_t i = 0, k = 0; i < header.vertices.size; ++i, ++ov)
		{
			vec3 v(0, 0, 0), n(0, 0, 0);

			for (size_t b = 0; b < 4; b++)
			{
				if (ov->bone_weights[b] > 0)
				{
					vec3 tv = bones[ov->bone_indices[b]].mat * ov->pos;
					vec3 tn = bones[ov->bone_indices[b]].mrot * ov->normal;
					v += tv * ((float)ov->bone_weights[b] / 255.0f);
					n += tn * ((float)ov->bone_weights[b] / 255.0f);
				}
			}

			/*
			vertices[i] = v;
			normals[i] = n;
			*/
			vertices[i] = v;
			vertices[header.vertices.size + i] = glm::normalize(n); // shouldn't these be normal by default?
		}

		glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);

	}

	for (size_t i = 0; i < header.lights.size; i++)
	{
		if (lights[i].parent >= 0)
		{
			lights[i].tpos = bones[lights[i].parent].mat * lights[i].pos;
			lights[i].tdir = bones[lights[i].parent].mrot * lights[i].dir;
		}
	}

	for (size_t i = 0; i < header.particle_emitters.size; i++)
	{
		// random time distribution for teh win ..?
		int pt = (t + (int)(tmax*particleSystems[i].tofs)) % tmax;
		particleSystems[i].setup(anim, pt);
	}

	for (size_t i = 0; i < header.ribbon_emitters.size; i++)
	{
		ribbons[i].setup(anim, t);
	}

	if (animTextures)
	{
		for (size_t i = 0; i < header.texture_transforms.size; i++)
		{
			texAnims[i].calc(anim, t);
		}
	}
}

void Model::drawModel()
{
	// assume these client states are enabled: GL_VERTEX_ARRAY, GL_NORMAL_ARRAY, GL_TEXTURE_COORD_ARRAY

	if (animated)
	{

		if (animGeometry)
		{

			glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbuf);

			glVertexPointer(3, GL_FLOAT, 0, 0);
			glNormalPointer(GL_FLOAT, 0, GL_BUFFER_OFFSET(vbufsize));

		}
		else
		{
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbuf);
			glVertexPointer(3, GL_FLOAT, 0, 0);

			glBindBufferARB(GL_ARRAY_BUFFER_ARB, nbuf);
			glNormalPointer(GL_FLOAT, 0, 0);
		}

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, tbuf);
		glTexCoordPointer(2, GL_FLOAT, 0, 0);
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glAlphaFunc(GL_GREATER, 0.3f);

	for (size_t i = 0; i < passes.size(); i++)
	{
		ModelRenderPass& p = passes[i];

		if (p.init(this))
		{
			// we don't want to render completely transparent parts

			// render
			if (animated)
			{
				//glDrawElements(GL_TRIANGLES, p.indexCount, GL_UNSIGNED_SHORT, indices + p.indexStart);
				// a GDC OpenGL Performace Tuning paper recommended glDrawRangeElements over glDrawElements
				// I can't notice a difference but I guess it can't hurt
				if (true && true)
				{
					glDrawRangeElements(GL_TRIANGLES, p.vertexStart, p.vertexEnd, p.indexCount, GL_UNSIGNED_SHORT, indices + p.indexStart);
					//} else if (!supportVBO) {
					//	glDrawElements(GL_TRIANGLES, p.indexCount, GL_UNSIGNED_SHORT, indices + p.indexStart); 
				}
				else
				{
					glBegin(GL_TRIANGLES);
					for (size_t k = 0, b = p.indexStart; k < p.indexCount; k++, b++)
					{
						uint16_t a = indices[b];
						glNormal3fv(glm::value_ptr(normals[a]));
						glTexCoord2fv(glm::value_ptr(origVertices[a].tex_coords[0]));
						glVertex3fv(glm::value_ptr(vertices[a]));
					}
					glEnd();
				}
			}
			else
			{
				glBegin(GL_TRIANGLES);
				for (size_t k = 0, b = p.indexStart; k < p.indexCount; k++, b++)
				{
					uint16_t a = indices[b];
					glNormal3fv(glm::value_ptr(normals[a]));
					glTexCoord2fv(glm::value_ptr(origVertices[a].tex_coords[0]));
					glVertex3fv(glm::value_ptr(vertices[a]));
				}
				glEnd();
			}

			p.deinit();
		}

	}
	// done with all render ops

	glAlphaFunc(GL_GREATER, 0.0f);
	glDisable(GL_ALPHA_TEST);

	GLfloat czero[4] = {0,0,0,1};
	glMaterialfv(GL_FRONT, GL_EMISSION, czero);
	glColor4f(1, 1, 1, 1);
	glDepthMask(GL_TRUE);
}

void Model::draw()
{
	if (!ok) return;

	if (!animated)
	{
		glCallList(dlist);
	}
	else
	{
		if (ind) 
			animate(0);
		else
		{
			if (!animcalc)
			{
				animate(0);
				animcalc = true;
			}
		}

		lightsOn(GL_LIGHT4);
		drawModel();
		lightsOff(GL_LIGHT4);

		// effects are unfogged..?
		glDisable(GL_FOG);

		// draw particle systems
		for (size_t i = 0; i < header.particle_emitters.size; i++)
		{
			particleSystems[i].draw();
		}

		// draw ribbons
		for (size_t i = 0; i < header.ribbon_emitters.size; i++)
		{
			ribbons[i].draw();
		}

		if (_WowSettings->drawfog)
		{
			glEnable(GL_FOG);
		}
	}
}

void Model::lightsOn(GLuint lbase)
{
	// setup lights
	for (uint32_t i = 0, l = lbase; i < header.lights.size; i++)
	{
		lights[i].setup(animtime, l++);
	}
}

void Model::lightsOff(GLuint lbase)
{
	for (uint32_t i = 0, l = lbase; i < header.lights.size; i++)
	{
		glDisable(l++);
	}
}

void Model::updateEmitters(float dt)
{
	if (!ok) return;
	for (size_t i = 0; i < header.particle_emitters.size; i++)
	{
		particleSystems[i].update(dt);
	}
}
