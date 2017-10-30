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
	uint16*        texweightlookup=(uint16*)        (_mF.GetData() + _model->header.transparency_lookup_table.offset);

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
		if (_model->m_SpecialTextures[texlookup[skinBatch[j].texture_Index]] == -1)
		{
			pass->__material.SetDiffuseTexture(_model->m_Textures[texlookup[skinBatch[j].texture_Index]]->GetObj());
		}
		else
		{
			/*Texture* diffuseSpecialTexture = _model->m_TextureReplaced[_model->m_SpecialTextures[texlookup[skinBatch[j].texture_Index]]];

			if (diffuseSpecialTexture != nullptr)
			{
				pass->__material.SetDiffuseTexture(diffuseSpecialTexture->GetObj());
			}*/
		}
		
		// Render state
		pass->__material.SetRenderState(rf.flags.M2MATERIAL_FLAGS_TWOSIDED, rf.flags.M2MATERIAL_FLAGS_DEPTHWRITE == 0, rf.flags.M2MATERIAL_FLAGS_DEPTHTEST == 0);

		// Blend state
		pass->__blendMode = rf.blending_mode;
		switch (rf.blending_mode)
		{
			// 1 table
			case M2BLEND_OPAQUE:
			pass->__material.SetBlendEGxBlendIndex(0);
			break;

			case M2BLEND_ALPHA_KEY:
			pass->__material.SetBlendEGxBlendIndex(1);
			break;

			case M2BLEND_ALPHA:
			pass->__material.SetBlendEGxBlendIndex(2);
			break;

			case M2BLEND_NO_ALPHA_ADD:
			pass->__material.SetBlendEGxBlendIndex(10);
			break;

			case M2BLEND_ADD:
			pass->__material.SetBlendEGxBlendIndex(3);
			break;

			case M2BLEND_MOD:
			pass->__material.SetBlendEGxBlendIndex(4);
			break;

			case M2BLEND_MOD2X:
			pass->__material.SetBlendEGxBlendIndex(5);
			break;

			default:
			assert1(rf.blending_mode);
		}
		
		pass->__colorIndex = skinBatch[j].colorIndex;

		// Texture weight
		assert1(skinBatch[j].texture_WeightIndex != -1);
		assert1(m_ModelObject->header.texture_weights.size > 0);
		pass->__textureWeight = texweightlookup[skinBatch[j].texture_WeightIndex];

		// Anim texture
		if (skinBatch[j].flags.Flag_TextureStatic)
		{
			pass->__textureAnims = -1;
		}
		else
		{
			pass->__textureAnims = texanimlookup[skinBatch[j].texture_TransformIndex];
		}

		/*pass->order = skinBatch[j].shader_id;

		//Texture* texid = m_DiffuseTextures[texlookup[tex[j].textureid]];
		//pass->texture = texid;
		pass->tex = texlookup[skinBatch[j].texture_Index];

		// TODO: figure out these flags properly -_-
		


		pass->blendmode = rf.blending_mode;
		pass->color = skinBatch[j].colorIndex;
		pass->opacity = _model->transLookup[skinBatch[j].texture_WeightIndex];

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
				pass->texanim = texanimlookup[skinBatch[j].texture_TransformIndex];
			}
		}
		else
		{
			pass->texanim = -1; // no texture animation
		}*/

		m_Passes.push_back(pass);
	}



	// transparent parts come later
	//std::sort(m_Passes.begin(), m_Passes.end());


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

	__geom = _Render->r->beginCreatingGeometry(_RenderStorage->__layout_GxVBF_PNT);

	// Vertex params
	_Render->r->setGeomVertexParams(__geom, m_ModelObject->__vb, 0, m_ModelObject->header.vertices.size * 0 * sizeof(float), 0);
	_Render->r->setGeomVertexParams(__geom, m_ModelObject->__vb, 1, m_ModelObject->header.vertices.size * 3 * sizeof(float), 0);
	_Render->r->setGeomVertexParams(__geom, m_ModelObject->__vb, 2, m_ModelObject->header.vertices.size * 6 * sizeof(float), 0);

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
	_TechniquesMgr->m_Model->BindS();
	_TechniquesMgr->m_Model->SetPV_W();

	_Render->r->setGeometry(__geom);

	for (size_t i = 0; i < m_Passes.size(); i++)
	{
		ModelRenderPass* p = m_Passes[i];

		if (showGeosets[p->m2SkinIndex])
		{
			p->__material.Set();



			// Color
			if (p->__colorIndex != 65535)
			{
				_TechniquesMgr->m_Model->SetColor(m_ModelObject->m_Colors[p->__colorIndex].getValue());
			}
		    else
			{
				_TechniquesMgr->m_Model->SetColor(vec4(0.5f, 0.5f, 0.5f, 1.0f));
			}

			// Blend & Alpha
			_TechniquesMgr->m_Model->SetAlpha(1.0f);
			_TechniquesMgr->m_Model->SetBlendMode(p->__blendMode);

			// Texture weight
			_TechniquesMgr->m_Model->SetTextureWeight(m_ModelObject->m_TextureWeights[p->__textureWeight].getValue());

			// Texture anim
			_TechniquesMgr->m_Model->SetTextureAnimEnable(p->__textureAnims != -1);
			if (p->__textureAnims != -1)
			{
				_TechniquesMgr->m_Model->SetTextureAnimMatrix(m_ModelObject->m_TexturesAnims[p->__textureAnims].getValue());
			}
			else
			{
				//continue;
			}

			_Render->r->drawIndexed(PRIM_TRILIST, p->indexStart, p->indexCount, p->vertexStart, p->vertexCount, false);
		}
	}

	_TechniquesMgr->m_Model->Unbind();
}
