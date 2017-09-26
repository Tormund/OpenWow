#include "stdafx.h"

// Include
#include "MDX.h"

// General
#include "Model_Skin.h"

Model_Skin::Model_Skin(MDX* _model, File& _mF, File& _aF) : m_ModelObject(_model)
{
	if (!_aF.Open())
	{
		Debug::Info("Error: loading lod [%s]", _aF.Path_Name().c_str());
		return;
	}

	M2SkinProfile* view = (M2SkinProfile*)(_aF.GetData());

	// Skin data
	uint16_t* vertexesIndex = (uint16_t*)(_aF.GetData() + view->vertices.offset);
	uint16_t* indexesIndex = (uint16_t*)(_aF.GetData() + view->indices.offset);
	M2SkinSection* ops = (M2SkinSection*)(_aF.GetData() + view->submeshes.offset);
	M2Batch* tex = (M2Batch*)(_aF.GetData() + view->batches.offset);


	showGeosets = new bool[view->submeshes.size];
	for (uint32_t i = 0; i < view->submeshes.size; i++)
	{
		showGeosets[i] = true;
	}

	indices = new uint16_t[view->indices.size];
	for (uint32_t i = 0; i < view->indices.size; i++)
	{
		indices[i] = vertexesIndex[indexesIndex[i]];
	}

	M2Material* renderFlags = (M2Material*)(_mF.GetData() + _model->header.materials.offset);
	uint16_t* texlookup = (uint16_t*)(_mF.GetData() + _model->header.texture_lookup_table.offset);
	uint16_t* texanimlookup = (uint16_t*)(_mF.GetData() + _model->header.texture_transforms_lookup_table.offset);
	uint16_t* texunitlookup = (uint16_t*)(_mF.GetData() + _model->header.tex_unit_lookup_table.offset);

	for (uint32_t j = 0; j < view->batches.size; j++)
	{
		ModelRenderPass* pass = new ModelRenderPass();

		pass->usetex2 = false;
		pass->useEnvMap = false;
		pass->cull = false;
		pass->trans = false;
		pass->unlit = false;
		pass->noZWrite = false;
		pass->billboard = false;

		uint16_t geoset = tex[j].skinSectionIndex;

		pass->geoset = (int)geoset;

		pass->indexStart = ops[geoset].indexStart;
		pass->indexCount = ops[geoset].indexCount;

		pass->vertexStart = ops[geoset].vertexStart;
		pass->vertexEnd = pass->vertexStart + ops[geoset].vertexCount;

		pass->order = tex[j].shader_id;

		//Texture* texid = textures[texlookup[tex[j].textureid]];
		//pass->texture = texid;
		pass->tex = texlookup[tex[j].textureComboIndex];

		// TODO: figure out these flags properly -_-
		M2Material& rf = renderFlags[tex[j].materialIndex];


		pass->blendmode = rf.blending_mode;
		pass->color = tex[j].colorIndex;
		pass->opacity = _model->transLookup[tex[j].textureWeightComboIndex];

		pass->unlit = (rf.flags.M2MATERIAL_FLAGS_UNLIT) != 0;
		pass->cull = (rf.flags.M2MATERIAL_FLAGS_TWOSIDED) == 0 && rf.blending_mode == 0;

		pass->billboard = (rf.flags.M2MATERIAL_FLAGS_DEPTHTESTBILLBOARD) != 0;

		pass->useEnvMap = (texunitlookup[tex[j].materialLayer] == -1) && pass->billboard && rf.blending_mode > 2;
		pass->noZWrite = (rf.flags.M2MATERIAL_FLAGS_DEPTHWRITE) != 0;

		// ToDo: Work out the correct way to get the true/false of transparency
		pass->trans = (pass->blendmode > 0) && (pass->opacity > 0);	// Transparency - not the correct way to get transparency

		pass->p = ops[geoset].centerPosition.x;

		// Texture flags
		pass->swrap = (_model->texdef[pass->tex].flags.M2TEXTURE_FLAGS_WRAPX) != 0; // Texture wrap X
		pass->twrap = (_model->texdef[pass->tex].flags.M2TEXTURE_FLAGS_WRAPY) != 0; // Texture wrap Y

		if (_model->m_TexturesAnims)
		{
			if (tex[j].flags & TEXTUREUNIT_STATIC)
			{
				pass->texanim = -1; // no texture animation
			}
			else
			{
				pass->texanim = texanimlookup[tex[j].textureTransformComboIndex];
			}
		}
		else
		{
			pass->texanim = -1; // no texture animation
		}

		m_Passes.push_back(pass);
	}

	// transparent parts come later
	sort(m_Passes.begin(), m_Passes.end());
}

Model_Skin::~Model_Skin()
{
	delete[] indices;
	delete[] showGeosets;
}

//

void Model_Skin::Draw()
{
	//if (m_ModelObject->m_IsBillboard)
	//{
	//	return;
	//}

	_TechniquesMgr->m_MDX_GeometryPass->Bind();
	_TechniquesMgr->m_MDX_GeometryPass->SetPVW();

	for (size_t i = 0; i < m_Passes.size(); i++)
	{
		ModelRenderPass* p = m_Passes[i];

		if (p->init(m_ModelObject) && showGeosets[p->geoset])
		{
			m_ModelObject->drawShaderBegin();

			glDrawElements(GL_TRIANGLES, p->indexCount, GL_UNSIGNED_SHORT, indices + p->indexStart);

			m_ModelObject->drawShaderEnd();

			_Perfomance->Inc(PERF_TEST);

			p->deinit();
		}
	}

	_TechniquesMgr->m_MDX_GeometryPass->Unbind();
}
