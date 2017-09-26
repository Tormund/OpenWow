#pragma once

class WMOMaterial;

struct Liquid_Vertex
{
	union
	{
		struct SMOWaterVert
		{
			uint8_t flow1;
			uint8_t flow2;
			uint8_t flow1Pct;
			uint8_t filler;
			float height;
		}  waterVert;

		struct SMOMagmaVert
		{
			int16_t s;
			int16_t t;
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
