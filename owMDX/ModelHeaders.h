#pragma once

#include "../shared/pack_begin.h"

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




////////

struct M2Loop
{
	uint32_t timestamp;
};

//

struct M2Sequence
{
	uint16_t id;                   // Animation id in AnimationData.dbc
	uint16_t variationIndex;       // Sub-animation id: Which number in a row of animations this one is.
	uint32_t duration;             // The length of this animation sequence in milliseconds.

	float movespeed;               // This is the speed the character moves with in this animation.
	uint32_t flags;                // See below.
	int16_t frequency;             // This is used to determine how often the animation is played. For all animations of the same type, this adds up to 0x7FFF (32767).
	uint16_t _padding;
	M2Range replay;                // May both be 0 to not repeat. Client will pick a random number of repetitions within bounds if given.
	uint32_t blendTime;

	M2Bounds bounds;
	int16_t variationNext;         // id of the following animation of this AnimationID, points to an Index or is -1 if none.
	uint16_t aliasNext;            // id in the list of animations. Used to find actual animation if this sequence is an alias (flags & 0x40)
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

struct M2Vertex
{
	C3Vector pos;
	uint8_t bone_weights[4];
	uint8_t bone_indices[4];
	C3Vector normal;
	C2Vector tex_coords[2];  // two textures, depending on shader used
};

//

struct M2Color
{
	M2Track<C3Vector> color;                                          // vertex colors in rgb order
	M2Track<short> alpha;    // FIXED 16                              // 0 - transparent, 0x7FFF - opaque. Normaly NonInterp
};

//

#define	M2TEXTURE_FLAGS_WRAPX 0x01
#define	M2TEXTURE_FLAGS_WRAPY 0x02
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

#define	M2MATERIAL_FLAGS_UNLIT	             0x01
#define	M2MATERIAL_FLAGS_UNFOGGED	         0x02
#define	M2MATERIAL_FLAGS_TWOSIDED	         0x04
#define	M2MATERIAL_FLAGS_DEPTHTESTBILLBOARD	 0x08
#define	M2MATERIAL_FLAGS_DEPTHWRITE	         0x10
struct M2Material
{
	uint16_t flags;
	uint16_t blending_mode; // apparently a bitfield
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

#define	M2PARTICLE_FLAGS_AFFECTEDBYLIGHTING		0x1
#define	M2PARTICLE_FLAGS_WORLDUP		        0x8
#define	M2PARTICLE_FLAGS_DONOTTRAIL		        0x10
#define	M2PARTICLE_FLAGS_INLIGHTING		        0x20
#define	M2PARTICLE_FLAGS_DONOTBILLBOARD	        0x1000
struct M2Particle
{
	uint32_t particleId;                      // Always (as I have seen): -1.
	uint32_t flags;                           // See Below
	C3Vector Position;                        // The position. Relative to the following bone.
	uint16_t bone;                            // The bone its attached to.


	union
	{
		uint16_t texture;                         // And the textures that are used. 

		struct                                    // For multi-textured particles actually three ids
		{
			uint16_t texture_0 : 5;
			uint16_t texture_1 : 5;
			uint16_t texture_2 : 5;
			uint16_t : 1;
		};
	};


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

	M2TrackFake<C3Vector> colorTrack;              // Most likely they all have 3 timestamps for {start, middle, end}.
	M2TrackFake<short> alphaTrack;                 // FIXME FIXED16
	M2TrackFake<C2Vector> scaleTrack;
	C2Vector scaleVary;                       // A percentage amount to randomly vary the scale of each particle
	M2TrackFake<uint16_t> headCellTrack;           // Some kind of intensity values seen: 0,16,17,32 (if set to different it will have high intensity)
	M2TrackFake<uint16_t> tailCellTrack;

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

	struct vector_2fp_6_9 { short x; short y; };

	vector_2fp_6_9 multiTextureParam0[2];
	vector_2fp_6_9 multiTextureParam1[2];
};

//---------------------------

struct ModelHeader
{
	char id[4];                                          // MD20 Magic
	uint32_t version;
	M2Array<char> name;                                  // should be globally unique, used to reload by name in internal clients

	struct
	{
		uint32_t flag_tilt_x : 1;
		uint32_t flag_tilt_y : 1;
		uint32_t : 1;

		uint32_t flag_has_blend_maps : 1;                   // add BlendMaps fields in header
		uint32_t : 1;

		uint32_t : 27;
	} global_flags;

	M2Array<M2Loop> global_loops;                        // Timestamps used in global looping animations.

	M2Array<M2Sequence> sequences;                       // Information about the animations in the model.
	M2Array<uint16_t> sequence_lookups;                  // Mapping of sequence IDs to the entries in the Animation sequences block.

	M2Array<M2CompBone> bones;                           // MAX_BONES = 0x100
	M2Array<uint16_t> key_bone_lookup;                   // Lookup table for key skeletal bones.

	M2Array<M2Vertex> vertices;

	uint32_t num_skin_profiles;                          // Views (LOD) are now in .skins.

	M2Array<M2Color> colors;                             // Color and alpha animations definitions.

	M2Array<M2Texture> textures;
	M2Array<M2TextureWeight> texture_weights;            // Transparency of textures.
	M2Array<M2TextureTransform> texture_transforms;
	
	M2Array<uint16_t> replacable_texture_lookup;
	M2Array<M2Material> materials;                       // Blending modes / render flags.
	M2Array<uint16_t> bone_lookup_table;
	M2Array<uint16_t> texture_lookup_table;
	M2Array<uint16_t> tex_unit_lookup_table;             // ≥ Cata: unused
	M2Array<uint16_t> transparency_lookup_table;
	M2Array<uint16_t> texture_transforms_lookup_table;

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

	M2Array<M2Particle> particle_emitters;
};


///////////////////
///////////////////
///////////////////


struct M2SkinSection
{
	uint16_t skinSectionId;       // Mesh part ID, see below.
	uint16_t Level;               // (level << 16) is added (|ed) to startTriangle and alike to avoid having to increase those fields to uint32s.
	uint16_t vertexStart;         // Starting vertex number.
	uint16_t vertexCount;         // Number of vertices.
	uint16_t indexStart;          // Starting triangle index (that's 3* the number of triangles drawn so far).
	uint16_t indexCount;          // Number of triangle indices.
	uint16_t boneCount;           // Number of elements in the bone lookup table.
	uint16_t boneComboIndex;      // Starting index in the bone lookup table.
	uint16_t boneInfluences;      // <= 4
								  // from <=BC documentation: Highest number of bones needed at one time in this Submesh --Tinyn (wowdev.org) 
								  // In 2.x this is the amount of of bones up the parent-chain affecting the submesh --NaK
	uint16_t centerBoneIndex;
	C3Vector centerPosition;     // Average position of all the vertices in the sub mesh.

	C3Vector sortCenterPosition; // The center of the box when an axis aligned box is built around the vertices in the submesh.
	float sortRadius;             // Distance of the vertex farthest from CenterBoundingBox.

};

struct M2Batch
{
	uint8_t flags;                       // Usually 16 for static textures, and 0 for animated textures. &0x1: materials invert something; &0x2: transform &0x4: projected texture; &0x10: something batch compatible; &0x20: projected texture?; &0x40: use textureWeights
	int8_t priorityPlane;
	uint16_t shader_id;                  // See below.
	uint16_t skinSectionIndex;           // A duplicate entry of a submesh from the list above.
	uint16_t geosetIndex;                // See below.
	uint16_t colorIndex;                 // A Color out of the Colors-Block or -1 if none.
	uint16_t materialIndex;              // The renderflags used on this texture-unit.
	uint16_t materialLayer;              // Capped at 7 (see CM2Scene::BeginDraw)
	uint16_t textureCount;               // 1 to 4. See below. Also seems to be the number of textures to load, starting at the texture lookup in the next field (0x10).
	uint16_t textureComboIndex;          // Index into Texture lookup table
	uint16_t textureCoordComboIndex;     // Index into the texture unit lookup table.
	uint16_t textureWeightComboIndex;    // Index into transparency lookup table.
	uint16_t textureTransformComboIndex; // Index into uvanimation lookup table. 
};

struct M2ShadowBatch
{
	uint8_t flags;              // if auto-generated: M2Batch.flags & 0xFF
	uint8_t flags2;             // if auto-generated: (renderFlag[i].flags & 0x04 ? 0x01 : 0x00)
								//                  | (!renderFlag[i].blendingmode ? 0x02 : 0x00)
								//                  | (renderFlag[i].flags & 0x80 ? 0x04 : 0x00)
								//                  | (renderFlag[i].flags & 0x400 ? 0x06 : 0x00)
	uint16_t _unknown1;
	uint16_t submesh_id;
	uint16_t texture_id;        // already looked-up
	uint16_t color_id;
	uint16_t transparency_id;   // already looked-up
};



// GENERAL HEADER

struct M2SkinProfile
{
	uint32_t magic;                         // 'SKIN'
	M2Array<uint16_t> vertices;
	M2Array<uint16_t> indices;
	M2Array<uint8_t> bones;                 // uint4t FIXME
	M2Array<M2SkinSection> submeshes;
	M2Array<M2Batch> m_WMOBatchIndexes;
	uint32_t boneCountMax;                  // WoW takes this and divides it by the number of bones in each submesh, then stores the biggest one.
											// Maximum number of bones per drawcall for each view. Related to (old) GPU numbers of registers. 
											// Values seen : 256, 64, 53, 21

	M2Array<M2ShadowBatch> shadow_batches;
};

#define	TEXTUREUNIT_STATIC	16

#define	TEXTURE_MAX	128

#include "../shared/pack_end.h"