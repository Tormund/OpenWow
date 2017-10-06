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

	uint32 nFlowvs;

	struct SWFlowv
	{
		CAaSphere sphere;
		vec3 dir;
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
			uint16 s;
			uint16 t;
			float height;
		} magmaVert;
	};
};

struct Liquid_Flag
{
	uint8 liquid : 6;
	uint8 fishable : 1;
	uint8 shared : 1;

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
	Liquid(uint32 x, uint32 y, vec3 base, float tilesize = LQ_DEFAULT_TILESIZE);
	~Liquid();

	void initFromTerrain(File& f, int flags);
	void initFromWMO(File& f, WMOMaterial* mat, bool indoor);

	void draw();


private:
	void initGeometry(File& f);
	void initTextures(const char *basename, int first, int last);

private:
	uint32 m_TilesX, m_TilesY;
	uint32 m_TilesCount;
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
