#pragma once

#include "Particle.h"
#include "ParticleSystem.h"
#include "RibbonEmitter.h"

#include "MDX_Part_Bone.h"
#include "MDX_Part_Camera.h"
#include "MDX_Part_Color.h"
#include "MDX_Part_Light.h"
#include "MDX_Part_TextureAnim.h"
#include "MDX_Part_Transparency.h"

#include "Model_Skin.h"

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

class MDX : public RefItemNamed
{
public:
	MDX(cstring name);
	~MDX();

	void Init(bool forceAnim = false);
	inline bool IsLoaded() { return m_Loaded; }

	void draw();
	void updateEmitters(float dt);

	void drawShaderBegin();
	void drawShaderEnd();

	ModelHeader header;


private:
	bool m_Loaded;
	string m_ModelName;
	string m_ModelFileName;
	string m_ModelInternalName;

private:
	GLuint globalBuffer;

	M2Vertex* m_OriginalVertexes;

	vec3* m_Vertices;
	vec2* m_Texcoords;
	vec3* m_Normals;

	vector<Model_Skin*> m_Skins;


public:
	bool animated;
	bool animGeometry, animTextures, animBones;

	bool forceAnim;
	File* animfiles;

	MDX_Part_Bone* m_Part_Bones;

	M2Texture* texdef;

	MDX_Part_TextureAnim* m_TexturesAnims;
	M2Sequence* m_Sequences;
	uint32* m_GlobalLoops;
	MDX_Part_Color* colors;

	uint16* transLookup;
	MDX_Part_Transparency* transparency;


	MDX_Part_Light* m_Lights;
	MDX_Part_Camera* m_Cameras;

#ifdef MDX_PARTICLES_ENABLE
	ParticleSystem* particleSystems;
	RibbonEmitter* ribbons;
#endif

	void drawModel();
	void initCommon(File& f);
	bool isAnimated(File& f);
	void initAnimated(File& f);
	void initStatic(File& f);

	void animate(uint32 _animationIndex);
	void calcBones(uint32 _animationIndex, int time);

	void lightsOn(GLuint lbase);
	void lightsOff(GLuint lbase);

public:
	

	// ===============================
	// Texture data
	// ===============================
	Texture** textures;
	int specialTextures[TEXTURE_MAX];
	Texture* replaceTextures[TEXTURE_MAX];
	bool useReplaceTextures[TEXTURE_MAX];

	bool m_IsBillboard;

	float m_Radius;
	float trans;
	bool animcalc;
	int m_CurrentAnimationIndex, animtime;

	friend class ModelRenderPass;
};
