#pragma once

#pragma pack(push, 1)

struct M2Bounds
{
	CAaBox extent;
	float radius;
};

template<typename T>
struct M2SplineKey
{
	T value;
	T inTan;
	T outTan;
};

struct M2Box
{
	C3Vector ModelRotationSpeedMin;
	C3Vector ModelRotationSpeedMax;
};

struct M2Range
{
	uint32_t minimum;
	uint32_t maximum;
};

template<typename T>
struct FBlock
{
	uint32_t nTimes;
	uint32_t ofsTimes;
	uint32_t nKeys;
	uint32_t ofsKeys;
};


////////

struct M2Loop
{
	uint32_t timestamp;
};

//

struct M2CompBone                 // probably M2Bone ≤ Vanilla
{
	int32_t key_bone_id;            // Back-reference to the key bone lookup table. -1 if this is no key bone.
	enum
	{
		spherical_billboard = 0x8,
		cylindrical_billboard_lock_x = 0x10,
		cylindrical_billboard_lock_y = 0x20,
		cylindrical_billboard_lock_z = 0x40,
		transformed = 0x200,
		kinematic_bone = 0x400,       // MoP+: allow physics to influence this bone
		helmet_anim_scaled = 0x1000,  // set blend_modificator to helmetAnimScalingRec.m_amount for this bone
	};
	uint32_t flags;
	int16_t parent_bone;            // Parent bone ID or -1 if there is none.
	uint16_t submesh_id;            // Mesh part ID OR uDistToParent?

	union
	{                         // only ≥ BC?
		struct
		{
			uint16_t uDistToFurthDesc;
			uint16_t uZRatioOfChain;
		} CompressData;
		uint32_t boneNameCRC;         // these are for debugging only. their bone names match those in key bone lookup.
	};

	M2Track<C3Vector> translation;
	M2Track<M2CompQuat> rotation;   // compressed values, default is (32767,32767,32767,65535) == (0,0,0,1) == identity
	M2Track<C3Vector> scale;

	C3Vector pivot;                 // The pivot point of that bone.
};

//

//

struct M2Color
{
	M2Track<C3Vector> color;                                          // vertex colors in rgb order
	M2Track<short> alpha;    // FIXED 16                              // 0 - transparent, 0x7FFF - opaque. Normaly NonInterp
};

//

struct M2Texture
{
	uint32_t type;          // see below
	uint32_t flags;         // see below
	M2Array<char> filename; // for non-hardcoded textures (type != 0), this still points to a zero-sized string
};

//

struct M2TextureWeight
{
	M2Track<short> weight; // FIXED 16
};

//

struct M2TextureTransform
{
	M2Track<C3Vector> translation;
	// FIXME M2Track<C4Quaternion> rotation;    // rotation center is texture center (0.5, 0.5)
	M2Track<C3Vector> rotation;    // rotation center is texture center (0.5, 0.5)
	M2Track<C3Vector> scaling;
};

//

struct M2Attachment
{
	uint32_t id;                        // Referenced in the lookup-block below.
	uint16_t bone;                      // attachment base
	uint16_t unknown;                   // see BogBeast.m2 in vanilla for a model having values here
	vec3 position;                  // relative to bone; Often this value is the same as bone's pivot point 
	M2Track<uint8_t> animate_attached;  // whether or not the attached model is animated when this model is. only a bool is used. default is true.
};

//

struct M2Event
{
	uint32_t identifier;  // mostly a 3 character name prefixed with '$'.
	uint32_t data;        // This data is passed when the event is fired. 
	uint32_t bone;        // Somewhere it has to be attached.
	vec3 position;    // Relative to that bone of course, animated. Pivot without animating.
	M2TrackBase enabled;  // This is a timestamp-only animation block. It is built up the same as a normal AnimationBlocks, but is missing values, as every timestamp is an implicit "fire now".
};

//

struct M2Light
{
	uint16_t type;                      // Types are listed below.
	int16_t bone;                       // -1 if not attached to a bone
	vec3 position;                 // relative to bone, if given

	M2Track<vec3> ambient_color;
	M2Track<float> ambient_intensity;   // defaults to 1.0

	M2Track<vec3> diffuse_color;
	M2Track<float> diffuse_intensity;   // defaults to 1.0

	M2Track<float> attenuation_start;
	M2Track<float> attenuation_end;

	M2Track<uint8_t> visibility;          // enabled?
};

//

struct M2Camera // TODO Spline keys
{
	uint32_t type; // 0: portrait, 1: characterinfo; -1: else (flyby etc.); referenced backwards in the lookup table.
	
	float far_clip;
	float near_clip;

	M2Track<vec3> positions; // How the camera's position moves. Should be 3*3 floats.
	vec3 position_base;

	M2Track<vec3> target_position; // How the target moves. Should be 3*3 floats.
	vec3 target_position_base;

	M2Track<float> roll; // The camera can have some roll-effect. Its 0 to 2*Pi. 
	M2Track<float> FoV;  // Units are not radians. Multiplying by 35 seems to be the correct # of degrees. This is incredibly important, as otherwise e.g. loading screen models will look totally wrong.
};

//

struct M2Ribbon
{
	uint32_t ribbonId;                  // Always (as I have seen): -1.
	uint32_t boneIndex;                 // A bone to attach to.
	C3Vector position;                 // And a position, relative to that bone.
	M2Array<uint16_t> textureIndices;   // into textures
	M2Array<uint16_t> materialIndices;  // into materials

	M2Track<C3Vector> colorTrack;
	M2Track<short> alphaTrack;          // FIXME FIXED 16               // And an alpha value in a short, where: 0 - transparent, 0x7FFF - opaque.
	M2Track<float> heightAboveTrack;
	M2Track<float> heightBelowTrack;        // do not set to same!

	float edgesPerSecond;                  // this defines how smooth the ribbon is. A low value may produce a lot of edges.
	float edgeLifetime;                    // the length aka Lifespan. in seconds
	float gravity;                      // use arcsin(val) to get the emission angle in degree

	uint16_t textureRows;                    // tiles in texture
	uint16_t textureCols;

	M2Track<uint16_t> texSlotTrack;
	M2Track<uint8_t> visibilityTrack;

	int16_t priorityPlane;
	uint16_t padding;

};

//

#define	MODELPARTICLE_DONOTTRAIL		0x10
#define	MODELPARTICLE_DONOTBILLBOARD	0x1000

struct M2ParticleOld
{
	uint32_t particleId;                      // Always (as I have seen): -1.
	uint32_t flags;                           // See Below
	C3Vector Position;                        // The position. Relative to the following bone.
	uint16_t bone;                            // The bone its attached to.


	//union
	//{
		uint16_t texture;                         // And the textures that are used. 

		/*struct                                    // For multi-textured particles actually three ids
		{
			uint16_t texture_0 : 5;
			uint16_t texture_1 : 5;
			uint16_t texture_2 : 5;
			uint16_t : 1;
		};*/
	//};


	M2Array<char> geometry_model_filename;    // if given, this emitter spawns models
	M2Array<char> recursion_model_filename;   // if given, this emitter is an alias for the (maximum 4) emitters of the given model


	uint8_t blendingType;                     // A blending type for the particle. See Below
	uint8_t emitterType;                      // 1 - Plane (rectangle), 2 - Sphere, 3 - Spline, 4 - Bone
	uint16_t particleColorIndex;              // This one is used for ParticleColor.dbc. See below.

	uint8_t particleType;                     // Found below.
	uint8_t headorTail;                       // 0 - Head, 1 - Tail, 2 - Both 

	uint16_t textureTileRotation;             // Rotation for the texture tile. (Values: -1,0,1) -- priorityPlane
	uint16_t textureDimensions_rows;          // for tiled textures
	uint16_t textureDimensions_columns;

	M2Track<float> emissionSpeed;             // Base velocity at which particles are emitted.
	M2Track<float> speedVariation;            // Random variation in particle emission speed. (range: 0 to 1)
	M2Track<float> verticalRange;             // Drifting away vertically. (range: 0 to pi) For plane generators, this is the maximum polar angle of the initial velocity; 
											  // 0 makes the velocity straight up (+z). For sphere generators, this is the maximum elevation of the initial position; 
											  // 0 makes the initial position entirely in the x-y plane (z=0).
	M2Track<float> horizontalRange;           // They can do it horizontally too! (range: 0 to 2*pi) For plane generators, this is the maximum azimuth angle of the initial velocity; 
											  // 0 makes the velocity have no sideways (y-axis) component. 
											  // For sphere generators, this is the maximum azimuth angle of the initial position.
	M2Track<float> gravity;                   // Not necessarily a float; see below.


	M2Track<float> lifespan;
	float lifespanVary;                       // An individual particle's lifespan is added to by lifespanVary * random(-1, 1)


	M2Track<float> emissionRate;
	float emissionRateVary;                   // This adds to the base emissionRate value the same way as lifespanVary. The random value is different every update.

	M2Track<float> emissionAreaLength;        // For plane generators, this is the width of the plane in the x-axis.
											  // For sphere generators, this is the minimum radius.

	M2Track<float> emissionAreaWidth;         // For plane generators, this is the width of the plane in the y-axis.
											  // For sphere generators, this is the maximum radius.

	M2Track<float> zSource;                   // When greater than 0, the initial velocity of the particle is (particle.position - C3Vector(0, 0, zSource)).Normalize()

	// Params BEGIN

	FBlock<C3Vector> colorTrack;              // Most likely they all have 3 timestamps for {start, middle, end}.
	FBlock<short> alphaTrack;                 // FIXME FIXED16
	FBlock<C2Vector> scaleTrack;
	C2Vector scaleVary;                       // A percentage amount to randomly vary the scale of each particle
	FBlock<uint16_t> headCellTrack;           // Some kind of intensity values seen: 0,16,17,32 (if set to different it will have high intensity)
	FBlock<uint16_t> tailCellTrack;

	float unk[3];
	float scales[3];
	float slowdown;
	float unknown1[2];
	float rotation;				//Sprite Rotation
	float unknown2[2];

	/*float tailLength;                         // TailCellTime?
	float TwinkleSpeed;                       // has something to do with the spread
	float TwinklePercent;                     // has something to do with the spread
	
	CRange twinkleScale;
	float BurstMultiplier;                    // ivelScale
	float drag;                               // For a non-zero values, instead of travelling linearly the particles seem to slow down sooner. Speed is multiplied by exp( -drag * t ).

	float baseSpin;                           // Initial rotation of the particle quad
	float baseSpinVary;
	
	float Spin;                               // Rotation of the particle quad per second
	float spinVary;*/


	M2Box tumble;
	C3Vector WindVector;
	//float WindTime;

	float followSpeed1;
	float followScale1;
	float followSpeed2;
	float followScale2;

	M2Array<C3Vector> splinePoints;            //Set only for spline praticle emitter. Contains array of points for spline
	
    // Params END
	
	M2Track<uint8_t> enabledIn;                // (boolean) Appears to be used sparely now, probably there's a flag that links particles to animation sets where they are enabled.
};

//---------------------------

struct ModelHeader
{
	char id[4];                   // MD20 Magic
	uint32_t version;
	M2Array<char> name;

	struct
	{
		uint32_t flag_tilt_x : 1;
		uint32_t flag_tilt_y : 1;
		uint32_t : 1;

		uint32_t flag_has_blend_maps : 1;                   // add BlendMaps fields in header
		uint32_t : 1;

		uint32_t : 27;
	} global_flags;

	M2Array<M2Loop> global_loops;

	uint32_t nAnimations; // AnimationRelated
	uint32_t ofsAnimations; // Information about the animations in the model.

	uint32_t nAnimationLookup; // AnimationRelated
	uint32_t ofsAnimationLookup; // Mapping of global IDs to the entries in the Animation sequences block.

	M2Array<M2CompBone> bones;

	uint32_t nKeyBoneLookup; // BonesAndLookups
	uint32_t ofsKeyBoneLookup; // Lookup table for key skeletal bones.

	uint32_t nVertices; // GeometryAndRendering
	uint32_t ofsVertices; // Vertices of the model.

	uint32_t num_skin_profiles; // GeometryAndRendering

	M2Array<M2Color> colors;

	M2Array<M2Texture> textures;
	M2Array<M2TextureWeight> texture_weights;
	M2Array<M2TextureTransform> texture_transforms;

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

	CAaBox bounding_box;                                 // min/max( [1].z, 2.0277779f ) - 0.16f seems to be the maximum camera height
	float bounding_sphere_radius;                        // detail doodad draw dist = clamp (bounding_sphere_radius * detailDoodadDensityFade * detailDoodadDist, …)
	
	CAaBox collision_box;
	float collision_sphere_radius;

	M2Array<uint16_t> m_CollisionTriangles;
	M2Array<vec3> m_CollisionVertices;
	M2Array<vec3> m_CollisionNormals;

	M2Array<M2Attachment> attachments;
	M2Array<uint16_t> attachment_lookup_table;
	
	M2Array<M2Event> events;
	M2Array<M2Light> lights;

	M2Array<M2Camera> cameras;
	M2Array<uint16_t> camera_lookup_table;

	M2Array<M2Ribbon> ribbon_emitters;
	M2Array<M2ParticleOld> particle_emitters;
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





#define	TEXTURE_MAX	32

struct FakeAnimationBlock
{
	uint32_t nTimes;
	uint32_t ofsTimes;
	uint32_t nKeys;
	uint32_t ofsKeys;
};



#pragma pack(pop)