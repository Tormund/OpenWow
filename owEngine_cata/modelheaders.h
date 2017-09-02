#ifndef MODELHEADERS_H
#define MODELHEADERS_H

#pragma pack(push,1)

//float floats[14];
struct PhysicsSettings
{
	vec3 VertexBox[2];
	float VertexRadius;
	vec3 BoundingBox[2];
	float BoundingRadius;
};

struct ModelHeader
{
	char id[4];
	uint8_t version[4];
	uint32_t nameLength;
	uint32_t nameOfs;
	uint32_t GlobalModelFlags; // 1: tilt x, 2: tilt y, 4:, 8: add another field in header, 16: ; (no other flags as of 3.1.1);

	uint32_t nGlobalSequences; // AnimationRelated
	uint32_t ofsGlobalSequences; // A list of timestamps.
	uint32_t nAnimations; // AnimationRelated
	uint32_t ofsAnimations; // Information about the animations in the model.
	uint32_t nAnimationLookup; // AnimationRelated
	uint32_t ofsAnimationLookup; // Mapping of global IDs to the entries in the Animation sequences block.
	//uint32_t nD;
	//uint32_t ofsD;
	uint32_t nBones; // BonesAndLookups
	uint32_t ofsBones; // Information about the bones in this model.
	uint32_t nKeyBoneLookup; // BonesAndLookups
	uint32_t ofsKeyBoneLookup; // Lookup table for key skeletal bones.

	uint32_t nVertices; // GeometryAndRendering
	uint32_t ofsVertices; // Vertices of the model.
	uint32_t nViews; // GeometryAndRendering
	//uint32_t ofsViews; // Views (LOD) are now in .skins.

	uint32_t nColors; // ColorsAndTransparency
	uint32_t ofsColors; // Color definitions.

	uint32_t nTextures; // TextureAndTheifAnimation
	uint32_t ofsTextures; // Textures of this model.

	uint32_t nTransparency; // H,  ColorsAndTransparency
	uint32_t ofsTransparency; // Transparency of textures.
	//uint32_t nI;   // always unused ?
	//uint32_t ofsI;
	uint32_t nTexAnims;	// J, TextureAndTheifAnimation
	uint32_t ofsTexAnims;
	uint32_t nTexReplace; // TextureAndTheifAnimation
	uint32_t ofsTexReplace; // Replaceable Textures.

	uint32_t nTexFlags; // Render Flags
	uint32_t ofsTexFlags; // Blending modes / render flags.
	uint32_t nBoneLookup; // BonesAndLookups
	uint32_t ofsBoneLookup; // A bone lookup table.

	uint32_t nTexLookup; // TextureAndTheifAnimation
	uint32_t ofsTexLookup; // The same for textures.

	uint32_t nTexUnitLookup;		// L, TextureAndTheifAnimation
	uint32_t ofsTexUnitLookup; // And texture units. Somewhere they have to be too.
	uint32_t nTransparencyLookup; // M, ColorsAndTransparency
	uint32_t ofsTransparencyLookup; // Everything needs its lookup. Here are the transparencies.
	uint32_t nTexAnimLookup; // TextureAndTheifAnimation
	uint32_t ofsTexAnimLookup; // Wait. Do we have animated Textures? Wasn't ofsTexAnims deleted? oO

	struct PhysicsSettings ps;

	uint32_t nBoundingTriangles; // Miscellaneous
	uint32_t ofsBoundingTriangles;
	uint32_t nBoundingVertices; // Miscellaneous
	uint32_t ofsBoundingVertices;
	uint32_t nBoundingNormals; // Miscellaneous
	uint32_t ofsBoundingNormals;

	uint32_t nAttachments; // O, Miscellaneous
	uint32_t ofsAttachments; // Attachments are for weapons etc.
	uint32_t nAttachLookup; // P, Miscellaneous
	uint32_t ofsAttachLookup; // Of course with a lookup.
	uint32_t nEvents; // 
	uint32_t ofsEvents; // Used for playing sounds when dying and a lot else.
	uint32_t nLights; // R
	uint32_t ofsLights; // Lights are mainly used in loginscreens but in wands and some doodads too.
	uint32_t nCameras; // S, Miscellaneous
	uint32_t ofsCameras; // The cameras are present in most models for having a model in the Character-Tab.
	uint32_t nCameraLookup; // Miscellaneous
	uint32_t ofsCameraLookup; // And lookup-time again.
	uint32_t nRibbonEmitters; // U, Effects
	uint32_t ofsRibbonEmitters; // Things swirling around. See the CoT-entrance for light-trails.
	uint32_t nParticleEmitters; // V, Effects
	uint32_t ofsParticleEmitters; // Spells and weapons, doodads and loginscreens use them. Blood dripping of a blade? Particles.

};

// block B - animations
struct ModelAnimation
{
	uint16_t animID;
	uint16_t subAnimID;
	uint32_t length;

	float moveSpeed;

	uint32_t loopType;
	uint32_t flags;
	uint32_t d1;
	uint32_t d2;
	uint32_t playSpeed;  // note: this can't be play speed because it's 0 for some models

	vec3 boxA, boxB;
	float rad;

	int16_t NextAnimation;
	int16_t Index;
};

struct AnimationBlockHeader
{
	uint32_t nEntrys;
	uint32_t ofsEntrys;
};

// sub-block in block E - animation data
struct AnimationBlock
{
	int16_t type;		// interpolation type (0=none, 1=linear, 2=hermite)
	int16_t seq;		// global sequence id or -1
	uint32_t nTimes;
	uint32_t ofsTimes;
	uint32_t nKeys;
	uint32_t ofsKeys;
};

#define	MODELBONE_BILLBOARD	8
#define	MODELBONE_TRANSFORM	512
// block E - bones
struct ModelBoneDef
{
	int32_t keyBoneId;
	int32_t flags;
	int16_t parent; // parent bone index
	int16_t geoid;
	int32_t unknown; // new int added to the bone definitions.  Added in WoW 2.0
	AnimationBlock translation;
	AnimationBlock rotation;
	AnimationBlock scaling;
	vec3 pivot;
};

struct ModelTexAnimDef
{
	AnimationBlock trans, rot, scale;
};

struct ModelVertex
{
	vec3 pos;
	uint8_t weights[4];
	uint8_t bones[4];
	vec3 normal;
	vec2 texcoords;
	int unk1, unk2; // always 0,0 so this is probably unused
};

struct ModelView
{
	char id[4];				 // Signature
	uint32_t nIndex, ofsIndex; // Vertices in this model (index into vertices[])
	uint32_t nTris, ofsTris;	 // indices
	uint32_t nProps, ofsProps; // additional vtx properties
	uint32_t nSub, ofsSub;	 // materials/renderops/submeshes
	uint32_t nTex, ofsTex;	 // material properties/textures
	int32_t lod;				 // LOD bias?
};


/// One material + render operation
struct ModelGeoset
{
	uint32_t id;		// mesh part id?
	uint16_t vstart;	// first vertex
	uint16_t vcount;	// num vertices
	uint16_t istart;	// first index
	uint16_t icount;	// num indices
	uint16_t nBones;		// number of bone indices, Number of elements in the bone lookup table.
	uint16_t StartBones;		// ? always 1 to 4, Starting index in the bone lookup table.
	uint16_t d5;		// ?
	uint16_t rootBone;		// root bone?
	vec3 BoundingBox[2];
	float radius;
};

#define	TEXTUREUNIT_STATIC	16
/// A texture unit (sub of material)
struct ModelTexUnit
{
	// probably the texture units
	// size always >=number of materials it seems
	uint16_t flags;		// Flags
	uint16_t shading;		// If set to 0x8000: shaders. Used in skyboxes to ditch the need for depth buffering. See below.
	uint16_t op;			// Material this texture is part of (index into mat)
	uint16_t op2;			// Always same as above?
	int16_t colorIndex;	// color or -1
	uint16_t flagsIndex;	// more flags...
	uint16_t texunit;		// Texture unit (0 or 1)
	uint16_t mode;			// ? (seems to be always 1)
	uint16_t textureid;	// Texture id (index into global texture list)
	uint16_t texunit2;	// copy of texture unit value?
	uint16_t transid;		// transparency id (index into transparency list)
	uint16_t texanimid;	// texture animation id
};

enum TextureFlags
{
	TEXTURE_WRAPX = 1,
	TEXTURE_WRAPY
};
#define	RENDERFLAGS_UNLIT	1
#define	RENDERFLAGS_UNFOGGED	2
#define	RENDERFLAGS_TWOSIDED	4
#define	RENDERFLAGS_BILLBOARD	8
#define	RENDERFLAGS_ZBUFFERED	16
// block X - render flags
struct ModelRenderFlags
{
	uint16_t flags;
	uint16_t blend;
};

// block G - color defs
struct ModelColorDef
{
	AnimationBlock color;
	AnimationBlock opacity;
};

// block H - transp defs
struct ModelTransDef
{
	AnimationBlock trans;
};

#define	TEXTURE_MAX	32
struct ModelTextureDef
{
	uint32_t type;
	uint32_t flags;
	uint32_t nameLen;
	uint32_t nameOfs;
};

enum ModelLightTypes
{
	MODELLIGHT_DIRECTIONAL = 0,
	MODELLIGHT_POINT
};
struct ModelLightDef
{
	int16_t type;
	int16_t bone;
	vec3 pos;
	AnimationBlock ambColor;
	AnimationBlock ambIntensity;
	AnimationBlock color;
	AnimationBlock intensity;
	AnimationBlock attStart;
	AnimationBlock attEnd;
	AnimationBlock unk1;
};

struct ModelCameraDef
{
	int32_t id; // 0 is potrait camera, 1 characterinfo camera; -1 if none; referenced in CamLookup_Table
	float farclip; // Where it stops to be drawn.
	float nearclip; // Far and near. Both of them.
	AnimationBlock transPos; // (vec3) How the cameras position moves. Should be 3*3 floats. (? WoW parses 36 bytes = 3*3*sizeof(float))
	vec3 pos; // float, Where the camera is located.
	AnimationBlock transTarget; // (vec3) How the target moves. Should be 3*3 floats. (?)
	vec3 target; // float, Where the camera points to.
	AnimationBlock rot; // (Quat) The camera can have some roll-effect. Its 0 to 2*Pi. 3 Floats!
	AnimationBlock AnimBlock4; // (Float) One Float. cataclysm
};

struct FakeAnimationBlock
{
	uint32_t nTimes;
	uint32_t ofsTimes;
	uint32_t nKeys;
	uint32_t ofsKeys;
};

struct ModelParticleParams
{
	FakeAnimationBlock colors; 	// (short, vec3)	This one points to 3 floats defining red, green and blue.
	FakeAnimationBlock opacity;      // (short, short)		Looks like opacity (short), Most likely they all have 3 timestamps for {start, middle, end}.
	FakeAnimationBlock sizes; 		// (short, vec2)	It carries two floats per key. (x and y scale)
	int32_t d[2];
	FakeAnimationBlock Intensity; 	// Some kind of intensity values seen: 0,16,17,32(if set to different it will have high intensity) (short, short)
	FakeAnimationBlock unk2; 		// (short, short)
	float unk[3];
	float scales[3];
	float slowdown;
	float unknown1[2];
	float rotation;				//Sprite Rotation
	float unknown2[2];
	float Rot1[3];					//Model Rotation 1
	float Rot2[3];					//Model Rotation 2
	float Trans[3];				//Model Translation
	float f2[4];
	int32_t nUnknownReference;
	int32_t ofsUnknownReferenc;
};

#define	MODELPARTICLE_DONOTTRAIL			0x10
#define	MODELPARTICLE_DONOTBILLBOARD	0x1000
struct ModelParticleEmitterDef
{
	int32_t id;
	int32_t flags;
	vec3 pos; // The position. Relative to the following bone.
	int16_t bone; // The bone its attached to.
	int16_t texture; // And the texture that is used.
	int32_t nModelFileName;
	int32_t ofsModelFileName;
	int32_t nParticleFileName;
	int32_t ofsParticleFileName; // TODO
	int8_t blend;
	int8_t EmitterType; // EmitterType	 1 - Plane (rectangle), 2 - Sphere, 3 - Spline? (can't be bothered to find one)
	int16_t ParticleColor; // This one is used so you can assign a color to specific particles. They loop over all 
						 // particles and compare +0x2A to 11, 12 and 13. If that matches, the colors from the dbc get applied.
	int8_t ParticleType; // 0 "normal" particle, 
					   // 1 large quad from the particle's origin to its position (used in Moonwell water effects)
					   // 2 seems to be the same as 0 (found some in the Deeprun Tram blinky-lights-sign thing)
	int8_t HeaderTail; // 0 - Head, 1 - Tail, 2 - Both
	int16_t TextureTileRotation; // TODO, Rotation for the texture m_TileExists. (Values: -1,0,1)
	int16_t cols; // How many different frames are on that texture? People should learn what rows and cols are.
	int16_t rows; // (2, 2) means slice texture to 2*2 pieces
	AnimationBlock EmissionSpeed; // (Float) All of the following blocks should be floats.
	AnimationBlock SpeedVariation; // (Float) Variation in the flying-speed. (range: 0 to 1)
	AnimationBlock VerticalRange; // (Float) Drifting away vertically. (range: 0 to pi)
	AnimationBlock HorizontalRange; // (Float) They can do it horizontally too! (range: 0 to 2*pi)
	AnimationBlock Gravity; // (Float) Fall down, apple!
	AnimationBlock Lifespan; // (Float) Everyone has to die.
	int32_t unknown;
	AnimationBlock EmissionRate; // (Float) Stread your particles, emitter.
	int32_t unknown2;
	AnimationBlock EmissionAreaLength; // (Float) Well, you can do that in this area.
	AnimationBlock EmissionAreaWidth; // (Float) 
	AnimationBlock Gravity2; // (Float) A second gravity? Its strong.
	ModelParticleParams p;
	AnimationBlock en; // (UInt16), seems unused in cataclysm
	int32_t unknown3; // 12319, cataclysm
	int32_t unknown4; // 12319, cataclysm
	int32_t unknown5; // 12319, cataclysm
	int32_t unknown6; // 12319, cataclysm
};


struct ModelRibbonEmitterDef
{
	int32_t id;
	int32_t bone;
	vec3 pos;
	int32_t nTextures;
	int32_t ofsTextures;
	int32_t nUnknown;
	int32_t ofsUnknown;
	AnimationBlock color;
	AnimationBlock opacity;
	AnimationBlock above;
	AnimationBlock below;
	float res, length, Emissionangle;
	int16_t s1, s2;
	AnimationBlock unk1;
	AnimationBlock unk2;
	int32_t unknown;
};


#pragma pack(pop)


#endif
