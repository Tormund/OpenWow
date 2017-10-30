#include "stdafx.h"

// General
#include "Material.h"

// Additional
#include "Render.h"
#include "RenderDevice.h"
#include "RenderStorage.h"

Material::Material()
{
	m_BlendEGxBlendIndex = -1;
	m_BlendEnabled = false;
	m_Blend_SrcFunc = BS_BLEND_SRC_ALPHA;
	m_Blend_DstFunc = BS_BLEND_INV_SRC_ALPHA;

	m_IsTwoSided = false;

	m_DepthTest = true;
	m_DepthWrite = true;

	m_DiffuseTexture = 0;
	m_SpecularTexture = 0;
}

void Material::Set()
{
	//_Render->r->setAlphaToCoverage(true);
	if (m_BlendEGxBlendIndex != -1)
	{
		_RenderStorage->SetEGxBlend(m_BlendEGxBlendIndex);
	}
	else
	{
		_Render->r->setBlendMode(m_BlendEnabled, m_Blend_SrcFunc, m_Blend_DstFunc);
	}
	_Render->r->setCullMode(m_IsTwoSided ? R_CullMode::RS_CULL_NONE : R_CullMode::RS_CULL_BACK);
	_Render->r->setDepthTest(m_DepthTest);
	_Render->r->setDepthMask(m_DepthWrite);

	if (m_DiffuseTexture != 0)
	{
		_Render->r->setTexture(C_DiffuseTextureIndex, m_DiffuseTexture, SS_FILTER_BILINEAR | SS_ANISO16 | SS_ADDR_WRAP, 0);
	}
	
	if (m_SpecularTexture != 0)
	{
		_Render->r->setTexture(C_SpecularTextureIndex, m_SpecularTexture, SS_FILTER_BILINEAR | SS_ANISO16 | SS_ADDR_WRAP, 0);
	}

}
