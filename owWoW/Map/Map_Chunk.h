#pragma once

// Includes and uses
#include "../Liquid.h"
#include "Map_Chunk_Types.h"

class MapTile;

struct MCLY
{
	uint32 textureIndex;
	struct
	{
		uint32 animation_rotation : 3;        // each tick is 45
		uint32 animation_speed : 3;
		uint32 animation_enabled : 1;
		uint32 overbright : 1;                // This will make the texture way brighter. Used for lava to make it "glow".
		uint32 use_alpha_map : 1;             // set for every layer after the first
		uint32 alpha_map_compressed : 1;      // see MCAL chunk description
		uint32 use_cube_map_reflection : 1;   // This makes the layer behave like its a reflection of the skybox. See below
		uint32 : 21;
	} flags;
	uint32 offsetInMCAL;
	__DBC_FOREIGN_KEY_ID(uint16, DBC_GroundEffectTexture, effectId);
	int16 unk0;
};

class MapChunk
{
public:
	MapChunk(MapTile* _parentTile);
	~MapChunk();

	void Load(File& f, load_phases _phase);
	void Post_Load();

	void Render();
	void drawPass(int anim);

	//

	void CreateMH2OLiquid(File& f, MH2O_Header* _liquidHeader);
	MCNK_MCLQ_LiquidType GetLiquidType();

public:
	MapTile* m_ParentTile;
	float m_GamePositionX, m_GamePositionY, m_GamePositionZ;

	int areaID;

	bool visible;
	bool hasholes;
	float waterlevel[2];


	MCLY mcly[4];
	int animated[4];

	Texture* m_DiffuseTextures[4];
	Texture* m_SpecularTextures[4];

	uint32 m_BlendRBGShadowATexture;

	uint32 __geom;

	uint16* m_Indexes;
	uint16  m_IndexesCount;

	Liquid* m_Liquid;

	BoundingBox m_Bounds;

private:
	MCNK_Header header;
	bool MCLV_exists;
	bool colorBufferEnable;
};