#pragma once

// ---------------------------------------------------------
// Buffers
// ---------------------------------------------------------

enum R_BufferMappingTypes
{
	Read = 0,
	Write,
	ReadWrite
};

// ---------------------------------------------------------
// Textures
// ---------------------------------------------------------

struct R_TextureTypes
{
	enum List
	{
		Tex2D = 0,
		Tex3D,
		TexCube
	};
};

struct R_TextureFormats
{
	enum List
	{
		Unknown,
		RGBA8,
		DXT1,
		DXT3,
		DXT5,
		RGBA16F,
		RGBA32F,
		DEPTH,
		R32,
		RG32
	};
};

struct R_TextureUsage
{
	enum List
	{
		Texture = 0,
		ComputeImageRO, // read-only image
		ComputeImageWO, // write-only image
		ComputeImageRW  // read-write image
	};
};


// ---------------------------------------------------------
// Shaders
// ---------------------------------------------------------

enum R_ShaderConstType
{
	CONST_INT,
	CONST_FLOAT,
	CONST_FLOAT2,
	CONST_FLOAT3,
	CONST_FLOAT4,
	CONST_FLOAT44,
	CONST_FLOAT33
};

// ---------------------------------------------------------
// Render states
// ---------------------------------------------------------

// Note: Render states use unions to provide a hash value. Writing to and reading from different members of a
//       union is not guaranteed to work by the C++ standard but is common practice and supported by many compilers.

enum R_SamplerState
{
	SS_FILTER_BILINEAR = 0x0,
	SS_FILTER_TRILINEAR = 0x0001,
	SS_FILTER_POINT = 0x0002,

	SS_ANISO1 = 0x0,
	SS_ANISO2 = 0x0004,
	SS_ANISO4 = 0x0008,
	SS_ANISO8 = 0x0010,
	SS_ANISO16 = 0x0020,

	SS_ADDRU_CLAMP = 0x0,
	SS_ADDRU_WRAP = 0x0040,
	SS_ADDRU_CLAMPCOL = 0x0080,

	SS_ADDRV_CLAMP = 0x0,
	SS_ADDRV_WRAP = 0x0100,
	SS_ADDRV_CLAMPCOL = 0x0200,

	SS_ADDRW_CLAMP = 0x0,
	SS_ADDRW_WRAP = 0x0400,
	SS_ADDRW_CLAMPCOL = 0x0800,

	SS_ADDR_CLAMP = SS_ADDRU_CLAMP | SS_ADDRV_CLAMP | SS_ADDRW_CLAMP,
	SS_ADDR_WRAP = SS_ADDRU_WRAP | SS_ADDRV_WRAP | SS_ADDRW_WRAP,
	SS_ADDR_CLAMPCOL = SS_ADDRU_CLAMPCOL | SS_ADDRV_CLAMPCOL | SS_ADDRW_CLAMPCOL,

	SS_COMP_LEQUAL = 0x1000
};

const uint32 SS_FILTER_START = 0;
const uint32 SS_FILTER_MASK = SS_FILTER_BILINEAR | SS_FILTER_TRILINEAR | SS_FILTER_POINT;

const uint32 SS_ANISO_START = 2;
const uint32 SS_ANISO_MASK = SS_ANISO1 | SS_ANISO2 | SS_ANISO4 | SS_ANISO8 | SS_ANISO16;

const uint32 SS_ADDRU_START = 6;
const uint32 SS_ADDRU_MASK = SS_ADDRU_CLAMP | SS_ADDRU_WRAP | SS_ADDRU_CLAMPCOL;

const uint32 SS_ADDRV_START = 8;
const uint32 SS_ADDRV_MASK = SS_ADDRV_CLAMP | SS_ADDRV_WRAP | SS_ADDRV_CLAMPCOL;

const uint32 SS_ADDRW_START = 10;
const uint32 SS_ADDRW_MASK = SS_ADDRW_CLAMP | SS_ADDRW_WRAP | SS_ADDRW_CLAMPCOL;

const uint32 SS_ADDR_START = 6;
const uint32 SS_ADDR_MASK = SS_ADDR_CLAMP | SS_ADDR_WRAP | SS_ADDR_CLAMPCOL;


enum R_FillMode
{
	RS_FILL_SOLID = 0,
	RS_FILL_WIREFRAME = 1
};

enum R_CullMode
{
	RS_CULL_BACK = 0,
	RS_CULL_FRONT,
	RS_CULL_NONE,
};

struct R_RasterState
{
	union
	{
		uint32  hash;
		struct
		{
			uint32  fillMode : 1;  // R_FillMode
			uint32  cullMode : 2;  // R_CullMode
			uint32  scissorEnable : 1;
			uint32  multisampleEnable : 1;
			uint32  renderTargetWriteMask : 1;
		};
	};
};

enum R_BlendFunc
{
	BS_BLEND_ZERO = 0,
	BS_BLEND_ONE,

	BS_BLEND_SRC_ALPHA,
	BS_BLEND_INV_SRC_ALPHA,
	BS_BLEND_DEST_ALPHA,
	BS_BLEND_INV_DEST_ALPHA,

	BS_BLEND_SRC_COLOR,
	BS_BLEND_INV_SRC_COLOR,
	BS_BLEND_DEST_COLOR,
	BS_BLEND_INV_DEST_COLOR
};

struct R_BlendState
{
	union
	{
		uint32  hash;
		struct
		{
			uint32  alphaToCoverageEnable : 1;
			uint32  blendEnable : 1;
			uint32  srcBlendFunc : 4;
			uint32  destBlendFunc : 4;
		};
	};
};

enum R_DepthFunc
{
	DSS_DEPTHFUNC_LESS_EQUAL = 0,
	DSS_DEPTHFUNC_LESS,
	DSS_DEPTHFUNC_EQUAL,
	DSS_DEPTHFUNC_GREATER,
	DSS_DEPTHFUNC_GREATER_EQUAL,
	DSS_DEPTHFUNC_ALWAYS
};

struct R_DepthStencilState
{
	union
	{
		uint32  hash;
		struct
		{
			uint32  depthWriteMask : 1;
			uint32  depthEnable : 1;
			uint32  depthFunc : 4;  // R_DepthFunc
		};
	};
};

// ---------------------------------------------------------
// Draw calls and clears
// ---------------------------------------------------------

enum R_ClearFlags
{
	CLR_COLOR_RT0 = 0x00000001,
	CLR_COLOR_RT1 = 0x00000002,
	CLR_COLOR_RT2 = 0x00000004,
	CLR_COLOR_RT3 = 0x00000008,
	CLR_DEPTH = 0x00000010
};

enum R_IndexFormat
{
	IDXFMT_16 = 0,
	IDXFMT_32
};

enum R_PrimitiveType
{
	PRIM_TRILIST = 0,
	PRIM_TRISTRIP,
	PRIM_LINES,
	PRIM_POINTS,
	PRIM_PATCHES
};

enum R_DrawBarriers
{
	NotSet = 0,
	VertexBufferBarrier,		// Wait till vertex buffer is updated by shaders
	IndexBufferBarrier,			// Wait till index buffer is updated by shaders
	ImageBarrier				// Wait till image is updated by shaders
};