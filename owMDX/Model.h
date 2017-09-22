#pragma once

#include "modelheaders.h"

#include "Particle.h"
#include "ParticleSystem.h"
#include "RibbonEmitter.h"

#include "Model_Bone.h"
#include "Model_Camera.h"
#include "Model_Color.h"
#include "Model_Light.h"
#include "Model_RenderPass.h"
#include "Model_TextureAnim.h"
#include "Model_Transparency.h"

class Model;

class ModelRenderPass
{
public:
	bool init(Model * m);
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
};

// copied from the .mdl docs? this might be completely wrong
enum BlendModes
{
	BM_OPAQUE,
	BM_TRANSPARENT,
	BM_ALPHA_BLEND,
	BM_ADDITIVE,
	BM_ADDITIVE_ALPHA,
	BM_MODULATE,
	BM_MODULATE2
};

class Model : public RefItemNamed
{
public:
	OW_MDX_DLL_API Model(cstring name);
	~Model();

	OW_MDX_DLL_API void Init(bool forceAnim = false);

	OW_MDX_DLL_API void draw();
	OW_MDX_DLL_API void updateEmitters(float dt);


private:
	string m_ModelName;
	string m_ModelFileName;
	string m_ModelInternalName;

private:
	GLuint dlist;
	GLuint vbuf, nbuf, tbuf;
	size_t vbufsize;
	bool animated;
	bool animGeometry, animTextures, animBones;

	bool forceAnim;
	File *animfiles;

	ModelHeader header;

	ModelTextureAnim* texAnims;
	M2Sequence* anims;
	uint32_t* globalSequences;
	ModelColor* colors;
	ModelTransparency* transparency;
	ModelLight* lights;
	ParticleSystem* particleSystems;
	RibbonEmitter* ribbons;

	OW_MDX_DLL_API void drawModel();
	OW_MDX_DLL_API void initCommon(File& f);
	OW_MDX_DLL_API bool isAnimated(File& f);
	OW_MDX_DLL_API void initAnimated(File& f);
	OW_MDX_DLL_API void initStatic(File& f);

	M2Vertex * origVertices;
	vec3* vertices;
	vec3* normals;

	uint16_t* indices;
	size_t nIndices;

	vector<ModelRenderPass> passes;




	void animate(int anim);
	void calcBones(int anim, int time);

	void lightsOn(GLuint lbase);
	void lightsOff(GLuint lbase);

public:
	ModelCamera* m_ModelCamera;
	Bone* bones;

	// ===============================
	// Toggles
	bool* showGeosets;

	// ===============================
	// Texture data
	// ===============================
	Texture** textures;
	int specialTextures[TEXTURE_MAX];
	Texture* replaceTextures[TEXTURE_MAX];
	bool useReplaceTextures[TEXTURE_MAX];


	bool ok;
	bool ind;

	float rad;
	float trans;
	bool animcalc;
	int anim, animtime;




	friend class ModelRenderPass;
};
