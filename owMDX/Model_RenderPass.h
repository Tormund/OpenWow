#pragma once

class Model;

/*class ModelRenderPass
{
public:
	bool init(Model * m);
	void deinit();

	bool operator< (const ModelRenderPass& _other) const
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

private:
	uint32_t indexStart, indexCount, vertexStart, vertexEnd;
	//Texture* texture, texture2;
	int tex;
	bool usetex2, useEnvMap, cull, trans, unlit, noZWrite, billboard;
	float p;

	int16_t texanim, color, opacity, blendmode;
	uint16_t order;

	// Geoset ID
	int geoset;

	// texture wrapping
	bool swrap, twrap;

	// colours
	vec4 ocol, ecol;
};*/
