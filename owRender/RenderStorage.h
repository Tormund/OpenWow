#pragma once

struct Texture_Vertex
{
	vec2 vertex;
	vec2 textureCoord;
};

class RenderStorage : public Module
{
public:
	DEF_MODULE(RenderStorage);

	void CreateLayouts();

	void CreateGeometry();
	
	//----------------------------------------------

	// Geometry 

	uint32 __vbPos2;
	uint32 __geomPos2;

	uint32 __vb;
	uint32 __geom;

	uint32 __vbPos3;
	uint32 __geomPos3;

	//----------------------------------------------

	// Layouts UI
	uint32 __layoutPos2;
	uint32 __layoutFont;

	// Layouts geometry
	uint32 __layoutSky;
	uint32 __layoutMapLowResolution;
	uint32 __layoutMapChunk;
	uint32 __layoutWMO;
	uint32 __layoutWMO_VC;
	uint32 __layoutMDX;
	uint32 __layoutWater;
};


#define _RenderStorage RenderStorage::instance()