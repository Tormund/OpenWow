#pragma once

#include "WowTypes.h"

class WMOMaterial
{
	struct __WMOMaterial
	{
		/*uint32_t F_UNLIT : 1;                    // disable lighting logic in shader (but can still use vertex colors)
		uint32_t F_UNFOGGED : 1;                 // disable fog shading (rarely used)
		uint32_t F_UNCULLED : 1;                 // two-sided
		uint32_t F_EXTLIGHT : 1;                 // darkened, the intern face of windows are flagged 0x08
		uint32_t F_SIDN : 1;                    // (bright at night, unshaded) (used on windows and lamps in Stormwind, for example) (see emissive color)
		uint32_t F_WINDOW : 1;                   // lighting related (flag checked in CMapObj::UpdateSceneMaterials)
		uint32_t F_CLAMP_S : 1;                  // tex clamp S (force this material's textures to use clamp s addressing)
		uint32_t F_CLAMP_T : 1;                  // tex clamp T (force this material's textures to use clamp t addressing)*/

		uint32_t flags;
		uint32_t shader;                 // Index into CMapObj::s_wmoShaderMetaData. See below (shader types).
		uint32_t blendMode;              // Blending: see Blend_State_Table
		uint32_t diffuseNameIndex;       // offset into MOTX
		CImVector emissive_color;        // emissive color; see below (emissive color)
		CImVector sidn_emissive_color;   // set at runtime; gets sidn-manipulated emissive color; see below (emissive color)
		uint32_t envNameIndex;
		uint32_t diffColor;
		uint32_t ground_type;            //  &TerrainTypeRec::m_ID
		uint32_t texture_2;
		uint32_t color_2;
		uint32_t flags_2;
		uint32_t runTimeData[4];         // This data is explicitly nulled upon loading. Contains textures or similar stuff.
	};

public:
	WMOMaterial(File& _file);
	~WMOMaterial();

	void initTexture(const char* _texturesFileNames);
	void setup();

	void SetBlendMode();

	uint32_t GetBlendMode() const { return data->blendMode; }
	uint32_t GetDiffuseColor() const { return data->diffColor; }

	CImVector EmissiveColor() const { return data->emissive_color; }

	bool IsLightingDisabled() const { return data->flags & 0x01; }
	bool IsFogDisabled() const { return data->flags & 0x02; }
	bool IsTwoSided() const { return data->flags & 0x04; }
	bool IsDarkened() const { return data->flags & 0x08; }
	bool IsUnshaded() const { return data->flags & 0x10; }
	bool IsLightingRelated() const { return data->flags & 0x20; }
	bool IsTexClampS() const { return data->flags & 0x40; }
	bool IsTesClampT() const { return data->flags & 0x80; }

	uint32_t GetShader() const { return data->shader; }

private:
	__WMOMaterial* data;

	Texture* texture;
};