#include "stdafx.h"

// General
#include "RenderStorage.h"

// Additional
#include "Render.h"
#include "RenderDevice.h"

bool RenderStorage::Init()
{
	CreateLayouts();

	CreateGeometry();

	return true;
}

void RenderStorage::Destroy()
{

}

void RenderStorage::CreateLayouts()
{
	R_VertexLayoutAttrib attribsV2[1] = {
		{"VertexPosition",      0, 2, 0}
	};
	__layoutV2 = _Render->r->registerVertexLayout(1, attribsV2);

	//

	R_VertexLayoutAttrib attribsV2T2[2] = {
		{"VertexPosition", 0, 2, 0},
		{"textureCoords",  1, 2, 0}
	};
	__layoutV2T2 = _Render->r->registerVertexLayout(2, attribsV2T2);

	//

	R_VertexLayoutAttrib attribsSky[2] = {
		{"VertexPosition", 0, 3, 0},
		{"color",          1, 3, 0}
	};
	__layoutSky = _Render->r->registerVertexLayout(2, attribsSky);
	//--------------------------------------------------------------------------------------------
	R_VertexLayoutAttrib attribsMapLowResolution[1] = {
		{"VertexPosition",      0, 3, 0}
	};
	__layoutMapLowResolution = _Render->r->registerVertexLayout(1, attribsMapLowResolution);
	//--------------------------------------------------------------------------------------------
	R_VertexLayoutAttrib attribsMapChunk[6] = {
		{"VertexPosition",       0, 3, 0},
		{"textureCoordsDetail",  1, 2, 0},
		{"textureCoordsAlpha",   2, 2, 0},
		{"normal",               3, 3, 0},
		{"colorMCCV",            4, 3, 0},
		{"colorMCLV",            5, 4, 0}
	};
	__layoutMapChunk = _Render->r->registerVertexLayout(6, attribsMapChunk);
	//--------------------------------------------------------------------------------------------
	R_VertexLayoutAttrib attribsWMO[3] = {
		{"VertexPosition",      0, 3, 0},
		{"textureCoords",  1, 2, 0},
		{"normal",         2, 3, 0}
	};
	__layoutWMO = _Render->r->registerVertexLayout(3, attribsWMO);

	R_VertexLayoutAttrib attribsWMO_VC[4] = {
		{"VertexPosition",      0, 3, 0},
		{"textureCoords",  1, 2, 0},
		{"normal",         2, 3, 0},
		{"color",          3, 4, 0}
	};
	__layoutWMO_VC = _Render->r->registerVertexLayout(4, attribsWMO_VC);
	//--------------------------------------------------------------------------------------------
	R_VertexLayoutAttrib attribsMDX[3] = {
		{"VertexPosition",      0, 3, 0},
		{"textureCoords",  1, 2, 0},
		{"normal",         2, 3, 0}
	};
	__layoutMDX = _Render->r->registerVertexLayout(3, attribsMDX);
	//--------------------------------------------------------------------------------------------
	R_VertexLayoutAttrib attribsWater[3] = {
		{"VertexPosition",      0, 3, 0},
		{"textureCoords",  1, 3, 0},
		{"normal",         2, 3, 0}
	};
	__layoutWater = _Render->r->registerVertexLayout(3, attribsWater);
}

void RenderStorage::CreateGeometry()
{
	// Indexes
	uint16 indexes[6] = {0, 1, 2, 2, 1, 3};
	uint32 __ib = _Render->r->createIndexBuffer(6 * sizeof(uint16), indexes);


	//========================================================================

	vector<vec2> verticesQuad;
	verticesQuad.push_back(vec2(-1.0f, -1.0f));
	verticesQuad.push_back(vec2(1.0f, -1.0f));
	verticesQuad.push_back(vec2(-1.0f, 1.0f));
	verticesQuad.push_back(vec2(1.0f, 1.0f));

	uint32 __vbQuad = _Render->r->createVertexBuffer(verticesQuad.size() * sizeof(vec2), &verticesQuad[0]);
	__Quad = _Render->r->beginCreatingGeometry(__layoutV2);

	_Render->r->setGeomVertexParams(__Quad, __vbQuad, 0, 0, sizeof(vec2));
	_Render->r->setGeomIndexParams(__Quad, __ib, R_IndexFormat::IDXFMT_16);

	_Render->r->finishCreatingGeometry(__Quad);

	//--

	vector<Texture_Vertex> verticesQuadVT;
	verticesQuadVT.push_back({vec2(-1.0f, -1.0f), vec2(0.0f, 0.0f)});
	verticesQuadVT.push_back({vec2(1.0f, -1.0f), vec2(1.0f, 0.0f)});
	verticesQuadVT.push_back({vec2(-1.0f, 1.0f), vec2(0.0f, 1.0f)});
	verticesQuadVT.push_back({vec2(1.0f, 1.0f), vec2(1.0f, 1.0f)});

	uint32 __vbQuadVT = _Render->r->createVertexBuffer(verticesQuadVT.size() * sizeof(Texture_Vertex), verticesQuadVT.data());
	__QuadVT = _Render->r->beginCreatingGeometry(__layoutV2T2);

	_Render->r->setGeomVertexParams(__QuadVT, __vbQuadVT, 0, 0,            sizeof(Texture_Vertex));
	_Render->r->setGeomVertexParams(__QuadVT, __vbQuadVT, 1, sizeof(vec2), sizeof(Texture_Vertex));

	_Render->r->setGeomIndexParams(__QuadVT, __ib, R_IndexFormat::IDXFMT_16);

	_Render->r->finishCreatingGeometry(__QuadVT);
}