#pragma once

class WMOMaterial;

struct LiquidInfo
{
	CRange height;
	struct SLVert
	{
		union
		{
			struct SWVert
			{
				char depth;
				char flow0Pct;
				char flow1Pct;
				char filler;
				float height;
			} waterVert;

			struct SOVert
			{
				char depth;
				char foam;
				char wet;
				char filler;
			} oceanVert;

			struct SMVert
			{
				unsigned __int16 s;
				unsigned __int16 t;
				float height;
			} magmaVert;
		};
	} verts[9 * 9];

	struct SLTiles
	{
		char tiles[8][8]; // 0x0f or 0x8 mean don't render (?)
	} tiles;

	uint32_t nFlowvs;

	struct SWFlowv
	{
		CAaSphere sphere;
		C3Vector dir;
		float velocity;
		float amplitude;
		float frequency;
	} flowvs[2]; // always 2 in file, independent on nFlowvs.
};



struct Liquid_Vertex
{
	union
	{
		struct SWVert
		{
			char depth;
			char flow0Pct;
			char flow1Pct;
			char filler;
			float height;
		} waterVert;

		struct SOVert
		{
			char depth;
			char foam;
			char wet;
			char filler;
		} oceanVert;

		struct SMOMagmaVert
		{
			uint16_t s;
			uint16_t t;
			float height;
		} magmaVert;
	};
};

struct Liquid_Flag
{
	uint8_t liquid : 6;
	uint8_t fishable : 1;
	uint8_t shared : 1;

	// 0x01
	// 0x02
	// 0x04
	// 0x08
	// 0x10
	// 0x20

	// 0x40
	// 0x80
};

//

class Liquid
{
public:
	Liquid(uint32_t x, uint32_t y, vec3 base, float tilesize = LQ_DEFAULT_TILESIZE);
	~Liquid();

	void initFromTerrain(File& f, int flags);
	void initFromWMO(File& f, WMOMaterial* mat, bool indoor);

	void draw();


private:
	void initGeometry(File& f);
	void initTextures(const char *basename, int first, int last);

private:
	uint32_t m_TilesX, m_TilesY;
	uint32_t m_TilesCount;
	Liquid_Flag m_LastFlag;
	GLuint m_OGLList;


	vector<Texture*> textures;

	
	

	vec3 pos;

	float tilesize;
	float ydir;
	float texRepeats;



	int type;

	vec3 col;
	
	bool trans;

	int shader;
};
