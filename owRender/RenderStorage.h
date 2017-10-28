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

	uint32 __Quad;
	uint32 __QuadVT;

	//----------------------------------------------

	// Layouts UI
	uint32 __layoutV2;
	uint32 __layoutV2T2;

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