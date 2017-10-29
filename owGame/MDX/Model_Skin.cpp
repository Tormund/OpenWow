#include "../stdafx.h"

// Include
#include "MDX.h"

// General
#include "Model_Skin.h"

Model_Skin::Model_Skin(MDX* _model, File& _mF, File& _aF) : m_ModelObject(_model)
{
	if (!_aF.Open())
	{
		Modules::log().Info("Error: loading lod [%s]", _aF.Path_Name().c_str());
		return;
	}

	M2SkinProfile* view = (M2SkinProfile*)(_aF.GetData());

	// Skin data
	uint16*        vertexesIndex = (uint16*)        (_aF.GetData() + view->vertices.offset);
	uint16*        indexesIndex  = (uint16*)        (_aF.GetData() + view->indices.offset);
	M2SkinSection* skins         = (M2SkinSection*) (_aF.GetData() + view->submeshes.offset);
	M2SkinBatch*   skinBatch     = (M2SkinBatch*)   (_aF.GetData() + view->batches.offset);

	//------------

	M2Material*    m2Materials =   (M2Material*)    (_mF.GetData() + _model->header.materials.offset);
	uint16*        texlookup =     (uint16*)        (_mF.GetData() + _model->header.texture_lookup_table.offset);
	uint16*        texanimlookup = (uint16*)        (_mF.GetData() + _model->header.texture_transforms_lookup_table.offset);
	uint16*        texunitlookup = (uint16*)        (_mF.GetData() + _model->header.tex_unit_lookup_table.offset);

	for (uint32 j = 0; j < view->batches.size; j++)
	{
		ModelRenderPass* pass = new ModelRenderPass();

		uint16 m2SkinIndex = skinBatch[j].m2SkinIndex;

		pass->m2SkinIndex = (int)m2SkinIndex;

		// Geometry data
		pass->indexStart = skins[m2SkinIndex].indexStart;
		pass->indexCount = skins[m2SkinIndex].indexCount;
		pass->vertexStart = skins[m2SkinIndex].vertexStart;
		pass->vertexCount = skins[m2SkinIndex].vertexCount;


		// Get classes
		M2Material& rf = m2Materials[skinBatch[j].materialIndex];

		// Diffuse texture
		if (_model->specialTextures[texlookup[skinBatch[j].textureComboIndex]] == -1)
		{
			pass->__material.SetDiffuseTexture(_model->textures[texlookup[skinBatch[j].textureComboIndex]]->GetObj());
		}
		else
		{
			pass->__material.SetDiffuseTexture(_model->replaceTextures[_model->specialTextures[texlookup[skinBatch[j].textureComboIndex]]]->GetObj());
		}
		
		// Render state
		pass->__material.SetRenderState(rf.flags.M2MATERIAL_FLAGS_TWOSIDED /*&& rf.blending_mode*/, rf.flags.M2MATERIAL_FLAGS_DEPTHWRITE == 0);

		//Modules::log().Info("Blend is [%d]", rf.blending_mode);

		// Blend state
		switch (rf.blending_mode)
		{
			// 1 table
			case M2COMBINER_OPAQUE:
			pass->__material.SetBlendState(false);
			break;

			case M2COMBINER_MOD:
			pass->__material.SetBlendState(true, R_BlendFunc::BS_BLEND_ONE, R_BlendFunc::BS_BLEND_ZERO);
			break;

			case M2COMBINER_DECAL:
			pass->__material.SetBlendState(true, R_BlendFunc::BS_BLEND_SRC_ALPHA, R_BlendFunc::BS_BLEND_INV_SRC_ALPHA);
			break;

			case M2COMBINER_ADD:
			pass->__material.SetBlendState(true, R_BlendFunc::BS_BLEND_ONE, R_BlendFunc::BS_BLEND_ONE);
			break;

			case M2COMBINER_MOD2X:
			pass->__material.SetBlendState(true, R_BlendFunc::BS_BLEND_SRC_ALPHA, R_BlendFunc::BS_BLEND_ONE);
			break;

			case M2COMBINER_FADE:
			pass->__material.SetBlendState(true, R_BlendFunc::BS_BLEND_DEST_COLOR, R_BlendFunc::BS_BLEND_ZERO);
			break;

			case M2COMBINER_MOD2X_NA:
			pass->__material.SetBlendState(true, R_BlendFunc::BS_BLEND_DEST_COLOR, R_BlendFunc::BS_BLEND_SRC_COLOR);
			break;


			// 2 table
			/*case M2COMBINER_OPAQUE:
			pass->__material.SetBlendState(false);
			break;

			case M2COMBINER_MOD:
			pass->__material.SetBlendState(true, R_BlendFunc::BS_BLEND_ONE, R_BlendFunc::BS_BLEND_ZERO);
			break;

			case M2COMBINER_DECAL:
			pass->__material.SetBlendState(true, R_BlendFunc::BS_BLEND_SRC_ALPHA, R_BlendFunc::BS_BLEND_INV_SRC_ALPHA);
			break;

			case M2COMBINER_ADD:
			pass->__material.SetBlendState(true, R_BlendFunc::BS_BLEND_SRC_ALPHA, R_BlendFunc::BS_BLEND_DEST_ALPHA);
			break;

			case M2COMBINER_MOD2X:
			pass->__material.SetBlendState(true, R_BlendFunc::BS_BLEND_SRC_ALPHA, R_BlendFunc::BS_BLEND_ONE);
			break;

			case M2COMBINER_FADE:
			pass->__material.SetBlendState(true, R_BlendFunc::BS_BLEND_SRC_ALPHA, R_BlendFunc::BS_BLEND_INV_SRC_ALPHA);
			break;

			case M2COMBINER_MOD2X_NA:
			pass->__material.SetBlendState(true, R_BlendFunc::BS_BLEND_DEST_ALPHA, R_BlendFunc::BS_BLEND_SRC_ALPHA);
			break;*/

			default:
			assert1(rf.blending_mode);
		}
		


		/*pass->order = skinBatch[j].shader_id;

		//Texture* texid = m_DiffuseTextures[texlookup[tex[j].textureid]];
		//pass->texture = texid;
		pass->tex = texlookup[skinBatch[j].textureComboIndex];

		// TODO: figure out these flags properly -_-
		


		pass->blendmode = rf.blending_mode;
		pass->color = skinBatch[j].colorIndex;
		pass->opacity = _model->transLookup[skinBatch[j].textureWeightComboIndex];

		pass->unlit = (rf.flags.M2MATERIAL_FLAGS_UNLIT) != 0;
		pass->cull = (rf.flags.M2MATERIAL_FLAGS_TWOSIDED) == 0 && rf.blending_mode == 0;

		pass->billboard = (rf.flags.M2MATERIAL_FLAGS_DEPTHTEST) != 0;

		pass->useEnvMap = (texunitlookup[skinBatch[j].materialLayer] == -1) && pass->billboard && rf.blending_mode > 2;
		pass->noZWrite = (rf.flags.M2MATERIAL_FLAGS_DEPTHWRITE) != 0;

		// ToDo: Work out the correct way to get the true/false of transparency
		pass->trans = (pass->blendmode > 0) && (pass->opacity > 0);	// Transparency - not the correct way to get transparency

		pass->p = skins[m2SkinIndex].centerPosition.x;

		// Texture flags
		pass->swrap = (_model->texdef[pass->tex].flags.M2TEXTURE_FLAGS_WRAPX) != 0; // Texture wrap X
		pass->twrap = (_model->texdef[pass->tex].flags.M2TEXTURE_FLAGS_WRAPY) != 0; // Texture wrap Y

		if (_model->m_TexturesAnims)
		{
			if (skinBatch[j].flags & TEXTUREUNIT_STATIC)
			{
				pass->texanim = -1; // no texture animation
			}
			else
			{
				pass->texanim = texanimlookup[skinBatch[j].textureTransformComboIndex];
			}
		}
		else
		{
			pass->texanim = -1; // no texture animation
		}*/

		m_Passes.push_back(pass);
	}



	// transparent parts come later
	sort(m_Passes.begin(), m_Passes.end());


	showGeosets = new bool[view->submeshes.size];
	for (uint32 i = 0; i < view->submeshes.size; i++)
	{
		showGeosets[i] = true;
	}

	indices = new uint16[view->indices.size];
	for (uint32 i = 0; i < view->indices.size; i++)
	{
		indices[i] = vertexesIndex[indexesIndex[i]];
	}


	//

	__geom = _Render->r->beginCreatingGeometry(_RenderStorage->__layoutMDX);

	// Vertex params
	_Render->r->setGeomVertexParams(__geom, m_ModelObject->__vb, 0, m_ModelObject->header.vertices.size * 0 * sizeof(float), 0);
	_Render->r->setGeomVertexParams(__geom, m_ModelObject->__vb, 1, m_ModelObject->header.vertices.size * 3 * sizeof(float), 0);
	_Render->r->setGeomVertexParams(__geom, m_ModelObject->__vb, 2, m_ModelObject->header.vertices.size * 5 * sizeof(float), 0);

	// Index bufer
	uint32 __ib = _Render->r->createIndexBuffer(view->indices.size * sizeof(uint16), indices);
	_Render->r->setGeomIndexParams(__geom, __ib, R_IndexFormat::IDXFMT_16);

	// Finish
	_Render->r->finishCreatingGeometry(__geom);

	delete[] indices;
}

Model_Skin::~Model_Skin()
{
	delete[] showGeosets;
}

//

void Model_Skin::Draw()
{
	_TechniquesMgr->m_MDX_GeometryPass->BindS();
	_TechniquesMgr->m_MDX_GeometryPass->SetPVW();
	
	_Render->r->setGeometry(__geom);

	for (size_t i = 0; i < m_Passes.size(); i++)
	{
		ModelRenderPass* p = m_Passes[i];

		if (/*p->init(m_ModelObject) && */showGeosets[p->m2SkinIndex])
		{
			p->__material.Set();

			_Render->r->drawIndexed(PRIM_TRILIST, p->indexStart, p->indexCount, p->vertexStart, p->vertexCount, false);
		}
	}

	_TechniquesMgr->m_MDX_GeometryPass->Unbind();
}
