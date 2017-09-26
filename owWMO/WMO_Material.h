#pragma once

class WMO;

struct WMOMaterialDef
{
	struct
	{
		uint32_t F_UNLIT : 1;                    // disable lighting logic in shader (but can still use vertex colors)
		uint32_t F_UNFOGGED : 1;                 // disable fog shading (rarely used)
		uint32_t F_UNCULLED : 1;                 // two-sided
		uint32_t F_EXTLIGHT : 1;                 // darkened, the intern face of windows are flagged 0x08
		uint32_t F_SIDN : 1;                    // (bright at night, unshaded) (used on windows and lamps in Stormwind, for example) (see emissive color)
		uint32_t F_WINDOW : 1;                   // lighting related (flag checked in CMapObj::UpdateSceneMaterials)
		uint32_t F_CLAMP_S : 1;                  // tex clamp S (force this material's textures to use clamp s addressing)
		uint32_t F_CLAMP_T : 1;                  // tex clamp T (force this material's textures to use clamp t addressing)
		uint32_t flag_0x100 : 1;
		uint32_t : 23;
	} flags;

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

	// Size
	static const uint32_t __size = 64;
};

class WMOMaterial
{
public:
	WMOMaterial(const WMO* _parentWMO, File& _file);
	~WMOMaterial();

	void setup();

	void SetBlendMode();

	uint32_t GetBlendMode() const { return matDef.blendMode; }
	uint32_t GetDiffuseColor() const { return matDef.diffColor; }

	CImVector EmissiveColor() const { return matDef.emissive_color; }

	bool IsLightingDisabled() const { return matDef.flags.F_UNLIT; }
	bool IsFogDisabled() const { return matDef.flags.F_UNFOGGED; }
	bool IsTwoSided() const { return matDef.flags.F_UNCULLED; }
	bool IsDarkened() const { return matDef.flags.F_EXTLIGHT; }
	bool IsUnshaded() const { return matDef.flags.F_SIDN; }
	bool IsLightingRelated() const { return matDef.flags.F_WINDOW; }
	bool IsTexClampS() const { return matDef.flags.F_CLAMP_S; }
	bool IsTesClampT() const { return matDef.flags.F_CLAMP_T; }

	uint32_t GetShader() const { return matDef.shader; }

private:
	const WMO* m_ParentWMO;

public:
	WMOMaterialDef matDef;
	Texture* texture;
};