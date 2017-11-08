#pragma once

#include "Animated.h"

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
    vec3 ModelRotationSpeedMin;
    vec3 ModelRotationSpeedMax;
};

struct M2Range
{
    uint32 minimum;
    uint32 maximum;
};




////////

struct M2Loop
{
    uint32 timestamp;
};

//

struct M2Sequence
{
    uint16 id;                   // Animation id in AnimationData.dbc
    uint16 variationIndex;       // Sub-animation id: Which number in a row of animations this one is.
    uint32 duration;             // The length of this animation sequence in milliseconds.

    float movespeed;               // This is the speed the character moves with in this animation.
    uint32 flags;                // See below.
    int16 frequency;             // This is used to determine how often the animation is played. For all animations of the same type, this adds up to 0x7FFF (32767).
    uint16 unk0;                  // Padding
    M2Range replay;                // May both be 0 to not repeat. Client will pick a random number of repetitions within bounds if given.
    uint32 blendTime;

    M2Bounds bounds;
    int16 variationNext;         // id of the following animation of this AnimationID, points to an Index or is -1 if none.
    uint16 aliasNext;            // id in the list of animations. Used to find actual animation if this sequence is an alias (flags & 0x40)
};

//

struct M2CompBone
{
    int32 key_bone_id;            // Back-reference to the key bone lookup table. -1 if this is no key bone.

    struct
    {
        uint32 __0 : 1;
        uint32 __1 : 1;
        uint32 __2 : 1;
        uint32 spherical_billboard : 1;
        uint32 cylindrical_billboard_lock_x : 1;
        uint32 cylindrical_billboard_lock_y : 1;
        uint32 cylindrical_billboard_lock_z : 1;
        uint32 __3 : 1;
        uint32 __4 : 1;
        uint32 transformed : 1;
        uint32 kinematic_bone : 1;       // MoP+: allow physics to influence this bone
        uint32 __5 : 1;
        uint32 helmet_anim_scaled : 1;   // set blend_modificator to helmetAnimScalingRec.m_amount for this bone
        uint32 : 19;
    } flags;

    int16 parent_bone;            // Parent bone ID or -1 if there is none.
    uint16 submesh_id;            // Mesh part ID OR uDistToParent?

    union
    {
        struct
        {
            uint16 uDistToFurthDesc;
            uint16 uZRatioOfChain;
        } CompressData;
        uint32 boneNameCRC;         // these are for debugging only. their bone names match those in key bone lookup.
    };

    M2Track<vec3> translation;
    M2Track<M2CompQuat> rotation;   // compressed values, default is (32767,32767,32767,65535) == (0,0,0,1) == identity
    M2Track<vec3> scale;

    vec3 pivot;                 // The pivot point of that bone.
};

//

struct M2Vertex
{
    vec3 pos;
    uint8 bone_weights[4];
    uint8 bone_indices[4];
    vec3 normal;
    vec2 tex_coords[2];  // two m_DiffuseTextures, depending on shader used
};

//

struct M2Color
{
    M2Track<vec3> color;                        // vertex colors in rgb order
    M2Track<short> alpha;    // FIXED 16        // 0 - transparent, 0x7FFF - opaque. Normaly NonInterp
};

//

struct M2Texture
{
    uint32 type;
    struct
    {
        uint32 M2TEXTURE_FLAGS_WRAPX : 1;
        uint32 M2TEXTURE_FLAGS_WRAPY : 1;
        uint32 : 30;
    } flags;
    M2Array<char> filename; // for non-hardcoded m_DiffuseTextures (type != 0), this still points to a zero-sized string
};

//

struct M2TextureWeight
{
    M2Track<short> weight;
};

//

struct M2TextureTransform
{
    M2Track<vec3> translation;
    M2Track<Quaternion> rotation;    // rotation center is texture center (0.5, 0.5)
    M2Track<vec3> scaling;
};

//

struct M2Material
{
    struct
    {
        uint16 M2MATERIAL_FLAGS_UNLIT : 1;
        uint16 M2MATERIAL_FLAGS_UNFOGGED : 1;
        uint16 M2MATERIAL_FLAGS_TWOSIDED : 1;
        uint16 M2MATERIAL_FLAGS_DEPTHTEST : 1;
        uint16 M2MATERIAL_FLAGS_DEPTHWRITE : 1;
        uint16 : 11;
    } flags;
    uint16 blending_mode;
};

//

struct M2Attachment
{
    uint32 id;                        // Referenced in the lookup-block below.
    uint16 bone;                      // attachment base
    uint16 unk0;
    vec3 position;                      // relative to bone; Often this value is the same as bone's pivot point 
    M2Track<uint8> animate_attached;  // whether or not the attached model is animated when this model is. only a bool is used. default is true.
};

//

struct M2Event
{
    uint32 identifier;  // mostly a 3 character name prefixed with '$'.
    uint32 data;        // This data is passed when the event is fired. 
    uint32 bone;        // Somewhere it has to be attached.
    vec3 position;    // Relative to that bone of course, animated. Pivot without animating.
    M2TrackBase enabled;  // This is a timestamp-only animation block. It is built up the same as a normal AnimationBlocks, but is missing values, as every timestamp is an implicit "fire now".
};

//

struct M2Light
{
    uint16 type;                      // Types are listed below.
    int16 bone;                       // -1 if not attached to a bone
    vec3 position;                 // relative to bone, if given

    M2Track<vec3> ambient_color;
    M2Track<float> ambient_intensity;   // defaults to 1.0

    M2Track<vec3> diffuse_color;
    M2Track<float> diffuse_intensity;   // defaults to 1.0

    M2Track<float> attenuation_start;
    M2Track<float> attenuation_end;

    M2Track<uint8> visibility;          // enabled?
};

//

struct M2Camera // TODO Spline keys
{
    uint32 type; // 0: portrait, 1: characterinfo; -1: else (flyby etc.); referenced backwards in the lookup table.

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
    uint32 ribbonId;                  // Always (as I have seen): -1.
    uint32 boneIndex;                 // A bone to attach to.
    vec3 position;                 // And a position, relative to that bone.
    M2Array<uint16> textureIndices;   // into m_DiffuseTextures
    M2Array<uint16> materialIndices;  // into materials

    M2Track<vec3> colorTrack;
    M2Track<short> alphaTrack;          // FIXME FIXED 16               // And an alpha value in a short, where: 0 - transparent, 0x7FFF - opaque.
    M2Track<float> heightAboveTrack;
    M2Track<float> heightBelowTrack;        // do not set to same!

    float edgesPerSecond;                  // this defines how smooth the ribbon is. A low value may produce a lot of edges.
    float edgeLifetime;                    // the length aka Lifespan. in seconds
    float gravity;                      // use arcsin(val) to get the emission angle in degree

    uint16 textureRows;                    // tiles in texture
    uint16 textureCols;

    M2Track<uint16> texSlotTrack;
    M2Track<uint8> visibilityTrack;

    int16 priorityPlane;
    uint16 padding;
};

//

struct M2Particle
{
    uint32 particleId;                      // Always (as I have seen): -1.
    struct
    {
        uint32 M2PARTICLE_FLAGS_AFFECTEDBYLIGHTING : 1;
        uint32 unk0 : 2;
        uint32 M2PARTICLE_FLAGS_WORLDUP : 1;
        uint32 M2PARTICLE_FLAGS_DONOTTRAIL : 1;
        uint32 M2PARTICLE_FLAGS_INLIGHTING : 1;
        uint32 unk1 : 6;
        uint32 M2PARTICLE_FLAGS_DONOTBILLBOARD : 1;
        uint32 : 19;
    } flags;

    vec3 Position;                        // The position. Relative to the following bone.
    uint16 bone;                            // The bone its attached to.

    union
    {
        uint16 texture;                         // And the m_DiffuseTextures that are used. 
        struct                                    // For multi-textured particles actually three ids
        {
            uint16 texture_0 : 5;
            uint16 texture_1 : 5;
            uint16 texture_2 : 5;
            uint16 : 1;
        };
    };

    M2Array<char> geometry_model_filename;    // if given, this emitter spawns models
    M2Array<char> recursion_model_filename;   // if given, this emitter is an alias for the (maximum 4) emitters of the given model

    uint8 blendingType;                     // A blending type for the particle. See Below
    uint8 emitterType;                      // 1 - Plane (rectangle), 2 - Sphere, 3 - Spline, 4 - Bone
    uint16 particleColorIndex;              // This one is used for ParticleColor.dbc. See below.

    uint8 particleType;                     // Found below.
    uint8 headorTail;                       // 0 - Head, 1 - Tail, 2 - Both 

    uint16 textureTileRotation;             // Rotation for the texture tile. (Values: -1,0,1) -- priorityPlane
    uint16 textureDimensions_rows;          // for tiled m_DiffuseTextures
    uint16 textureDimensions_columns;

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

    M2TrackFake<vec3> colorTrack;              // Most likely they all have 3 timestamps for {start, middle, end}.
    M2TrackFake<short> alphaTrack;                 // FIXME FIXED16

    M2TrackFake<vec2> scaleTrack;
    vec2 scaleVary;                       // A percentage amount to randomly vary the scale of each particle
    M2TrackFake<uint16> headCellTrack;           // Some kind of intensity values seen: 0,16,17,32 (if set to different it will have high intensity)
    M2TrackFake<uint16> tailCellTrack;

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
    vec3 WindVector;
    //float WindTime;

    float followSpeed1;
    float followScale1;
    float followSpeed2;
    float followScale2;

    M2Array<vec3> splinePoints;            //Set only for spline praticle emitter. Contains array of points for spline

    // Params END

    M2Track<uint8> enabledIn;                // (boolean) Appears to be used sparely now, probably there's a flag that links particles to animation sets where they are enabled.

    struct vector_2fp_6_9 { short x; short y; };

    vector_2fp_6_9 multiTextureParam0[2];
    vector_2fp_6_9 multiTextureParam1[2];
};

//---------------------------

struct ModelHeader
{
    char id[4];                                          // MD20 Magic
    uint32 version;
    M2Array<char> name;                                  // should be globally unique, used to reload by name in internal clients

    struct
    {
        uint32 flag_tilt_x : 1;
        uint32 flag_tilt_y : 1;
        uint32 unk0 : 1;
        uint32 flag_has_blend_maps : 1;                  // add BlendMaps fields in header
        uint32 unk1 : 1;
        uint32 : 27;
    } global_flags;
    M2Array<M2Loop> global_loops;                        // Timestamps used in global looping animations.


    // Sequences
    M2Array<M2Sequence> sequences;                       // Information about the animations in the model.
    M2Array<uint16> sequence_lookups;                    // Mapping of sequence IDs to the entries in the Animation sequences block.

    // Bones
    M2Array<M2CompBone> bones;                           // MAX_BONES = 0x100
    M2Array<uint16> key_bone_lookup;                     // Lookup table for key skeletal bones.

    // Vertices
    M2Array<M2Vertex> vertices;

    // Skin
    uint32 num_skin_profiles;                            // Views (LOD) are now in .skins.

    // Materials
    M2Array<M2Color> colors;                             // Color and alpha animations definitions.

    M2Array<M2Texture> textures;
    M2Array<M2TextureWeight> texture_weights;            // Transparency of m_DiffuseTextures.
    M2Array<M2TextureTransform> texture_transforms;

    M2Array<uint16> replacable_texture_lookup;
    M2Array<M2Material> materials;                       // Blending modes / render flags.
    M2Array<uint16> bone_lookup_table;
    M2Array<uint16> texture_lookup_table;
    M2Array<uint16> __unused1;
    M2Array<uint16> transparency_lookup_table;
    M2Array<uint16> texture_transforms_lookup_table;


    // Bounding box
    CAaBox bounding_box;                                 // min/max( [1].z, 2.0277779f ) - 0.16f seems to be the maximum camera height
    float bounding_sphere_radius;                        // detail doodad draw dist = clamp (bounding_sphere_radius * detailDoodadDensityFade * detailDoodadDist, …)

    CAaBox collision_box;
    float collision_sphere_radius;


    // Collision stuff
    M2Array<uint16> m_CollisionTriangles;
    M2Array<vec3> m_CollisionVertices;
    M2Array<vec3> m_CollisionNormals;


    // Attachments
    M2Array<M2Attachment> attachments;
    M2Array<uint16> attachment_lookup_table;


    // Events
    M2Array<M2Event> events;


    // Light
    M2Array<M2Light> lights;


    // Camara
    M2Array<M2Camera> cameras;
    M2Array<uint16> camera_lookup_table;


    // Emitters and particles
    M2Array<M2Ribbon> ribbon_emitters;
    M2Array<M2Particle> particle_emitters;
};


///////////////////
///////////////////
///////////////////

#define	TEXTURE_MAX	128

#include "../shared/pack_end.h"