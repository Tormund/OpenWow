#include "../stdafx.h"

// General
#include "MDX.h"

// Additional
#include "../Environment/EnvironmentManager.h"

bool MDX::isAnimated(File& f)
{
	// see if we have any animated bones
	M2CompBone* bo = (M2CompBone*)(f.GetData() + header.bones.offset);

	animGeometry = false;
	animBones = false;
	m_IsBillboard = false;

	M2Vertex* verts = (M2Vertex*)(f.GetData() + header.vertices.offset);
	for (uint32 i = 0; i < header.vertices.size && !animGeometry; i++)
	{
		for (uint32 b = 0; b < 4; b++)
		{
			if (verts[i].bone_weights[b] > 0)
			{
				M2CompBone& bb = bo[verts[i].bone_indices[b]];
				if (bb.translation.interpolation_type || bb.rotation.interpolation_type || bb.scale.interpolation_type || (bb.flags.spherical_billboard))
				{
					if (bb.flags.spherical_billboard)
					{
						m_IsBillboard = true;
					}

					animGeometry = true;

					break;
				}
			}
		}
	}

	if (animGeometry)
	{
		animBones = true;
	}
	else
	{
		for (uint32 i = 0; i < header.bones.size; i++)
		{
			M2CompBone& bb = bo[i];
			if (bb.translation.interpolation_type || bb.rotation.interpolation_type || bb.scale.interpolation_type)
			{
				animBones = true;
				break;
			}
		}
	}

	animTextures = header.texture_transforms.size > 0;

	bool animMisc = header.cameras.size > 0 || header.lights.size > 0 || header.particle_emitters.size > 0 || header.ribbon_emitters.size > 0;

	if (animMisc)
	{
		animBones = true;
	}

	// animated colors
	if (header.colors.size)
	{
		M2Color* cols = (M2Color*)(f.GetData() + header.colors.offset);
		for (uint32 i = 0; i < header.colors.size; i++)
		{
			if (cols[i].color.interpolation_type || cols[i].alpha.interpolation_type)
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
		for (uint32 i = 0; i < header.texture_weights.size; i++)
		{
			if (trs[i].weight.interpolation_type)
			{
				animMisc = true;
				break;
			}
		}
	}

	return animGeometry || animTextures || animMisc;
}

void MDX::initAnimated(File& f)
{
	m_OriginalVertexes = new M2Vertex[header.vertices.size];
	memcpy(m_OriginalVertexes, f.GetData() + header.vertices.offset, header.vertices.size * sizeof(M2Vertex));

	initCommon(f);

	if (header.sequences.size > 0)
	{
		m_Sequences = new M2Sequence[header.sequences.size];
		memcpy(m_Sequences, f.GetData() + header.sequences.offset, header.sequences.size * sizeof(M2Sequence));

		animfiles = new File[header.sequences.size];
		char tempname[256];
		for (uint32 i = 0; i < header.sequences.size; i++)
		{
			sprintf_s(tempname, "%s%04d-%02d.anim", m_ModelName.c_str(), m_Sequences[i].id, m_Sequences[i].variationIndex);
			if (MPQFile::GetFileSize(tempname) > 0)
			{
				animfiles[i].SetName(tempname);
				animfiles[i].Open();
			}
			else
			{
				//Debug::Warn("MDX[%s]: Animation doesn't exists.", tempname);
				//Debug::Warn("header.bones.size = [%d]", header.bones.size);
				//assert1(animBones == false);
			}
		}
	}

	if (animBones)
	{
		m_Part_Bones = new MDX_Part_Bone[header.bones.size];
		M2CompBone* bonesDefs = (M2CompBone*)(f.GetData() + header.bones.offset);
		for (uint32 i = 0; i < header.bones.size; i++)
		{
			m_Part_Bones[i].init(f, bonesDefs[i], m_GlobalLoops, animfiles);
		}
	}

	if (header.texture_transforms.size)
	{
		m_TexturesAnims = new MDX_Part_TextureAnim[header.texture_transforms.size];
		M2TextureTransform* textureAnimDefs = (M2TextureTransform*)(f.GetData() + header.texture_transforms.offset);
		for (uint32 i = 0; i < header.texture_transforms.size; i++)
		{
			m_TexturesAnims[i].init(f, textureAnimDefs[i], m_GlobalLoops);
		}
	}

#ifdef MDX_PARTICLES_ENABLE

	// particle systems
	if (header.particle_emitters.size)
	{
		M2Particle* pdefs = (M2Particle*)(f.GetData() + header.particle_emitters.offset);
		particleSystems = new ParticleSystem[header.particle_emitters.size];
		for (uint32 i = 0; i < header.particle_emitters.size; i++)
		{
			particleSystems[i].model = this;
			particleSystems[i].init(f, pdefs[i], m_GlobalLoops);
		}
	}

	// ribbons
	if (header.ribbon_emitters.size)
	{
		M2Ribbon* rdefs = (M2Ribbon*)(f.GetData() + header.ribbon_emitters.offset);
		ribbons = new RibbonEmitter[header.ribbon_emitters.size];
		for (uint32 i = 0; i < header.ribbon_emitters.size; i++)
		{
			ribbons[i].model = this;
			ribbons[i].init(f, rdefs[i], m_GlobalLoops);
		}
	}

#endif

	// just use the first camera, meh
	if (header.cameras.size)
	{
		m_Cameras = new MDX_Part_Camera[header.cameras.size];
		M2Camera* cameraDefs = (M2Camera*)(f.GetData() + header.cameras.offset);
		for (uint32 i = 0; i < header.cameras.size; i++)
		{
			m_Cameras[i].init(f, cameraDefs[i], m_GlobalLoops);
		}
	}

	// init lights
	if (header.lights.size)
	{
		m_Lights = new MDX_Part_Light[header.lights.size];
		M2Light* lightsDefs = (M2Light*)(f.GetData() + header.lights.offset);
		for (uint32 i = 0; i < header.lights.size; i++)
		{
			m_Lights[i].init(f, lightsDefs[i], m_GlobalLoops);
		}
	}

	animcalc = false;
}

void MDX::calcBones(uint32 _animationIndex, int time)
{
	for (uint32 i = 0; i < header.bones.size; i++)
	{
		m_Part_Bones[i].m_IsCalculated = false;
	}

	for (uint32 i = 0; i < header.bones.size; i++)
	{
		m_Part_Bones[i].calcMatrix(m_Part_Bones, _animationIndex, time);
	}
}

void MDX::animate(uint32 _animationIndex)
{
	int tmax = m_Sequences[_animationIndex].duration;
	animtime = _EnvironmentManager->globalTime % tmax;

	m_CurrentAnimationIndex = _animationIndex;

	if (animBones)
	{
		calcBones(_animationIndex, animtime);
	}

	if (animGeometry)
	{
		M2Vertex* ov = m_OriginalVertexes;
		for (uint32 i = 0, k = 0; i < header.vertices.size; ++i, ++ov)
		{
			vec3 vertex(0, 0, 0);
			vec3 normal(0, 0, 0);

			for (uint32 b = 0; b < 4; b++)
			{
				if (ov->bone_weights[b] > 0)
				{
					vec3 tv = m_Part_Bones[ov->bone_indices[b]].m_TransformMatrix * ov->pos;
					vec3 tn = m_Part_Bones[ov->bone_indices[b]].m_RotationMatrix * ov->normal;

					vertex += tv * ((float)ov->bone_weights[b] / 255.0f);
					normal += tn * ((float)ov->bone_weights[b] / 255.0f);
				}
			}

			m_Vertices[i] = vertex;
			m_Normals[i] = normal.normalized(); // shouldn't these be normal by default?
		}

		// Add sub-data

		glBindBuffer(GL_ARRAY_BUFFER, globalBuffer);

		glBufferSubData(GL_ARRAY_BUFFER, header.vertices.size * 0 * sizeof(float), header.vertices.size * sizeof(vec3), m_Vertices);
		//glBufferSubData(GL_ARRAY_BUFFER, header.vertices.size * 3 * sizeof(float), header.vertices.size * sizeof(vec2), m_Texcoords); not used
		glBufferSubData(GL_ARRAY_BUFFER, header.vertices.size * 5 * sizeof(float), header.vertices.size * sizeof(vec3), m_Normals);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	for (uint32 i = 0; i < header.lights.size; i++)
	{
		if (m_Lights[i].parent >= 0)
		{
			m_Lights[i].tpos = m_Part_Bones[m_Lights[i].parent].m_TransformMatrix * m_Lights[i].pos;
			m_Lights[i].tdir = m_Part_Bones[m_Lights[i].parent].m_RotationMatrix * m_Lights[i].dir;
		}
	}

#ifdef MDX_PARTICLES_ENABLE
	for (uint32 i = 0; i < header.particle_emitters.size; i++)
	{
		// random time distribution for teh win ..?
		int pt = (animtime + (int)(tmax*particleSystems[i].tofs)) % tmax;
		particleSystems[i].setup(_animationIndex, pt);
	}

	for (uint32 i = 0; i < header.ribbon_emitters.size; i++)
	{
		ribbons[i].setup(_animationIndex, animtime);
	}

#endif

	if (animTextures)
	{
		for (uint32 i = 0; i < header.texture_transforms.size; i++)
		{
			m_TexturesAnims[i].calc(_animationIndex, animtime);
		}
	}
}