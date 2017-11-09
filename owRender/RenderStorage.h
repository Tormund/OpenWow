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

    uint32 CreateQuadIndicesBuffer(uint32 _quadCount);
	void SetEGxBlend(uint8 _index);
	
    //----------------------------------------------
    // Layouts
    //----------------------------------------------

    // Layouts UI
    uint32 __layoutV2;
    uint32 __layoutV2T2;

    // Layouts geometry
    uint32 __layoutSky;
    uint32 __layoutWMO;
    uint32 __layoutWMO_VC;
    uint32 __layoutWater;

    uint32 __layout_GxVBF_P;
    uint32 __layout_GxVBF_PBNT2;  // MDX
    uint32 __layout_GxVBF_PNC2T2; // Tiles

	//----------------------------------------------
	// Geometry & VertexBuffers
    //----------------------------------------------

	uint32 __Quad;
	uint32 __QuadVT;

    uint32 __vbQuadVTDynamic;
    uint32 __QuadVTDynamic;

    //----------------------------------------------
    // Index buffers
    //----------------------------------------------

    uint32 __ibQuadDefault;
};


#define _RenderStorage RenderStorage::instance()