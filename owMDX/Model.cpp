#include "stdafx.h"

// General
#include "model.h"

// Additional
#include "Model_RenderPass.h"
#include "ModelsUtils.h"

int globalTime = 0;

#define GL_BUFFER_OFFSET(i) ((char *)(0) + (i))

Model::Model(cstring name, bool forceAnim) : RefItemNamed(name), forceAnim(forceAnim)
{
	// replace .MDX with .M2
	char tempname[256];
	strncpy_s(tempname, name.c_str(), sizeof(tempname));
	if (tempname[name.length() - 1] != '2')
	{
		tempname[name.length() - 2] = '2';
		tempname[name.length() - 1] = 0;
	}

	File f(tempname);
	f.Open();
	ok = !f.IsEof();

	if (!ok)
	{
		Debug::Info("Error: loading model [%s]", tempname);
		return;
	}
	fullname = tempname;

	memcpy(&header, f.GetData(), sizeof(ModelHeader));

	// HACK: these particle systems are way too active and cause horrible horrible slowdowns
	// I'm removing them until I can fix emission speed so it doesn't get this crazy
	if (false
		|| name == "World\\Kalimdor\\Orgrimmar\\Passivedoodads\\Orgrimmarbonfire\\Orgrimmarsmokeemitter.Mdx"
		//|| name=="World\\Kalimdor\\Orgrimmar\\Passivedoodads\\Orgrimmarbonfire\\Orgrimmarbonfire01.Mdx"	
		)
	{
		header.nParticleEmitters = 0;
	}

	animated = isAnimated(f) || forceAnim;  // isAnimated will set animGeometry and animTextures

	Debug::Info("Loading model %s%s", tempname, animated ? " (animated)" : "");

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

	globalSequences = 0;
	animtime = 0;
	anim = 0;
	colors = 0;
	lights = 0;
	transparency = 0;
	particleSystems = 0;
	ribbons = 0;
	if (header.nGlobalSequences)
	{
		globalSequences = new uint32_t[header.nGlobalSequences];
		memcpy(globalSequences, (f.GetData() + header.ofsGlobalSequences), header.nGlobalSequences * 4);
	}

	if (animated)
		initAnimated(f);
	else
		initStatic(f);

	//f.close();
}

Model::~Model()
{
	if (ok)
	{
		//Debug::Info("Unloading model %s", name.c_str());

		if (header.nTextures)
		{
			for (size_t i = 0; i < header.nTextures; i++)
			{
				if (textures[i] != 0)
				{
					//Texture *tex = (Texture*)video.textures.items[textures[i]];
					//video.textures.del(textures[i]); // BOUZI
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

bool Model::isAnimated(File &f)
{
	// see if we have any animated bones
	ModelBoneDef *bo = (ModelBoneDef*)(f.GetData() + header.ofsBones);

	animGeometry = false;
	animBones = false;
	ind = false;

	ModelVertex *verts = (ModelVertex*)(f.GetData() + header.ofsVertices);
	for (size_t i = 0; i < header.nVertices && !animGeometry; i++)
	{
		for (size_t b = 0; b < 4; b++)
		{
			if (verts[i].weights[b] > 0)
			{
				ModelBoneDef &bb = bo[verts[i].bones[b]];
				if (bb.translation.type || bb.rotation.type || bb.scaling.type || (bb.flags & 8))
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
		for (size_t i = 0; i < header.nBones; i++)
		{
			ModelBoneDef &bb = bo[i];
			if (bb.translation.type || bb.rotation.type || bb.scaling.type)
			{
				animBones = true;
				break;
			}
		}
	}

	animTextures = header.nTexAnims > 0;

	bool animMisc = header.nCameras > 0 || // why waste time, pretty much all models with cameras need animation
		header.nLights > 0 || // same here
		header.nParticleEmitters > 0 ||
		header.nRibbonEmitters > 0;

	if (animMisc) animBones = true;

	// animated colors
	if (header.nColors)
	{
		ModelColorDef *cols = (ModelColorDef*)(f.GetData() + header.ofsColors);
		for (size_t i = 0; i < header.nColors; i++)
		{
			if (cols[i].color.type != 0 || cols[i].opacity.type != 0)
			{
				animMisc = true;
				break;
			}
		}
	}

	// animated opacity
	if (header.nTransparency && !animMisc)
	{
		ModelTransDef *trs = (ModelTransDef*)(f.GetData() + header.ofsTransparency);
		for (size_t i = 0; i < header.nTransparency; i++)
		{
			if (trs[i].trans.type != 0)
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
		vertices = new vec3[header.nVertices];
		normals = new vec3[header.nVertices];
	}

	//vec3 vmin = vec3( 9999999.0f, 9999999.0f, 9999999.0f);
	//vec3 vmax = vec3(-9999999.0f,-9999999.0f,-9999999.0f);
	// vertices, normals
	for (size_t i = 0; i < header.nVertices; i++)
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
	ModelTextureDef *texdef = (ModelTextureDef*)(f.GetData() + header.ofsTextures);
	if (header.nTextures)
	{
		textures = new Texture*[header.nTextures];
		char texname[256];
		for (size_t i = 0; i < header.nTextures; i++)
		{
			// Error check
			if (i > TEXTURE_MAX - 1)
			{
				Debug::Info("Error: Model Texture %d over %d", header.nTextures, TEXTURE_MAX);
				break;
			}

			if (texdef[i].type == 0)
			{
				strncpy_s(texname, (const char*)(f.GetData() + texdef[i].nameOfs), texdef[i].nameLen);
				texname[texdef[i].nameLen] = 0;
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
	if (header.nColors)
	{
		colors = new ModelColor[header.nColors];
		ModelColorDef *colorDefs = (ModelColorDef*)(f.GetData() + header.ofsColors);
		for (size_t i = 0; i < header.nColors; i++)
			colors[i].init(f, colorDefs[i], globalSequences);
	}

	// init transparency
	int16_t *transLookup = (int16_t*)(f.GetData() + header.ofsTransparencyLookup);
	if (header.nTransparency)
	{
		transparency = new ModelTransparency[header.nTransparency];
		ModelTransDef *trDefs = (ModelTransDef*)(f.GetData() + header.ofsTransparency);
		for (size_t i = 0; i < header.nTransparency; i++)
			transparency[i].init(f, trDefs[i], globalSequences);
	}

	// just use the first LOD/view

	if (header.nViews > 0)
	{
		// indices - allocate space, too
		string lodname = fullname.substr(0, fullname.length() - 3);
		fullname = lodname;
		lodname.append("00.skin");
		File g(lodname.c_str());
		g.Open();
		if (g.IsEof())
		{
			Debug::Info("Error: loading lod [%s]", lodname.c_str());
			//g.close();
			return;
		}
		ModelView *view = (ModelView*)(g.GetData());

		uint16_t *indexLookup = (uint16_t*)(g.GetData() + view->ofsIndex);
		uint16_t *triangles = (uint16_t*)(g.GetData() + view->ofsTris);
		nIndices = view->nTris;
		indices = new uint16_t[nIndices];
		for (size_t i = 0; i < nIndices; i++)
		{
			indices[i] = indexLookup[triangles[i]];
		}

		// render ops
		ModelGeoset *ops = (ModelGeoset*)(g.GetData() + view->ofsSub);
		ModelTexUnit *tex = (ModelTexUnit*)(g.GetData() + view->ofsTex);
		ModelRenderFlags *renderFlags = (ModelRenderFlags*)(f.GetData() + header.ofsTexFlags);
		uint16_t *texlookup = (uint16_t*)(f.GetData() + header.ofsTexLookup);
		uint16_t *texanimlookup = (uint16_t*)(f.GetData() + header.ofsTexAnimLookup);
		int16_t *texunitlookup = (int16_t*)(f.GetData() + header.ofsTexUnitLookup);

		showGeosets = new bool[view->nSub];
		for (size_t i = 0; i < view->nSub; i++)
		{
			showGeosets[i] = true;
		}

		for (size_t j = 0; j < view->nTex; j++)
		{
			ModelRenderPass pass;

			pass.usetex2 = false;
			pass.useEnvMap = false;
			pass.cull = false;
			pass.trans = false;
			pass.unlit = false;
			pass.noZWrite = false;
			pass.billboard = false;

			size_t geoset = tex[j].op;

			pass.geoset = (int)geoset;

			pass.indexStart = ops[geoset].istart;
			pass.indexCount = ops[geoset].icount;
			pass.vertexStart = ops[geoset].vstart;
			pass.vertexEnd = pass.vertexStart + ops[geoset].vcount;

			pass.order = tex[j].shading;

			//Texture* texid = textures[texlookup[tex[j].textureid]];
			//pass.texture = texid;
			pass.tex = texlookup[tex[j].textureid];

			// TODO: figure out these flags properly -_-
			ModelRenderFlags &rf = renderFlags[tex[j].flagsIndex];


			pass.blendmode = rf.blend;
			pass.color = tex[j].colorIndex;
			pass.opacity = transLookup[tex[j].transid];

			pass.unlit = (rf.flags & RENDERFLAGS_UNLIT) != 0;
			pass.cull = (rf.flags & RENDERFLAGS_TWOSIDED) == 0 && rf.blend == 0;

			pass.billboard = (rf.flags & RENDERFLAGS_BILLBOARD) != 0;

			pass.useEnvMap = (texunitlookup[tex[j].texunit] == -1) && pass.billboard && rf.blend > 2;
			pass.noZWrite = (rf.flags & RENDERFLAGS_ZBUFFERED) != 0;

			// ToDo: Work out the correct way to get the true/false of transparency
			pass.trans = (pass.blendmode > 0) && (pass.opacity > 0);	// Transparency - not the correct way to get transparency

			pass.p = ops[geoset].BoundingBox[0].x;

			// Texture flags
			pass.swrap = (texdef[pass.tex].flags & TEXTURE_WRAPX) != 0; // Texture wrap X
			pass.twrap = (texdef[pass.tex].flags & TEXTURE_WRAPY) != 0; // Texture wrap Y

			if (animTextures)
			{
				if (tex[j].flags & TEXTUREUNIT_STATIC)
				{
					pass.texanim = -1; // no texture animation
				}
				else
				{
					pass.texanim = texanimlookup[tex[j].texanimid];
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

	// zomg done
}

void Model::initStatic(File &f)
{
	origVertices = (ModelVertex*)(f.GetData() + header.ofsVertices);

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

void Model::initAnimated(File &f)
{
	origVertices = new ModelVertex[header.nVertices];
	memcpy(origVertices, f.GetData() + header.ofsVertices, header.nVertices * sizeof(ModelVertex));

	glGenBuffersARB(1, &vbuf);
	glGenBuffersARB(1, &tbuf);
	const size_t size = header.nVertices * sizeof(float);
	vbufsize = 3 * size;

	initCommon(f);

	if (header.nAnimations > 0)
	{
		anims = new ModelAnimation[header.nAnimations];
		memcpy(anims, f.GetData() + header.ofsAnimations, header.nAnimations * sizeof(ModelAnimation));

		animfiles = new File[header.nAnimations];
		char tempname[256];
		for (size_t i = 0; i < header.nAnimations; i++)
		{
			sprintf_s(tempname, "%s%04d-%02d.anim", fullname.c_str(), anims[i].animID, anims[i].subAnimID);
			if (MPQFile::GetFileSize(tempname) > 0)
			{
				animfiles[i].SetName(tempname);
				animfiles[i].Open();
			}
		}
	}

	if (animBones)
	{
		// init bones...
		bones = new Bone[header.nBones];
		ModelBoneDef *mb = (ModelBoneDef*)(f.GetData() + header.ofsBones);
		for (size_t i = 0; i < header.nBones; i++)
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
	vec2 *texcoords = new vec2[header.nVertices];
	for (size_t i = 0; i < header.nVertices; i++)
		texcoords[i] = origVertices[i].texcoords;
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, tbuf);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, 2 * size, texcoords, GL_STATIC_DRAW_ARB);
	delete[] texcoords;

	if (animTextures)
	{
		texAnims = new ModelTextureAnim[header.nTexAnims];
		ModelTexAnimDef *ta = (ModelTexAnimDef*)(f.GetData() + header.ofsTexAnims);
		for (size_t i = 0; i < header.nTexAnims; i++)
		{
			texAnims[i].init(f, ta[i], globalSequences);
		}
	}

	// particle systems
	if (header.nParticleEmitters)
	{
		ModelParticleEmitterDef *pdefs = (ModelParticleEmitterDef *)(f.GetData() + header.ofsParticleEmitters);
		particleSystems = new ParticleSystem[header.nParticleEmitters];
		for (size_t i = 0; i < header.nParticleEmitters; i++)
		{
			particleSystems[i].model = this;
			particleSystems[i].init(f, pdefs[i], globalSequences);
		}
	}

	// ribbons
	if (header.nRibbonEmitters)
	{
		ModelRibbonEmitterDef *rdefs = (ModelRibbonEmitterDef *)(f.GetData() + header.ofsRibbonEmitters);
		ribbons = new RibbonEmitter[header.nRibbonEmitters];
		for (size_t i = 0; i < header.nRibbonEmitters; i++)
		{
			ribbons[i].model = this;
			ribbons[i].init(f, rdefs[i], globalSequences);
		}
	}

	// just use the first camera, meh
	if (header.nCameras > 0)
	{
		ModelCameraDef* camDefs = (ModelCameraDef*)(f.GetData() + header.ofsCameras);
		m_ModelCamera = new ModelCamera();
		m_ModelCamera->init(f, camDefs[0], globalSequences);
	}

	// init lights
	if (header.nLights)
	{
		lights = new ModelLight[header.nLights];
		ModelLightDef *lDefs = (ModelLightDef*)(f.GetData() + header.ofsLights);
		for (size_t i = 0; i < header.nLights; i++)
			lights[i].init(f, lDefs[i], globalSequences);
	}

	animcalc = false;
}

void Model::calcBones(int anim, int time)
{
	for (size_t i = 0; i < header.nBones; i++)
	{
		bones[i].calc = false;
	}

	for (size_t i = 0; i < header.nBones; i++)
	{
		bones[i].calcMatrix(bones, anim, time);
	}
}

void Model::animate(int anim)
{
	ModelAnimation &a = anims[anim];
	int t = globalTime; //(int)(_World->animtime /* / a.playSpeed*/);
	int tmax = a.length;
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
		ModelVertex *ov = origVertices;
		for (size_t i = 0, k = 0; i < header.nVertices; ++i, ++ov)
		{
			vec3 v(0, 0, 0), n(0, 0, 0);

			for (size_t b = 0; b < 4; b++)
			{
				if (ov->weights[b] > 0)
				{
					vec3 tv = bones[ov->bones[b]].mat * ov->pos;
					vec3 tn = bones[ov->bones[b]].mrot * ov->normal;
					v += tv * ((float)ov->weights[b] / 255.0f);
					n += tn * ((float)ov->weights[b] / 255.0f);
				}
			}

			/*
			vertices[i] = v;
			normals[i] = n;
			*/
			vertices[i] = v;
			vertices[header.nVertices + i] = glm::normalize(n); // shouldn't these be normal by default?
		}

		glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);

	}

	for (size_t i = 0; i < header.nLights; i++)
	{
		if (lights[i].parent >= 0)
		{
			lights[i].tpos = bones[lights[i].parent].mat * lights[i].pos;
			lights[i].tdir = bones[lights[i].parent].mrot * lights[i].dir;
		}
	}

	for (size_t i = 0; i < header.nParticleEmitters; i++)
	{
		// random time distribution for teh win ..?
		int pt = (t + (int)(tmax*particleSystems[i].tofs)) % tmax;
		particleSystems[i].setup(anim, pt);
	}

	for (size_t i = 0; i < header.nRibbonEmitters; i++)
	{
		ribbons[i].setup(anim, t);
	}

	if (animTextures)
	{
		for (size_t i = 0; i < header.nTexAnims; i++)
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

		//glTexCoordPointer(2, GL_FLOAT, sizeof(ModelVertex), &origVertices[0].texcoords);
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
						glTexCoord2fv(glm::value_ptr(origVertices[a].texcoords));
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
					glTexCoord2fv(glm::value_ptr(origVertices[a].texcoords));
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
		for (size_t i = 0; i < header.nParticleEmitters; i++)
		{
			particleSystems[i].draw();
		}

		// draw ribbons
		for (size_t i = 0; i < header.nRibbonEmitters; i++)
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
	for (uint32_t i = 0, l = lbase; i < header.nLights; i++)
	{
		lights[i].setup(animtime, l++);
	}
}

void Model::lightsOff(GLuint lbase)
{
	for (uint32_t i = 0, l = lbase; i < header.nLights; i++)
	{
		glDisable(l++);
	}
}

void Model::updateEmitters(float dt)
{
	if (!ok) return;
	for (size_t i = 0; i < header.nParticleEmitters; i++)
	{
		particleSystems[i].update(dt);
	}
}
