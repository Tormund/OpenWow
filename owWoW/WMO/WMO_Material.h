#pragma once

class WMO;

struct WMOMaterialDef
{
	struct
	{
		uint32 F_UNLIT : 1;                    // disable lighting logic in shader (but can still use vertex colors)
		uint32 F_UNFOGGED : 1;                 // disable fog shading (rarely used)
		uint32 F_UNCULLED : 1;                 // two-sided
		uint32 F_EXTLIGHT : 1;                 // darkened, the intern face of windows are flagged 0x08
		uint32 F_SIDN : 1;                     // (bright at night, unshaded) (used on windows and lamps in Stormwind, for example) (see emissive color)
		uint32 F_WINDOW : 1;                   // lighting related (flag checked in CMapObj::UpdateSceneMaterials)
		uint32 F_CLAMP_S : 1;                  // tex clamp S (force this material's m_DiffuseTextures to use clamp s addressing)
		uint32 F_CLAMP_T : 1;                  // tex clamp T (force this material's m_DiffuseTextures to use clamp t addressing)
		uint32 : 24;
	} flags;

	uint32 shader;                 // Index into CMapObj::s_wmoShaderMetaData. See below (shader types).
	uint32 blendMode;
	uint32 diffuseNameIndex;       // offset into MOTX
	CBgra emissive_color;        // emissive color; see below (emissive color)
	CBgra sidn_emissive_color;   // set at runtime; gets sidn-manipulated emissive color; see below (emissive color)
	uint32 envNameIndex;
	uint32 diffColor;
	uint32 ground_type;            //  &TerrainTypeRec::m_ID
	uint32 texture_2;
	uint32 color_2;
	uint32 flags_2;
	uint32 unk[4];

	// Size
	static const uint32 __size = 64;
};

class WMOMaterial
{
public:
	WMOMaterial(const WMO* _parentWMO, File& _file);
	~WMOMaterial();

	uint32 GetBlendMode() const { return matDef.blendMode; }
	uint32 GetDiffuseColor() const { return matDef.diffColor; }

	CBgra EmissiveColor() const { return matDef.emissive_color; }

	bool IsLightingDisabled() const { return matDef.flags.F_UNLIT; }
	bool IsFogDisabled() const { return matDef.flags.F_UNFOGGED; }
	bool IsTwoSided() const { return matDef.flags.F_UNCULLED; }
	bool IsDarkened() const { return matDef.flags.F_EXTLIGHT; }
	bool IsUnshaded() const { return matDef.flags.F_SIDN; }
	bool IsLightingRelated() const { return matDef.flags.F_WINDOW; }
	bool IsTexClampS() const { return matDef.flags.F_CLAMP_S; }
	bool IsTesClampT() const { return matDef.flags.F_CLAMP_T; }

private:
	const WMO* m_ParentWMO;

public:
	WMOMaterialDef matDef;
	Texture* texture;
};