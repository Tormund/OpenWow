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
	R_VertexLayoutAttrib attribsPos2[1] = {
		{"VertexPosition",      0, 2, 0}
	};
	__layoutPos2 = _Render->r->registerVertexLayout(1, attribsPos2);

	//

	R_VertexLayoutAttrib attribsModel3[2] = {
		{"vertexPos",      0, 2, 0},
		{"textureCoords",  1, 2, 0}
	};
	__layoutFont = _Render->r->registerVertexLayout(2, attribsModel3);

	//

	R_VertexLayoutAttrib attribsSky[2] = {
		{"vertexPos",      0, 3, 0},
		{"color",          1, 3, 0}
	};
	__layoutSky = _Render->r->registerVertexLayout(2, attribsSky);
	//--------------------------------------------------------------------------------------------
	R_VertexLayoutAttrib attribsMapLowResolution[1] = {
		{"vertexPos",      0, 3, 0}
	};
	__layoutMapLowResolution = _Render->r->registerVertexLayout(1, attribsMapLowResolution);
	//--------------------------------------------------------------------------------------------
	R_VertexLayoutAttrib attribsMapChunk[6] = {
		{"vertexPos",            0, 3, 0},
		{"textureCoordsDetail",  1, 2, 0},
		{"textureCoordsAlpha",   2, 2, 0},
		{"normal",               3, 3, 0},
		{"colorMCCV",            4, 3, 0},
		{"colorMCLV",            5, 4, 0}
	};
	__layoutMapChunk = _Render->r->registerVertexLayout(6, attribsMapChunk);
	//--------------------------------------------------------------------------------------------
	R_VertexLayoutAttrib attribsWMO[3] = {
		{"vertexPos",      0, 3, 0},
		{"textureCoords",  1, 2, 0},
		{"normal",         2, 3, 0}
	};
	__layoutWMO = _Render->r->registerVertexLayout(3, attribsWMO);

	R_VertexLayoutAttrib attribsWMO_VC[4] = {
		{"vertexPos",      0, 3, 0},
		{"textureCoords",  1, 2, 0},
		{"normal",         2, 3, 0},
		{"color",          3, 4, 0}
	};
	__layoutWMO_VC = _Render->r->registerVertexLayout(4, attribsWMO_VC);
	//--------------------------------------------------------------------------------------------
	R_VertexLayoutAttrib attribsMDX[3] = {
		{"vertexPos",      0, 3, 0},
		{"textureCoords",  1, 2, 0},
		{"normal",         2, 3, 0}
	};
	__layoutMDX = _Render->r->registerVertexLayout(3, attribsMDX);
	//--------------------------------------------------------------------------------------------
	R_VertexLayoutAttrib attribsWater[3] = {
		{"vertexPos",      0, 3, 0},
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

	//

	//-- Pos2

	__vbPos2 = _Render->r->createVertexBuffer(4 * sizeof(vec2), nullptr);
	__geomPos2 = _Render->r->beginCreatingGeometry(__layoutPos2);

	_Render->r->setGeomVertexParams(__geomPos2, __vbPos2, 0, 0, 0);
	_Render->r->setGeomIndexParams(__geomPos2, __ib, R_IndexFormat::IDXFMT_16);

	_Render->r->finishCreatingGeometry(__geomPos2);


	//-- Pos3

	__vbPos3 = _Render->r->createVertexBuffer(4 * sizeof(vec2), nullptr);
	__geomPos3 = _Render->r->beginCreatingGeometry(__layoutPos2);

	_Render->r->setGeomVertexParams(__geomPos3, __vbPos3, 0, 0, 0);

	uint16 indexes3[8] = {0, 1, 1, 2, 2, 3, 3, 0};
	uint32 __ibPos3 = _Render->r->createIndexBuffer(8 * sizeof(uint16), indexes3);
	_Render->r->setGeomIndexParams(__geomPos3, __ibPos3, R_IndexFormat::IDXFMT_16);

	_Render->r->finishCreatingGeometry(__geomPos3);

	//

	//-- Pos2 Texture2

	__vb = _Render->r->createVertexBuffer(4 * sizeof(Texture_Vertex), nullptr);
	__geom = _Render->r->beginCreatingGeometry(__layoutFont);

	_Render->r->setGeomVertexParams(__geom, __vb, 0, 0, sizeof(Texture_Vertex));
	_Render->r->setGeomVertexParams(__geom, __vb, 1, sizeof(vec2), sizeof(Texture_Vertex));

	_Render->r->setGeomIndexParams(__geom, __ib, R_IndexFormat::IDXFMT_16);

	_Render->r->finishCreatingGeometry(__geom);
}