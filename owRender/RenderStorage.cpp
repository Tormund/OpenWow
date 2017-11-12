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

	CreateWoWLayouts();

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
	R_VertexLayoutAttrib attribsWater[3] = {
		{"VertexPosition", 0, 3, 0},
		{"textureCoords",  1, 3, 0},
		{"normal",         2, 3, 0}
	};
	__layoutWater = _Render->r->registerVertexLayout(3, attribsWater);
}

void RenderStorage::CreateGeometry()
{
	// Indexes
	uint16 indexes[6] = {0, 1, 2, 2, 1, 3};
    __ibQuadDefault = _Render->r->createIndexBuffer(sizeof(indexes), indexes);

    vector<vec2> verticesQuad;
    verticesQuad.push_back(vec2(-1.0f, -1.0f));
    verticesQuad.push_back(vec2(1.0f, -1.0f));
    verticesQuad.push_back(vec2(-1.0f, 1.0f));
    verticesQuad.push_back(vec2(1.0f, 1.0f));

    vector<vec2> texCoordsQuad;
    texCoordsQuad.push_back(vec2(0.0f, 0.0f));
    texCoordsQuad.push_back(vec2(1.0f, 0.0f));
    texCoordsQuad.push_back(vec2(0.0f, 1.0f));
    texCoordsQuad.push_back(vec2(1.0f, 1.0f));

    vector<Texture_Vertex> verticesQuadVT;
    verticesQuadVT.push_back({vec2(-1.0f, -1.0f), vec2(0.0f, 0.0f)});
    verticesQuadVT.push_back({vec2(1.0f, -1.0f), vec2(1.0f, 0.0f)});
    verticesQuadVT.push_back({vec2(-1.0f, 1.0f), vec2(0.0f, 1.0f)});
    verticesQuadVT.push_back({vec2(1.0f, 1.0f), vec2(1.0f, 1.0f)});

	// -- Simple vertex Geom

	uint32 __vbQuad = _Render->r->createVertexBuffer(verticesQuad.size() * sizeof(vec2), &verticesQuad[0]);
	__Quad = _Render->r->beginCreatingGeometry(__layoutV2);

	_Render->r->setGeomVertexParams(__Quad, __vbQuad, R_DataType::T_FLOAT, 0, sizeof(vec2));
	_Render->r->setGeomIndexParams(__Quad, __ibQuadDefault, R_IndexFormat::IDXFMT_16);

	_Render->r->finishCreatingGeometry(__Quad);

	//-- Vertex + TextureCoords

	uint32 __vbQuadVT = _Render->r->createVertexBuffer(verticesQuadVT.size() * sizeof(Texture_Vertex), verticesQuadVT.data());
	__QuadVT = _Render->r->beginCreatingGeometry(__layoutV2T2);

	_Render->r->setGeomVertexParams(__QuadVT, __vbQuadVT, R_DataType::T_FLOAT, 0, sizeof(Texture_Vertex));
	_Render->r->setGeomVertexParams(__QuadVT, __vbQuadVT, R_DataType::T_FLOAT, sizeof(vec2), sizeof(Texture_Vertex));

	_Render->r->setGeomIndexParams(__QuadVT, __ibQuadDefault, R_IndexFormat::IDXFMT_16);

	_Render->r->finishCreatingGeometry(__QuadVT);

    //-- Vertex + TextureCoords Dynamic

    __vbQuadVTDynamic = _Render->r->createVertexBuffer(verticesQuad.size() * sizeof(Texture_Vertex), nullptr);
    _Render->r->updateBufferData(__vbQuadVTDynamic, 4 * 0 * sizeof(float), 4 * sizeof(vec2), verticesQuad.data());
    _Render->r->updateBufferData(__vbQuadVTDynamic, 4 * 2 * sizeof(float), 4 * sizeof(vec2), texCoordsQuad.data());

    __QuadVTDynamic = _Render->r->beginCreatingGeometry(__layoutV2T2);

    _Render->r->setGeomVertexParams(__QuadVTDynamic, __vbQuadVTDynamic, R_DataType::T_FLOAT, 0,                0);
    _Render->r->setGeomVertexParams(__QuadVTDynamic, __vbQuadVTDynamic, R_DataType::T_FLOAT, 4 * sizeof(vec2), 0);

    _Render->r->setGeomIndexParams(__QuadVTDynamic, __ibQuadDefault, R_IndexFormat::IDXFMT_16);

    _Render->r->finishCreatingGeometry(__QuadVTDynamic);
}

void RenderStorage::CreateWoWLayouts()
{
	R_VertexLayoutAttrib attribs_GxVBF_P[1] = { // 12
		{"position",    0, 3, 0}
	};
    __layout_GxVBF_P = _Render->r->registerVertexLayout(1, attribs_GxVBF_P); // USED IN LOW-RESOLUTION TILES

	//--

	R_VertexLayoutAttrib attribs_GxVBF_PN[2] = { // 24
		{"position",    0, 3, 0},
		{"normal",      1, 3, 0}
	};

	R_VertexLayoutAttrib attribs_GxVBF_PNC[3] = { // 28
		{"position",    0, 3, 0},
		{"normal",      1, 3, 0},
		{"color",       2, 1, 0}
	};

	//--

	R_VertexLayoutAttrib attribs_GxVBF_PNT[3] = { // 32
		{"position",    0, 3, 0},
		{"normal",      1, 3, 0},
		{"tc",          2, 2, 0}
	};

	R_VertexLayoutAttrib attribs_GxVBF_PNCT[4] = { // 36
		{"position",    0, 3, 0},
		{"normal",      1, 3, 0},
		{"color",       2, 1, 0},
		{"tc",          3, 2, 0}
	};

	//--

	R_VertexLayoutAttrib attribs_GxVBF_PNT2[4] = { // 40
		{"position",    0, 3, 0},
		{"normal",      1, 3, 0},
		{"tc0",         2, 2, 0},
		{"tc1",         3, 2, 0}
	};

	R_VertexLayoutAttrib attribs_GxVBF_PNCT2[5] = { // 44
		{"position",    0, 3, 0},
		{"normal",      1, 3, 0},
		{"color",       2, 1, 0},
		{"tc0",         3, 2, 0},
		{"tc1",         4, 2, 0}
	};

	//--

	R_VertexLayoutAttrib attribs_GxVBF_PC[2] = { // 16
		{"position",    0, 3, 0},
		{"color",       1, 1, 0}
	};

	R_VertexLayoutAttrib attribs_GxVBF_PCT[3] = { // 24
		{"position",    0, 3, 0},
		{"color",       1, 1, 0},
		{"tc",          2, 2, 0}
	};

	R_VertexLayoutAttrib attribs_GxVBF_PCT2[4] = { // 32
		{"position",    0, 3, 0},
		{"color",       1, 1, 0},
		{"tc0",         2, 2, 0},
		{"tc1",         3, 2, 0}
	};

	//--

	R_VertexLayoutAttrib attribs_GxVBF_PT[2] = { // 20
		{"position",    0, 3, 0},
		{"tc",          1, 2, 0}
	};

	R_VertexLayoutAttrib attribs_GxVBF_PT2[3] = { // 28
		{"position",    0, 3, 0},
		{"tc0",         1, 2, 0},
		{"tc0",         2, 2, 0}
	};

	//--

	R_VertexLayoutAttrib attribs_GxVBF_PBNT2[6] = { // 48
		{"position",    0, 3, 0},
		{"blendWeight", 1, 1, 0},
		{"blendIndices",2, 1, 0},
		{"normal",      3, 3, 0},
		{"tc0",         4, 2, 0},
		{"tc1",         5, 2, 0}
	};
    __layout_GxVBF_PBNT2 = _Render->r->registerVertexLayout(6, attribs_GxVBF_PBNT2); // USED IN M2

	R_VertexLayoutAttrib attribs_GxVBF_PNC2T2[6] = { // 72  // Original 48
		{"position",    0, 3, 0},
		{"normal",      1, 3, 0},
		{"color0",      2, 4, 0}, // Original 1
		{"color1",      3, 4, 0}, // Original 1
		{"tc0",         4, 2, 0},
		{"tc1",         5, 2, 0}
	};
    __layout_GxVBF_PNC2T2 = _Render->r->registerVertexLayout(6, attribs_GxVBF_PNC2T2); // USED IN MapChunk
}

//

uint32 RenderStorage::CreateQuadIndicesBuffer(uint32 _quadCount)
{
    // {0, 1, 2, 2, 1, 3}

    uint16* indexes = new uint16[6 * _quadCount];
    uint16 j = 0;
    for (uint32 i = 0; i < _quadCount * 6; i += 6)
    {
        indexes[i + 0] = 0 + j;
        indexes[i + 1] = 1 + j;
        indexes[i + 2] = 2 + j;
        indexes[i + 3] = 2 + j;
        indexes[i + 4] = 1 + j;
        indexes[i + 5] = 3 + j;

        j += 4;
    }
    return _Render->r->createIndexBuffer(6 * _quadCount * sizeof(uint16), indexes);
}

void RenderStorage::SetEGxBlend(uint8 _index)
{
	switch (_index)
	{
		case 0: // Opaque
		_Render->r->setBlendMode(false, BS_BLEND_ONE,             BS_BLEND_ZERO);
		break;

		case 1: // AlphaKey
		_Render->r->setBlendMode(false, BS_BLEND_ONE,             BS_BLEND_ZERO);
		break;

		case 2: // Alpha
		_Render->r->setBlendModeEx(false, BS_BLEND_SRC_ALPHA,     BS_BLEND_INV_SRC_ALPHA,   BS_BLEND_ONE,              BS_BLEND_INV_SRC_ALPHA);
		break;

		case 3: // Add
		_Render->r->setBlendModeEx(true, BS_BLEND_SRC_ALPHA,      BS_BLEND_ONE,             BS_BLEND_ZERO,             BS_BLEND_ONE);
		break;

		case 4: // Mod
		_Render->r->setBlendModeEx(true, BS_BLEND_DEST_COLOR,     BS_BLEND_ZERO,            BS_BLEND_DEST_ALPHA,       BS_BLEND_ZERO);
		break;

		case 5: // Mod2x
		_Render->r->setBlendModeEx(true, BS_BLEND_DEST_COLOR,     BS_BLEND_SRC_COLOR,       BS_BLEND_DEST_ALPHA,       BS_BLEND_SRC_ALPHA);
		break;

		case 6: // ModAdd
		_Render->r->setBlendModeEx(true, BS_BLEND_DEST_COLOR,     BS_BLEND_ONE,             BS_BLEND_DEST_ALPHA,       BS_BLEND_ONE);
		break;

		case 7: // InvSrcAlphaAdd
		_Render->r->setBlendModeEx(true, BS_BLEND_INV_SRC_ALPHA,  BS_BLEND_ONE,             BS_BLEND_INV_SRC_ALPHA,    BS_BLEND_ONE);
		break;

		case 8: // InvSrcAlphaOpaque
		_Render->r->setBlendModeEx(true, BS_BLEND_INV_SRC_ALPHA,  BS_BLEND_ZERO,            BS_BLEND_INV_SRC_ALPHA,    BS_BLEND_ZERO);
		break;

		case 9: // SrcAlphaOpaque
		_Render->r->setBlendModeEx(true, BS_BLEND_SRC_ALPHA,      BS_BLEND_ZERO,            BS_BLEND_SRC_ALPHA,        BS_BLEND_ZERO);
		break;

		case 10: // NoAlphaAdd
		_Render->r->setBlendModeEx(true, BS_BLEND_ONE,            BS_BLEND_ONE,             BS_BLEND_ZERO,             BS_BLEND_ONE);
		break;

		case 11: // ConstantAlpha
		//GLSetBlend(true, GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA, GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);
		fail1("Not implemented!!!");
		break;

		case 12: // Screen
		_Render->r->setBlendModeEx(true, BS_BLEND_INV_DEST_COLOR, BS_BLEND_ONE,             BS_BLEND_ONE,              BS_BLEND_ZERO);
		break;

		case 13: // BlendAdd
		_Render->r->setBlendModeEx(true, BS_BLEND_ONE,            BS_BLEND_INV_SRC_ALPHA,   BS_BLEND_ONE,              BS_BLEND_INV_SRC_ALPHA);
		break;

		default:
		fail2(std::to_string(_index).c_str());
	}
}
