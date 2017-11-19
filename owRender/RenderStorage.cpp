#include "stdafx.h"

// General
#include "RenderStorage.h"

// Additional
#include "Render.h"
#include "RenderDevice.h"

bool RenderStorage::Init()
{
    CreateWoWLayouts();


    CreateLayouts();
    CreateGeometry();

    

    return true;
}

void RenderStorage::Destroy()
{

}

void RenderStorage::CreateLayouts()
{
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

    vector<vec3> verticesQuad;
    verticesQuad.push_back(vec3(-1.0f, -1.0f, 1.0f));
    verticesQuad.push_back(vec3(1.0f, -1.0f, 1.0f));
    verticesQuad.push_back(vec3(-1.0f, 1.0f, 1.0f));
    verticesQuad.push_back(vec3(1.0f, 1.0f, 1.0f));

    vector<vec2> texCoordsQuad;
    texCoordsQuad.push_back(vec2(0.0f, 0.0f));
    texCoordsQuad.push_back(vec2(1.0f, 0.0f));
    texCoordsQuad.push_back(vec2(0.0f, 1.0f));
    texCoordsQuad.push_back(vec2(1.0f, 1.0f));

    //-----------------------------------------
    // Unit quad
    //-----------------------------------------

    uint32 __vbQuad = _Render->r->createVertexBuffer(4 * sizeof(vec3), verticesQuad.data());

    __Quad = _Render->r->beginCreatingGeometry(__layout_GxVBF_P);
    _Render->r->setGeomVertexParams(__Quad, __vbQuad, R_DataType::T_FLOAT, 0, sizeof(vec3));
    _Render->r->setGeomIndexParams(__Quad, __ibQuadDefault, R_IndexFormat::IDXFMT_16);
    _Render->r->finishCreatingGeometry(__Quad);

    //-----------------------------------------
    // Unit quad with texture coords
    //-----------------------------------------

    uint32 __vbQuadVT = _Render->r->createVertexBuffer(4 * sizeof(Texture_Vertex), nullptr);
    _Render->r->updateBufferData(__vbQuadVT, 0,                4 * sizeof(vec3), verticesQuad.data());
    _Render->r->updateBufferData(__vbQuadVT, 4 * sizeof(vec3), 4 * sizeof(vec2), texCoordsQuad.data());

    __QuadVT = _Render->r->beginCreatingGeometry(__layout_GxVBF_PT);
    _Render->r->setGeomVertexParams(__QuadVT, __vbQuadVT, R_DataType::T_FLOAT, 0,            0);
    _Render->r->setGeomVertexParams(__QuadVT, __vbQuadVT, R_DataType::T_FLOAT, 4 * sizeof(vec3), 0);
    _Render->r->setGeomIndexParams(__QuadVT, __ibQuadDefault, R_IndexFormat::IDXFMT_16);
    _Render->r->finishCreatingGeometry(__QuadVT);

    //-----------------------------------------
    // Unit quad with updatable texture coords
    //-----------------------------------------

    __vbQuadVTDynamic = _Render->r->createVertexBuffer(4 * sizeof(Texture_Vertex), nullptr);
    _Render->r->updateBufferData(__vbQuadVTDynamic, 0,                4 * sizeof(vec3), verticesQuad.data());
    _Render->r->updateBufferData(__vbQuadVTDynamic, 4 * sizeof(vec3), 4 * sizeof(vec2), texCoordsQuad.data());

    __QuadVTDynamic = _Render->r->beginCreatingGeometry(__layout_GxVBF_PT);
    _Render->r->setGeomVertexParams(__QuadVTDynamic, __vbQuadVTDynamic, R_DataType::T_FLOAT, 0, 0);
    _Render->r->setGeomVertexParams(__QuadVTDynamic, __vbQuadVTDynamic, R_DataType::T_FLOAT, 4 * sizeof(vec3), 0);
    _Render->r->setGeomIndexParams(__QuadVTDynamic, __ibQuadDefault, R_IndexFormat::IDXFMT_16);
    _Render->r->finishCreatingGeometry(__QuadVTDynamic);

    //-----------------------------------------
    // Unit cube
    //-----------------------------------------

    float cubeVerts[8 * 3] = {  // x, y, z
        0.f, 0.f, 1.f,   1.f, 0.f, 1.f,   1.f, 1.f, 1.f,   0.f, 1.f, 1.f,
        0.f, 0.f, 0.f,   1.f, 0.f, 0.f,   1.f, 1.f, 0.f,   0.f, 1.f, 0.f
    };
    uint16 cubeInds[36] = {
        0, 1, 2, 2, 3, 0,   1, 5, 6, 6, 2, 1,   5, 4, 7, 7, 6, 5,
        4, 0, 3, 3, 7, 4,   3, 2, 6, 6, 7, 3,   4, 5, 1, 1, 0, 4
    };

    uint32 _vbCube = _Render->r->createVertexBuffer(8 * sizeof(vec3), cubeVerts);
    uint32 _ibCube = _Render->r->createIndexBuffer(36 * sizeof(uint16), cubeInds);

    _cubeGeo = _Render->r->beginCreatingGeometry(__layout_GxVBF_P);
    _Render->r->setGeomVertexParams(_cubeGeo, _vbCube, R_DataType::T_FLOAT, 0, sizeof(vec3));
    _Render->r->setGeomIndexParams(_cubeGeo, _ibCube, IDXFMT_16);
    _Render->r->finishCreatingGeometry(_cubeGeo);

    //-----------------------------------------
    // Unit (geodesic) sphere (created by recursively subdividing a base octahedron)
    //-----------------------------------------

    vec3 spVerts[126] =
    {  // x, y, z
        vec3(0.0f, 1.0f, 0.0f),       vec3(0.0f, -1.0f, 0.0f),
        vec3(-0.707f, 0.0f, 0.707f),   vec3(0.707f, 0.0f, 0.707f),
        vec3(0.707f, 0.0f, -0.707f),   vec3(-0.707f, 0.0f, -0.707f)
    };
    uint16 spInds[128 * 3] = {  // Number of faces: (4 ^ iterations) * 8
        2, 3, 0,   3, 4, 0,   4, 5, 0,   5, 2, 0,   2, 1, 3,   3, 1, 4,   4, 1, 5,   5, 1, 2
    };
    for (uint32 i = 0, nv = 6, ni = 24; i < 2; ++i)  // Two iterations
    {
        // Subdivide each face into 4 tris by bisecting each edge and push vertices onto unit sphere
        for (uint32 j = 0, prevNumInds = ni; j < prevNumInds; j += 3)
        {
            spVerts[nv++] = ((spVerts[spInds[j + 0]] + spVerts[spInds[j + 1]]) * 0.5f).normalized();
            spVerts[nv++] = ((spVerts[spInds[j + 1]] + spVerts[spInds[j + 2]]) * 0.5f).normalized();
            spVerts[nv++] = ((spVerts[spInds[j + 2]] + spVerts[spInds[j + 0]]) * 0.5f).normalized();

            spInds[ni++] = spInds[j + 0]; 
            spInds[ni++] = nv - 3; 
            spInds[ni++] = nv - 1;

            spInds[ni++] = nv - 3; 
            spInds[ni++] = spInds[j + 1]; 
            spInds[ni++] = nv - 2;

            spInds[ni++] = nv - 2; 
            spInds[ni++] = spInds[j + 2]; 
            spInds[ni++] = nv - 1;

            spInds[j + 0] = nv - 3; 
            spInds[j + 1] = nv - 2; 
            spInds[j + 2] = nv - 1;
        }
    }

    uint32 _vbSphere = _Render->r->createVertexBuffer(126 * sizeof(vec3), spVerts);
    uint32 _ibSphere = _Render->r->createIndexBuffer(128 * 3 * sizeof(uint16), spInds);

    _sphereGeo = _Render->r->beginCreatingGeometry(__layout_GxVBF_P);
    _Render->r->setGeomVertexParams(_sphereGeo, _vbSphere, R_DataType::T_FLOAT, 0, sizeof(vec3));
    _Render->r->setGeomIndexParams(_sphereGeo, _ibSphere, IDXFMT_16);
    _Render->r->finishCreatingGeometry(_sphereGeo);

    //-----------------------------------------
    // Unit cone
    //-----------------------------------------

    float coneVerts[13 * 3] = {  // x, y, z
        0.f, 0.f, 0.f,
        0.f, 1.f, -1.f,   -0.5f, 0.866f, -1.f,   -0.866f, 0.5f, -1.f,
        -1.f, 0.f, -1.f,   -0.866f, -0.5f, -1.f,   -0.5f, -0.866f, -1.f,
        0.f, -1.f, -1.f,   0.5f, -0.866f, -1.f,   0.866f, -0.5f, -1.f,
        1.f, 0.f, -1.f,   0.866f, 0.5f, -1.f,   0.5f, 0.866f, -1.f,
    };
    uint16 coneInds[22 * 3] = {
        0, 1, 2,   0, 2, 3,   0, 3, 4,   0, 4, 5,   0, 5, 6,   0, 6, 7,
        0, 7, 8,   0, 8, 9,   0, 9, 10,   0, 10, 11,   0, 11, 12,   0, 12, 1,
        10, 6, 2,   10, 8, 6,   10, 9, 8,   8, 7, 6,   6, 4, 2,   6, 5, 4,   4, 3, 2,
        2, 12, 10,   2, 1, 12,   12, 11, 10
    };

    uint32 _vbCone = _Render->r->createVertexBuffer(13 * sizeof(vec3), coneVerts);
    uint32 _ibCone = _Render->r->createIndexBuffer(22 * 3 * sizeof(uint16), coneInds);

    _coneGeo = _Render->r->beginCreatingGeometry(__layout_GxVBF_P);
    _Render->r->setGeomVertexParams(_coneGeo, _vbCone, R_DataType::T_FLOAT, 0, sizeof(vec3));
    _Render->r->setGeomIndexParams(_coneGeo, _ibCone, IDXFMT_16);
    _Render->r->finishCreatingGeometry(_coneGeo);
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
    __layout_GxVBF_PN = _Render->r->registerVertexLayout(2, attribs_GxVBF_PN); // USED IN LOW-RESOLUTION TILES

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
    __layout_GxVBF_PÑ = _Render->r->registerVertexLayout(2, attribs_GxVBF_PC); // USED IN M2


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
    __layout_GxVBF_PT = _Render->r->registerVertexLayout(2, attribs_GxVBF_PT); // USED IN M2

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

    R_VertexLayoutAttrib attribs_GxVBF_PNC2T2[6] = { // Original 48
        {"position",    0, 3, 0},
        {"normal",      1, 3, 0},
        {"color0",      2, 4, 0}, // Original 1 but type is uint8
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
        _Render->r->setBlendMode(false, BS_BLEND_ONE, BS_BLEND_ZERO);
        break;

        case 1: // AlphaKey
        _Render->r->setBlendMode(false, BS_BLEND_ONE, BS_BLEND_ZERO);
        break;

        case 2: // Alpha
        _Render->r->setBlendModeEx(false, BS_BLEND_SRC_ALPHA, BS_BLEND_INV_SRC_ALPHA, BS_BLEND_ONE, BS_BLEND_INV_SRC_ALPHA);
        break;

        case 3: // Add
        _Render->r->setBlendModeEx(true, BS_BLEND_SRC_ALPHA, BS_BLEND_ONE, BS_BLEND_ZERO, BS_BLEND_ONE);
        break;

        case 4: // Mod
        _Render->r->setBlendModeEx(true, BS_BLEND_DEST_COLOR, BS_BLEND_ZERO, BS_BLEND_DEST_ALPHA, BS_BLEND_ZERO);
        break;

        case 5: // Mod2x
        _Render->r->setBlendModeEx(true, BS_BLEND_DEST_COLOR, BS_BLEND_SRC_COLOR, BS_BLEND_DEST_ALPHA, BS_BLEND_SRC_ALPHA);
        break;

        case 6: // ModAdd
        _Render->r->setBlendModeEx(true, BS_BLEND_DEST_COLOR, BS_BLEND_ONE, BS_BLEND_DEST_ALPHA, BS_BLEND_ONE);
        break;

        case 7: // InvSrcAlphaAdd
        _Render->r->setBlendModeEx(true, BS_BLEND_INV_SRC_ALPHA, BS_BLEND_ONE, BS_BLEND_INV_SRC_ALPHA, BS_BLEND_ONE);
        break;

        case 8: // InvSrcAlphaOpaque
        _Render->r->setBlendModeEx(true, BS_BLEND_INV_SRC_ALPHA, BS_BLEND_ZERO, BS_BLEND_INV_SRC_ALPHA, BS_BLEND_ZERO);
        break;

        case 9: // SrcAlphaOpaque
        _Render->r->setBlendModeEx(true, BS_BLEND_SRC_ALPHA, BS_BLEND_ZERO, BS_BLEND_SRC_ALPHA, BS_BLEND_ZERO);
        break;

        case 10: // NoAlphaAdd
        _Render->r->setBlendModeEx(true, BS_BLEND_ONE, BS_BLEND_ONE, BS_BLEND_ZERO, BS_BLEND_ONE);
        break;

        case 11: // ConstantAlpha
        //GLSetBlend(true, GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA, GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);
        fail1("Not implemented!!!");
        break;

        case 12: // Screen
        _Render->r->setBlendModeEx(true, BS_BLEND_INV_DEST_COLOR, BS_BLEND_ONE, BS_BLEND_ONE, BS_BLEND_ZERO);
        break;

        case 13: // BlendAdd
        _Render->r->setBlendModeEx(true, BS_BLEND_ONE, BS_BLEND_INV_SRC_ALPHA, BS_BLEND_ONE, BS_BLEND_INV_SRC_ALPHA);
        break;

        default:
        fail2(std::to_string(_index).c_str());
    }
}
