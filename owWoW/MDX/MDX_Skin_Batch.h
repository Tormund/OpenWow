#pragma once

class MDX;

class MDX_Skin_Batch
{
public:
    uint16 m2SkinIndex;

    uint32 indexStart, indexCount;
    uint32 vertexStart, vertexCount;
    uint16 bonesStartIndex, bonesCount, boneInfluences;

	uint16    __colorIndex;
	uint16    __textureWeight;
	int16     __textureAnims;
	int       __blendMode;
	Material  __material;
};
