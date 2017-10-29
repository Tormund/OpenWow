#include "stdafx.h"

// General
#include "TechniquesManager.h"

// Additional
#include "Material.h"

TechniquesMgr::TechniquesMgr()
{
	m_MapChunk_GeometryPass = nullptr;
	m_MapTileLowRes_GeometryPass = nullptr;
	m_MDX_GeometryPass = nullptr;
	m_WMO_GeometryPass = nullptr;
	m_SimpleRender = nullptr;
}

TechniquesMgr::~TechniquesMgr()
{
	delete m_MapChunk_GeometryPass;
	delete m_MapTileLowRes_GeometryPass;
	delete m_MDX_GeometryPass;
	delete m_WMO_GeometryPass;

	delete m_SimpleRender;
}

void TechniquesMgr::Init()
{
	m_Debug_GeometryPass = new Debug_GeometryPass();

	//

	m_Sky_GeometryPass = new Sky_GeometryPass();

	//----------------------------------------------------------------//

	m_MapChunk_GeometryPass = new MapChunk_GeometryPass();
	m_MapChunk_GeometryPass->BindS();

	m_MapChunk_GeometryPass->SetColorTextureUnit0(0);
	m_MapChunk_GeometryPass->SetColorTextureUnit1(1);
	m_MapChunk_GeometryPass->SetColorTextureUnit2(2);
	m_MapChunk_GeometryPass->SetColorTextureUnit3(3);

	m_MapChunk_GeometryPass->SetBlendBuffer(4);

	m_MapChunk_GeometryPass->SetSpecularTextureUnit0(5);
	m_MapChunk_GeometryPass->SetSpecularTextureUnit1(6);
	m_MapChunk_GeometryPass->SetSpecularTextureUnit2(7);
	m_MapChunk_GeometryPass->SetSpecularTextureUnit3(8);

	m_MapChunk_GeometryPass->Unbind();

	//----------------------------------------------------------------//

	m_MapTileLowRes_GeometryPass = new MapTileLowRes_GeometryPass();

	//----------------------------------------------------------------//

	m_MDX_GeometryPass = new MDX_GeometryPass();
	m_MDX_GeometryPass->BindS();
	m_MDX_GeometryPass->SetColorTextureUnit(Material::C_DiffuseTextureIndex);
	m_MDX_GeometryPass->SetSpecularTextureUnit(Material::C_SpecularTextureIndex);
	m_WMO_GeometryPass->Unbind();

	//

	m_Water = new Water_Pass();
	m_Water->BindS();
	m_Water->SetColorTextureUnit(Material::C_DiffuseTextureIndex);
	m_Water->SetSpecularTextureUnit(Material::C_SpecularTextureIndex);
	m_Water->Unbind();

	//----------------------------------------------------------------//

	m_WMO_GeometryPass = new WMO_GeomertyPass();
	m_WMO_GeometryPass->BindS();
	m_WMO_GeometryPass->SetColorTextureUnit(Material::C_DiffuseTextureIndex);
	m_WMO_GeometryPass->SetSpecularTextureUnit(Material::C_SpecularTextureIndex);
	m_WMO_GeometryPass->Unbind();

	//----------------------------------------------------------------//

	//----------------------------------------------------------------//
	m_DSDirLightPassTech = new DSDirLightPassTech();
	m_DSDirLightPassTech->BindS();

	m_DSDirLightPassTech->SetProjectionMatrix(mat4());
	m_DSDirLightPassTech->SetViewMatrix(mat4());
	m_DSDirLightPassTech->SetWorldMatrix(mat4());

	m_DSDirLightPassTech->BindToPostprocess();

	m_DSDirLightPassTech->SetScreenSize(Modules::config().windowSizeX, Modules::config().windowSizeY);
	m_DSDirLightPassTech->SetMatSpecularPower(64);

	m_DSDirLightPassTech->Unbind();

	//----------------------------------------------------------------//

	m_SimpleRender = new SimpleRenderGBuffer();
	m_SimpleRender->BindS();

	m_SimpleRender->SetProjectionMatrix(mat4());
	m_SimpleRender->SetViewMatrix(mat4());
	m_SimpleRender->SetWorldMatrix(mat4());

	m_SimpleRender->BindToPostprocess();

	m_SimpleRender->SetScreenSize(Modules::config().windowSizeX, Modules::config().windowSizeY);
	
	m_SimpleRender->Unbind();

	//

	m_UI_Color = new UI_Color();

	//

	m_UI_Font = new UI_Font();
	m_UI_Font->BindS();
	m_UI_Font->SetFontTexture(Material::C_DiffuseTextureIndex);
	m_UI_Font->Unbind();

	//

	m_UI_Texture = new UI_Texture();
	m_UI_Texture->BindS();
	m_UI_Texture->SetTexture(Material::C_DiffuseTextureIndex);
	m_UI_Texture->Unbind();
}
