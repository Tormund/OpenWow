#pragma once

class MDX;

class ModelRenderPass
{
public:
	bool init(MDX * m);
	void deinit();

	inline bool operator< (const ModelRenderPass& _other) const
	{
		if (order < _other.order)
		{
			return true;
		}
		else if (order > _other.order)
		{
			return false;
		}
		else
		{
			return (blendmode == _other.blendmode) ? (p < _other.p) : blendmode < _other.blendmode;
		}
	}

public:
	
	//Texture* texture, texture2;
	int tex;
	bool usetex2, useEnvMap, cull, trans, unlit, noZWrite, billboard;
	float p;

	int16 texanim, color, opacity, blendmode;
	uint16 order;

	// Geoset ID
	int m2SkinIndex;

	// texture wrapping
	bool swrap, twrap;

	// colours
	vec4 ocol, ecol;

    uint32 indexStart, indexCount;
    uint32 vertexStart, vertexCount;
    uint16 bonesStartIndex, bonesCount, boneInfluences;

	uint16    __colorIndex;
	uint16    __textureWeight;
	int16     __textureAnims;
	int       __blendMode;
	Material  __material;
};
