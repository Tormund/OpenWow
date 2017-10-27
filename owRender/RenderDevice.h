#pragma once

#include "GPUTimer.h"

const uint32 MaxNumVertexLayouts = 64;
const uint32 MaxComputeImages = 8;

// ---------------------------------------------------------
// General
// ---------------------------------------------------------

template<class T>
class R_Objects
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
		assert1(find(_freeList.begin(), _freeList.end(), handle - 1) == _freeList.end());

		_objects[handle - 1] = T();  // Destruct and replace with default object
		_freeList.push_back(handle - 1);
	}

	T& getRef(uint32 handle)
	{
		assert1(handle > 0 && handle <= _objects.size());

		return _objects[handle - 1];
	}

private:
	vector< T >       _objects;
	vector< uint32 >  _freeList;
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


#include "RenderEnums.h"
#include "RenderTypes.h"

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
	uint32 registerVertexLayout(uint32 numAttribs, R_VertexLayoutAttrib *attribs);

	// Rendering
	void beginRendering();
	uint32 beginCreatingGeometry(uint32 vlObj);
	void finishCreatingGeometry(uint32 geoObj);
	void setGeomVertexParams(uint32 geoObj, uint32 vbo, uint32 vbSlot, uint32 offset, uint32 stride);
	void setGeomIndexParams(uint32 geoObj, uint32 indBuf, R_IndexFormat format);
	void destroyGeometry(uint32 &geoObj, bool destroyBindedBuffers);

	// Buffers
	uint32 createVertexBuffer(uint32 size, const void *data);
	uint32 createIndexBuffer(uint32 size, const void *data);
	uint32 createTextureBuffer(R_TextureFormats::List format, uint32 bufSize, const void *data);
	uint32 createShaderStorageBuffer(uint32 size, const void *data);
	void destroyBuffer(uint32 &bufObj);
	void destroyTextureBuffer(uint32& bufObj);
	void updateBufferData(uint32 bufObj, uint32 offset, uint32 size, const void *data);
	void* mapBuffer(uint32 geoObj, uint32 bufObj, uint32 offset, uint32 size, R_BufferMappingTypes mapType);
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
	void setMemoryBarrier(R_DrawBarriers barrier)
	{
		_memBarriers = barrier;
		_pendingMask |= PM_BARRIER;
	}
	void setStorageBuffer(uint8 slot, uint32 bufObj)
	{
		assert1(slot < _maxComputeBufferAttachments && _storageBufs.size() < _maxComputeBufferAttachments);

		R_Buffer &buf = _buffers.getRef(bufObj);
		_storageBufs.push_back(R_ShaderStorage(slot, buf.glObj));

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
	
	// Draw calls and clears
	void clear(uint32 flags = CLR_COLOR_RT0 | CLR_COLOR_RT1 | CLR_COLOR_RT2 | CLR_COLOR_RT3 | CLR_DEPTH, float* colorRGBA = 0x0, float depth = 1.0f);
	void draw(R_PrimitiveType primType, uint32 firstVert, uint32 numVerts);
	void drawIndexed(R_PrimitiveType primType, uint32 firstIndex, uint32 numIndices, uint32 firstVert, uint32 numVerts);

	// -----------------------------------------------------------------------------
	// Getters
	// -----------------------------------------------------------------------------

	// WARNING: Modifying internal states may lead to unexpected behavior and/or crashes
	const R_Buffer& getBuffer(uint32 bufObj) { return _buffers.getRef(bufObj); }
	const R_Texture& getTexture(uint32 texObj) { return _textures.getRef(texObj); }
	const R_RenderBuffer& getRenderBuffer(uint32 rbObj) { return _rendBufs.getRef(rbObj); }
	const DeviceCaps &getCaps() const { return _caps; }

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
	bool applyVertexLayout(R_GeometryInfo &geo);
	void applySamplerState(R_Texture &tex);
	void applyRenderStates();

protected:
	enum RDIPendingMask
	{
		PM_VIEWPORT = 0x00000001,
		//PM_INDEXBUF      = 0x00000002,
		//PM_VERTLAYOUT    = 0x00000004,
		PM_TEXTUREBUFFER = 0x00000004,
		PM_TEXTURES = 0x00000008,
		PM_SCISSOR = 0x00000010,
		PM_RENDERSTATES = 0x00000020,
		PM_GEOMETRY = 0x00000040,
		PM_BARRIER = 0x00000080,
		PM_COMPUTE = 0x00000100
	};


protected:
	DeviceCaps					  _caps;

	R_VertexLayout                _vertexLayouts[MaxNumVertexLayouts];
	R_Objects< R_Buffer >         _buffers;
	R_Objects< R_Texture >        _textures;
	R_Objects< R_TextureBuffer >  _textureBuffs;
	R_Objects< R_Shader >         _shaders;
	R_Objects< R_RenderBuffer >   _rendBufs;
	R_Objects< R_GeometryInfo >   _vaos;
	vector< R_ShaderStorage >     _storageBufs;
	R_TexSlot					  _texSlots[16];
	R_RasterState				  _curRasterState, _newRasterState;
	R_BlendState				  _curBlendState, _newBlendState;
	R_DepthStencilState		      _curDepthStencilState, _newDepthStencilState;
	R_DrawBarriers				  _memBarriers;

	bool                             m_IsIndexFormat32;
	uint32                             _activeVertexAttribsMask;

	uint16                             _lastTessPatchVertsValue;
	uint16                             _maxComputeBufferAttachments;

	bool                               _doubleBuffered;

	//--------------------------------------------------
	// DEFAULT
	//--------------------------------------------------

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

	uint32						_curShaderId;
	uint32						_pendingMask;
	uint32						_curGeometryIndex;
	uint32						_maxTexSlots; // specified in inherited render devices

	uint32						_tessPatchVerts; // number of vertices in patch. Used for tesselation.

	int							_defaultFBO;
	bool                        _defaultFBOMultisampled;
};
