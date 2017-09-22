#ifndef LIQUID_H
#define LIQUID_H

class Liquid;

class WMOMaterial;

#include "maptile.h"

const float LQ_DEFAULT_TILESIZE = C_ChunkSize / 8.0f;

// handle liquids like oceans, lakes, rivers, slime, magma
class Liquid {

	int xtiles, ytiles;
	GLuint dlist;

	vec3 pos;

	float tilesize;
	float ydir;
	float texRepeats;

	void initGeometry(File& f);
	void initTextures(const char *basename, int first, int last);

	int type;
	
	vec3 col;
	int tmpflag;
	bool trans;

	int shader;

public:

	vector<Texture*> textures;

	Liquid(int x, int y, vec3 base, float tilesize = LQ_DEFAULT_TILESIZE):
		xtiles(x), ytiles(y), pos(base), tilesize(tilesize), shader(-1), ydir(1.0f)
	{
	}
	~Liquid();

	//void init(File& f);
	void initFromTerrain(File& f, int flags);
	void initFromWMO(File& f, WMOMaterial &mat, bool indoor);

	void draw();


};



#endif
