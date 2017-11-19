#include "stdafx.h"

// General
#include "TechniquesManager.h"

// Additional
#include "Material.h"

TechniquesMgr::TechniquesMgr()
{
	m_MapChunk_GeometryPass = nullptr;
	m_MapTileLowRes_GeometryPass = nullptr;
	m_WMO_GeometryPass = nullptr;
	m_POST_Simple = nullptr;
}

TechniquesMgr::~TechniquesMgr()
{
	delete m_MapChunk_GeometryPass;
	delete m_MapTileLowRes_GeometryPass;
	delete m_WMO_GeometryPass;

	delete m_POST_Simple;
}

void TechniquesMgr::Init()
{
	m_Debug_GeometryPass = new Debug_GeometryPass();

    m_Debug_Normals = new Debug_Normals();

	//

	m_Sky_GeometryPass = new Sky_GeometryPass();

	//----------------------------------------------------------------//

	m_MapChunk_GeometryPass = new MapChunk_GeometryPass();
	m_MapChunk_GeometryPass->BindS();

    for (uint8 i = 0; i < 4; i++)
    {
        m_MapChunk_GeometryPass->SetColorTextureUnit(i, i);
        m_MapChunk_GeometryPass->SetSpecularTextureUnit(i, 5 + i);
    }
	m_MapChunk_GeometryPass->SetBlendBuffer(4);

	m_MapChunk_GeometryPass->Unbind();

	//----------------------------------------------------------------//

	m_MapTileLowRes_GeometryPass = new MapTileLowRes_GeometryPass();

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

	m_Model = new Model_Shader();
	m_Model->BindS();
	m_Model->SetDiffuseTexture(Material::C_DiffuseTextureIndex);
	m_Model->SetSpecularTexture(Material::C_SpecularTextureIndex);
	m_Model->Unbind();

	//----------------------------------------------------------------//

	m_POST_DirectionalLight = new POST_DirectionalLight();
	m_POST_DirectionalLight->BindS();

	m_POST_DirectionalLight->SetProjectionMatrix(mat4());
	m_POST_DirectionalLight->SetViewMatrix(mat4());
	m_POST_DirectionalLight->SetWorldMatrix(mat4());

	m_POST_DirectionalLight->BindToPostprocess();

	m_POST_DirectionalLight->SetScreenSize(_Config.windowSizeX, _Config.windowSizeY);
	m_POST_DirectionalLight->SetMatSpecularPower(64);

	m_POST_DirectionalLight->Unbind();

    //----------------------------------------------------------------//

    m_POST_Fog = new POST_Fog();
    m_POST_Fog->BindS();

    m_POST_Fog->SetProjectionMatrix(mat4());
    m_POST_Fog->SetViewMatrix(mat4());
    m_POST_Fog->SetWorldMatrix(mat4());

    m_POST_Fog->BindToPostprocess();

    m_POST_Fog->SetScreenSize(_Config.windowSizeX, _Config.windowSizeY);

    m_POST_Fog->Unbind();
	//----------------------------------------------------------------//

	m_POST_Simple = new POST_Simple();
	m_POST_Simple->BindS();

	m_POST_Simple->SetProjectionMatrix(mat4());
	m_POST_Simple->SetViewMatrix(mat4());
	m_POST_Simple->SetWorldMatrix(mat4());

	m_POST_Simple->BindToPostprocess();

	m_POST_Simple->SetScreenSize(_Config.windowSizeX, _Config.windowSizeY);
	
	m_POST_Simple->Unbind();

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
