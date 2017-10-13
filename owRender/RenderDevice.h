#pragma once

#include "GPUTimer.h"

const uint32 MaxNumVertexLayouts = 64;
const uint32 MaxComputeImages = 8;

// ---------------------------------------------------------
// General
// ---------------------------------------------------------

template<class T>
class RDIObjects
{
public:
	uint32 add(const T &obj)
	{
		if (!_freeList.empty())
		{
			uint32 index = _freeList.back();
			_freeList.pop_back();
			_objects[index] = obj;
			return index + 1;
		}
		else
		{
			_objects.push_back(obj);
			return (uint32)_objects.size();
		}
	}

	void remove(uint32 handle)
	{
		assert1(handle > 0 && handle <= _objects.size());
		assert1(std::find(_freeList.begin(), _freeList.end(), handle - 1) == _freeList.end());

		_objects[handle - 1] = T();  // Destruct and replace with default object
		_freeList.push_back(handle - 1);
	}

	T& getRef(uint32 handle)
	{
		assert1(handle > 0 && handle <= _objects.size());

		return _objects[handle - 1];
	}

private:
	std::vector< T >       _objects;
	std::vector< uint32 >  _freeList;
};


struct DeviceCaps
{
	uint16	maxJointCount;
	uint16	maxTexUnitCount;
	bool	texFloat;
	bool	texNPOT;
	bool	rtMultisampling;
	bool	geometryShaders;
	bool	tesselation;
	bool	computeShaders;
	bool	instancing;
};


// ---------------------------------------------------------
// Vertex layout
// ---------------------------------------------------------

struct VertexLayoutAttrib
{
	std::string  semanticName;
	uint32       vbSlot;
	uint32       size;
	uint32       offset;
};

struct R_VertexLayout
{
	uint32              numAttribs;
	VertexLayoutAttrib  attribs[16];
};


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
		BGRA8,
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

struct R_TexSlot
{
	R_TexSlot() :
		texObj(0),
		samplerState(0),
		usage(0)
	{}

	R_TexSlot(uint32 texObj, uint32 samplerState, uint32 usage) :
		texObj(texObj),
		samplerState(samplerState),
		usage(usage)
	{}

	//

	uint32  texObj;
	uint32  samplerState;
	uint32  usage;
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
	BS_BLEND_DEST_COLOR,
	BS_BLEND_SRC_COLOR,
	BS_BLEND_INV_DEST_COLOR,
	BS_BLEND_INV_SRC_COLOR
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

enum RDIClearFlags
{
	CLR_COLOR_RT0 = 0x00000001,
	CLR_COLOR_RT1 = 0x00000002,
	CLR_COLOR_RT2 = 0x00000004,
	CLR_COLOR_RT3 = 0x00000008,
	CLR_DEPTH = 0x00000010
};

enum RDIIndexFormat
{
	IDXFMT_16 = 0,
	IDXFMT_32
};

enum RDIPrimType
{
	PRIM_TRILIST = 0,
	PRIM_TRISTRIP,
	PRIM_LINES,
	PRIM_POINTS,
	PRIM_PATCHES
};

enum RDIDrawBarriers
{
	NotSet = 0,
	VertexBufferBarrier,		// Wait till vertex buffer is updated by shaders
	IndexBufferBarrier,			// Wait till index buffer is updated by shaders
	ImageBarrier				// Wait till image is updated by shaders
};

// ---------------------------------------------------------
// Buffers
// ---------------------------------------------------------

struct RDIBufferGL4
{
	uint32  type;
	uint32  glObj;
	uint32  size;
	int		geometryRefCount;

	RDIBufferGL4() : type(0), glObj(0), size(0), geometryRefCount(0) {}
};

struct RDIVertBufSlotGL4
{
	uint32  vbObj;
	uint32  offset;
	uint32  stride;

	RDIVertBufSlotGL4() : vbObj(0), offset(0), stride(0) {}
	RDIVertBufSlotGL4(uint32 vbObj, uint32 offset, uint32 stride) :
		vbObj(vbObj), offset(offset), stride(stride)
	{}
};

struct RDIGeometryInfoGL4
{
	std::vector< RDIVertBufSlotGL4 > vertexBufInfo;
	uint32 vao;
	uint32 indexBuf;
	uint32 layout;
	bool indexBuf32Bit;
	bool atrribsBinded;

	RDIGeometryInfoGL4() : vao(0), indexBuf(0), layout(0), indexBuf32Bit(false), atrribsBinded(false) {}
};

struct R_ShaderStorageGL4
{
	R_ShaderStorageGL4(uint8 targetSlot, uint32 glObj) :
		oglObject(glObj),
		slot(targetSlot)
	{}

	//

	uint32 oglObject;
	uint8 slot;
};



// ---------------------------------------------------------
// Textures
// ---------------------------------------------------------

struct RDITextureGL4
{
	uint32                glObj;
	uint32                glFmt;
	int                   type;
	R_TextureFormats::List  format;
	int                   width, height, depth;
	int                   memSize;
	uint32                samplerState;
	bool                  sRGB;
	bool                  hasMips, genMips;

	RDITextureGL4() : glObj(0), glFmt(0), type(0), format(R_TextureFormats::Unknown), width(0), height(0),
		depth(0), memSize(0), samplerState(0), sRGB(false), hasMips(false), genMips(false)
	{

	}
};

struct RDITexSlotGL4
{
	uint32  texObj;
	uint32  samplerState;

	RDITexSlotGL4() : texObj(0), samplerState(0) {}
	RDITexSlotGL4(uint32 texObj, uint32 samplerState) :
		texObj(texObj), samplerState(samplerState)
	{}
};

struct RDITextureBufferGL4
{
	uint32  bufObj;
	uint32  glFmt;
	uint32	glTexID;

	RDITextureBufferGL4() : bufObj(0), glFmt(0), glTexID(0) {}
};



// ---------------------------------------------------------
// Shaders
// ---------------------------------------------------------

struct RDIInputLayoutGL4
{
	bool  valid;
	int8  attribIndices[16];

	RDIInputLayoutGL4() : valid(false)
	{
		memset(attribIndices, 0, sizeof(attribIndices));
	}
};

struct RDIShaderGL4
{
	uint32				oglProgramObj;
	RDIInputLayoutGL4	inputLayouts[MaxNumVertexLayouts];

	RDIShaderGL4() : oglProgramObj(0)
	{

	}
};



// ---------------------------------------------------------
// Render buffers
// ---------------------------------------------------------

struct RDIRenderBufferGL4
{
	static const uint32 MaxColorAttachmentCount = 4;

	uint32  fbo, fboMS;  // fboMS: Multisampled FBO used when samples > 0
	uint32  width, height;
	uint32  samples;

	uint32  depthTex, colTexs[MaxColorAttachmentCount];
	uint32  depthBuf, colBufs[MaxColorAttachmentCount];  // Used for multisampling

	RDIRenderBufferGL4() : fbo(0), fboMS(0), width(0), height(0), depthTex(0), depthBuf(0), samples(0)
	{
		for (uint32 i = 0; i < MaxColorAttachmentCount; ++i) colTexs[i] = colBufs[i] = 0;
	}
};



// =================================================================================================

class RenderDevice
{
public:
	RenderDevice();
	~RenderDevice();

	void initStates();
	bool init();

	// -----------------------------------------------------------------------------
	// Resources
	// -----------------------------------------------------------------------------

	// Vertex layouts
	uint32 registerVertexLayout(uint32 numAttribs, VertexLayoutAttrib *attribs);

	// Rendering
	void beginRendering();
	uint32 beginCreatingGeometry(uint32 vlObj);
	void finishCreatingGeometry(uint32 geoObj);
	void setGeomVertexParams(uint32 geoObj, uint32 vbo, uint32 vbSlot, uint32 offset, uint32 stride);
	void setGeomIndexParams(uint32 geoObj, uint32 indBuf, RDIIndexFormat format);
	void destroyGeometry(uint32 &geoObj, bool destroyBindedBuffers);

	// Buffers
	uint32 createVertexBuffer(uint32 size, const void *data);
	uint32 createIndexBuffer(uint32 size, const void *data);
	uint32 createTextureBuffer(R_TextureFormats::List format, uint32 bufSize, const void *data);
	uint32 createShaderStorageBuffer(uint32 size, const void *data);
	void destroyBuffer(uint32 &bufObj);
	void destroyTextureBuffer(uint32& bufObj);
	void updateBufferData(uint32 geoObj, uint32 bufObj, uint32 offset, uint32 size, void *data);
	void *mapBuffer(uint32 geoObj, uint32 bufObj, uint32 offset, uint32 size, R_BufferMappingTypes mapType);
	void unmapBuffer(uint32 geoObj, uint32 bufObj);
	uint32 getBufferMem() const
	{
		return _bufferMem;
	}

	// Textures
	uint32 calcTextureSize(R_TextureFormats::List format, int width, int height, int depth);
	uint32 createTexture(R_TextureTypes::List type, int width, int height, int depth, R_TextureFormats::List format, bool hasMips, bool genMips, bool compress, bool sRGB);
	void uploadTextureData(uint32 texObj, int slice, int mipLevel, const void *pixels);
	void destroyTexture(uint32 &texObj);
	void updateTextureData(uint32 texObj, int slice, int mipLevel, const void *pixels);
	bool getTextureData(uint32 texObj, int slice, int mipLevel, void *buffer);
	void bindImageToTexture(uint32 texObj, void* eglImage);
	uint32 getTextureMem() const
	{
		return _textureMem;
	}

	// Shaders
	uint32 createShader(const char *vertexShaderSrc, const char *fragmentShaderSrc, const char *geometryShaderSrc, const char *tessControlShaderSrc, const char *tessEvaluationShaderSrc, const char *computeShaderSrc);
	void destroyShader(uint32 &shaderId);
	void bindShader(uint32 shaderId);
	int getShaderConstLoc(uint32 shaderId, const char *name);
	int getShaderSamplerLoc(uint32 shaderId, const char *name);
	int getShaderBufferLoc(uint32 shaderId, const char *name);
	void setShaderConst(int loc, R_ShaderConstType type, void *values, uint32 count = 1);
	void setShaderSampler(int loc, uint32 texUnit);
	const char* getDefaultVSCode();
	const char* getDefaultFSCode();
	void runComputeShader(uint32 shaderId, uint32 xDim, uint32 yDim, uint32 zDim);
	string getShaderLog() const
	{
		return _shaderLog;
	}

	// Renderbuffers
	uint32 createRenderBuffer(uint32 width, uint32 height, R_TextureFormats::List format, bool depth, uint32 numColBufs, uint32 samples);
	void destroyRenderBuffer(uint32 &rbObj);
	uint32 getRenderBufferTex(uint32 rbObj, uint32 bufIndex);
	void setRenderBuffer(uint32 rbObj);
	bool getRenderBufferData(uint32 rbObj, int bufIndex, int *width, int *height, int *compCount, void *dataBuffer, int bufferSize);
	void getRenderBufferDimensions(uint32 rbObj, int *width, int *height);

	// Queries
	uint32 createOcclusionQuery();
	void destroyQuery(uint32 queryObj);
	void beginQuery(uint32 queryObj);
	void endQuery(uint32 queryObj);
	uint32 getQueryResult(uint32 queryObj);

	// Render Device dependent GPU Timer
	GPUTimer* createGPUTimer()
	{
		return new GPUTimer();
	}

	// -----------------------------------------------------------------------------
	// Commands
	// -----------------------------------------------------------------------------
	void setViewport(int x, int y, int width, int height)
	{
		_vpX = x;
		_vpY = y;
		_vpWidth = width;
		_vpHeight = height;
		_pendingMask |= PM_VIEWPORT;
	}
	void setScissorRect(int x, int y, int width, int height)
	{
		_scX = x;
		_scY = y;
		_scWidth = width;
		_scHeight = height;
		_pendingMask |= PM_SCISSOR;
	}
	void setGeometry(uint32 geoIndex)
	{
		_curGeometryIndex = geoIndex;
		_pendingMask |= PM_GEOMETRY;
	}
	void setTexture(uint32 slot, uint32 texObj, uint16 samplerState, uint16 usage)
	{
		assert1(slot < 16);
		_texSlots[slot] = R_TexSlot(texObj, samplerState, usage);
		_pendingMask |= PM_TEXTURES;
	}
	void setMemoryBarrier(RDIDrawBarriers barrier)
	{
		_memBarriers = barrier;
		_pendingMask |= PM_BARRIER;
	}
	void setStorageBuffer(uint8 slot, uint32 bufObj)
	{
		assert1(slot < _maxComputeBufferAttachments && _storageBufs.size() < _maxComputeBufferAttachments);

		RDIBufferGL4 &buf = _buffers.getRef(bufObj);
		_storageBufs.push_back(R_ShaderStorageGL4(slot, buf.glObj));

		_pendingMask |= PM_COMPUTE;
	}

	// Render states
	void setColorWriteMask(bool enabled)
	{
		_newRasterState.renderTargetWriteMask = enabled;
		_pendingMask |= PM_RENDERSTATES;
	}
	void getColorWriteMask(bool &enabled) const
	{
		enabled = _newRasterState.renderTargetWriteMask;
	}

	void setFillMode(R_FillMode fillMode)
	{
		_newRasterState.fillMode = fillMode;
		_pendingMask |= PM_RENDERSTATES;
	}
	void getFillMode(R_FillMode &fillMode) const
	{
		fillMode = (R_FillMode)_newRasterState.fillMode;
	}

	void setCullMode(R_CullMode cullMode)
	{
		_newRasterState.cullMode = cullMode;
		_pendingMask |= PM_RENDERSTATES;
	}
	void getCullMode(R_CullMode &cullMode) const
	{
		cullMode = (R_CullMode)_newRasterState.cullMode;
	}

	void setScissorTest(bool enabled)
	{
		_newRasterState.scissorEnable = enabled;
		_pendingMask |= PM_RENDERSTATES;
	}
	void getScissorTest(bool &enabled) const
	{
		enabled = _newRasterState.scissorEnable;
	}

	void setMulisampling(bool enabled)
	{
		_newRasterState.multisampleEnable = enabled;
		_pendingMask |= PM_RENDERSTATES;
	}
	void getMulisampling(bool &enabled) const
	{
		enabled = _newRasterState.multisampleEnable;
	}

	void setAlphaToCoverage(bool enabled)
	{
		_newBlendState.alphaToCoverageEnable = enabled;
		_pendingMask |= PM_RENDERSTATES;
	}
	void getAlphaToCoverage(bool &enabled) const
	{
		enabled = _newBlendState.alphaToCoverageEnable;
	}

	void setBlendMode(bool enabled, R_BlendFunc srcBlendFunc = BS_BLEND_ZERO, R_BlendFunc destBlendFunc = BS_BLEND_ZERO)
	{
		_newBlendState.blendEnable = enabled;
		_newBlendState.srcBlendFunc = srcBlendFunc;
		_newBlendState.destBlendFunc = destBlendFunc;
		_pendingMask |= PM_RENDERSTATES;
	}
	void getBlendMode(bool &enabled, R_BlendFunc &srcBlendFunc, R_BlendFunc &destBlendFunc) const
	{
		enabled = _newBlendState.blendEnable;
		srcBlendFunc = (R_BlendFunc)_newBlendState.srcBlendFunc;
		destBlendFunc = (R_BlendFunc)_newBlendState.destBlendFunc;
	}

	void setDepthMask(bool enabled)
	{
		_newDepthStencilState.depthWriteMask = enabled;
		_pendingMask |= PM_RENDERSTATES;
	}
	void getDepthMask(bool &enabled) const
	{
		enabled = _newDepthStencilState.depthWriteMask;
	}

	void setDepthTest(bool enabled)
	{
		_newDepthStencilState.depthEnable = enabled;
		_pendingMask |= PM_RENDERSTATES;
	}
	void getDepthTest(bool &enabled) const
	{
		enabled = _newDepthStencilState.depthEnable;
	}

	void setDepthFunc(R_DepthFunc depthFunc)
	{
		_newDepthStencilState.depthFunc = depthFunc;
		_pendingMask |= PM_RENDERSTATES;
	}
	void getDepthFunc(R_DepthFunc &depthFunc) const
	{
		depthFunc = (R_DepthFunc)_newDepthStencilState.depthFunc;
	}

	void setTessPatchVertices(uint16 verts)
	{
		_tessPatchVerts = verts;
		_pendingMask |= PM_RENDERSTATES;
	}

	bool commitStates(uint32 filter = 0xFFFFFFFF);
	void resetStates();
	const DeviceCaps &getCaps() const { return _caps; }

	// Draw calls and clears
	void clear(uint32 flags, float* colorRGBA = 0x0, float depth = 1.0f);
	void draw(RDIPrimType primType, uint32 firstVert, uint32 numVerts);
	void drawIndexed(RDIPrimType primType, uint32 firstIndex, uint32 numIndices, uint32 firstVert, uint32 numVerts);

	// -----------------------------------------------------------------------------
	// Getters
	// -----------------------------------------------------------------------------

	// WARNING: Modifying internal states may lead to unexpected behavior and/or crashes
	RDIBufferGL4& getBuffer(uint32 bufObj) { return _buffers.getRef(bufObj); }
	RDITextureGL4& getTexture(uint32 texObj) { return _textures.getRef(texObj); }
	RDIRenderBufferGL4& getRenderBuffer(uint32 rbObj) { return _rendBufs.getRef(rbObj); }

	//	friend class Renderer;

protected:
	// Buffer helper
	inline uint32 createBuffer(uint32 type, uint32 size, const void *data);
	inline void	decreaseBufferRefCount(uint32 bufObj);

	// Shader helper
	uint32 createShaderProgram(const char *vertexShaderSrc, const char *fragmentShaderSrc, const char *geometryShaderSrc, const char *tessControlShaderSrc, const char *tessEvalShaderSrc, const char *computeShaderSrc);
	bool linkShaderProgram(uint32 programObj);

	// RenderBuffer helper
	void resolveRenderBuffer(uint32 rbObj);

	void checkError();
	bool applyVertexLayout(RDIGeometryInfoGL4 &geo);
	void applySamplerState(RDITextureGL4 &tex);
	void applyRenderStates();

protected:
	enum RDIPendingMask
	{
		PM_VIEWPORT = 0x00000001,
		// 		PM_INDEXBUF      = 0x00000002,
		// 		PM_VERTLAYOUT    = 0x00000004,
		PM_TEXTUREBUFFER = 0x00000004,
		PM_TEXTURES = 0x00000008,
		PM_SCISSOR = 0x00000010,
		PM_RENDERSTATES = 0x00000020,
		PM_GEOMETRY = 0x00000040,
		PM_BARRIER = 0x00000080,
		PM_COMPUTE = 0x00000100
	};


protected:
	R_VertexLayout                    _vertexLayouts[MaxNumVertexLayouts];
	RDIObjects< RDIBufferGL4 >         _buffers;
	RDIObjects< RDITextureGL4 >        _textures;
	RDIObjects< RDITextureBufferGL4 >  _textureBuffs;
	RDIObjects< RDIShaderGL4 >         _shaders;
	RDIObjects< RDIRenderBufferGL4 >   _rendBufs;
	RDIObjects< RDIGeometryInfoGL4 >   _vaos;
	std::vector< R_ShaderStorageGL4 > _storageBufs;

	uint32                             _indexFormat;
	uint32                             _activeVertexAttribsMask;

	uint16                             _lastTessPatchVertsValue;
	uint16                             _maxComputeBufferAttachments;

	bool                               _doubleBuffered;

	//--------------------------------------------------
	// DEFAULT
	//--------------------------------------------------

	DeviceCaps					_caps;

	R_TexSlot					_texSlots[16];
	R_RasterState				_curRasterState, _newRasterState;
	R_BlendState				_curBlendState, _newBlendState;
	R_DepthStencilState		_curDepthStencilState, _newDepthStencilState;
	RDIDrawBarriers				_memBarriers;

	// 8 ssbo

	string					    _shaderLog;
	uint32						_depthFormat;
	int							_vpX, _vpY, _vpWidth, _vpHeight;
	int							_scX, _scY, _scWidth, _scHeight;
	int							_fbWidth, _fbHeight;
	uint32						_curRendBuf;
	int							_outputBufferIndex;  // Left and right eye for stereo rendering
	uint32						_textureMem, _bufferMem;

	uint32                      _numVertexLayouts;

	uint32						_prevShaderId, _curShaderId;
	uint32						_pendingMask;
	uint32						_curGeometryIndex;
	//	uint32						_curTextureBuf;
	uint32						_maxTexSlots; // specified in inherited render devices

	uint32						_tessPatchVerts; // number of vertices in patch. Used for tesselation.

	int							_defaultFBO;
	bool                        _defaultFBOMultisampled;
};
