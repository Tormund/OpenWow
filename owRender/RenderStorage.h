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

	void CreateWoWLayouts();

	void SetEGxBlend(uint8 _index);
	
	//----------------------------------------------

	// Geometry

	uint32 __Quad;
	uint32 __QuadVT;

    uint32 __vbQuadVTDynamic;
    uint32 __QuadVTDynamic;


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
	uint32 __layoutWater;

	// Model
	uint32 __layout_GxVBF_PNT;
};


#define _RenderStorage RenderStorage::instance()