#include "stdafx.h"

// General
#include "RenderDevice.h"

// Additional
#include "OpenGL.h"

#define CHECK_GL_ERROR checkError();

// Log shaders
static const char *defaultShaderVS =
"#version 330\n"
"uniform mat4 viewProjMat;\n"
"uniform mat4 worldMat;\n"
"layout ( location = 0 ) in vec3 vertPos;\n"
"void main() {\n"
"	gl_Position = viewProjMat * worldMat * vec4( vertPos, 1.0 );\n"
"}\n";

static const char *defaultShaderFS =
"#version 330\n"
"out vec4 fragColor;\n"
"uniform vec4 color;\n"
"void main() {\n"
"	fragColor = color;\n"
"}\n";

// Bindings for RDI types to GL
static const uint32 dataTypes[8] = {GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, GL_UNSIGNED_INT, GL_FLOAT, GL_DOUBLE };

static const uint32 indexFormats[2] = {GL_UNSIGNED_SHORT, GL_UNSIGNED_INT};

static const uint32 primitiveTypes[5] = {GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_LINES, GL_POINTS, GL_PATCHES};

static const uint32 textureTypes[3] = {GL_TEXTURE_2D, GL_TEXTURE_3D, GL_TEXTURE_CUBE_MAP};

static const uint32 memoryBarrierType[3] = {GL_BUFFER_UPDATE_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT, GL_ELEMENT_ARRAY_BARRIER_BIT, GL_SHADER_IMAGE_ACCESS_BARRIER_BIT};

static const uint32 bufferMappingTypes[3] = {GL_MAP_READ_BIT, GL_MAP_WRITE_BIT, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT};

static const uint32 oglBlendFuncs[10] = {
	GL_ZERO, 
	GL_ONE, 

	GL_SRC_ALPHA, 
	GL_ONE_MINUS_SRC_ALPHA, 
	GL_DST_ALPHA, 
	GL_ONE_MINUS_DST_ALPHA, 

	GL_SRC_COLOR,
	GL_ONE_MINUS_SRC_COLOR,
	GL_DST_COLOR, 
	GL_ONE_MINUS_DST_COLOR
};

void glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	Log::Error("---------------");
	Log::Error("OpenGL Debug message (%d): [%s]", id, message);

	switch (source)
	{
		case GL_DEBUG_SOURCE_API:             Log::Error("Source: OpenGL API"); break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   Log::Error("Source: Window System API"); break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: Log::Error("Source: Shader Compiler"); break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:     Log::Error("Source: Third Party"); break;
		case GL_DEBUG_SOURCE_APPLICATION:     Log::Error("Source: Application"); break;
		case GL_DEBUG_SOURCE_OTHER:           Log::Error("Source: Other"); break;
	}

	switch (type)
	{
		case GL_DEBUG_TYPE_ERROR:               Log::Error("Type: Error"); break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: Log::Error("Type: Deprecated Behaviour"); break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  Log::Error("Type: Undefined Behaviour"); break;
		case GL_DEBUG_TYPE_PORTABILITY:         Log::Error("Type: Portability"); break;
		case GL_DEBUG_TYPE_PERFORMANCE:         Log::Error("Type: Performance"); break;
		case GL_DEBUG_TYPE_MARKER:              Log::Error("Type: Marker"); break;
		case GL_DEBUG_TYPE_PUSH_GROUP:          Log::Error("Type: Push Group"); break;
		case GL_DEBUG_TYPE_POP_GROUP:           Log::Error("Type: Pop Group"); break;
		case GL_DEBUG_TYPE_OTHER:               Log::Error("Type: Other"); break;
	}

	switch (severity)
	{
		case GL_DEBUG_SEVERITY_HIGH:         Log::Error("Severity: high"); break;
		case GL_DEBUG_SEVERITY_MEDIUM:       Log::Error("Severity: medium"); break;
		case GL_DEBUG_SEVERITY_LOW:          Log::Error("Severity: low"); break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: Log::Error("Severity: notification"); break;
	}


	//system("pause");
	//Log::Exit(-1);
}

RenderDevice::RenderDevice()
{
	_numVertexLayouts = 0;

	_vpX = 0; _vpY = 0; _vpWidth = 320; _vpHeight = 240;
	_scX = 0; _scY = 0; _scWidth = 320; _scHeight = 240;

	_curShaderId = 0;
	_curRendBuf = 0; _outputBufferIndex = 0;
	_textureMem = 0; _bufferMem = 0;
	_curRasterState.hash = _newRasterState.hash = 0;
	_curBlendState.hash = _newBlendState.hash = 0;
	_curDepthStencilState.hash = _newDepthStencilState.hash = 0;
	_curGeometryIndex = 1;
	_defaultFBO = 0;
	_defaultFBOMultisampled = false;
	m_IsIndexFormat32 = false;
	_activeVertexAttribsMask = 0;
	_pendingMask = 0;
	_tessPatchVerts = _lastTessPatchVertsValue = 0;
	_memBarriers = NotSet;

	_maxComputeBufferAttachments = 8;
	_storageBufs.reserve(_maxComputeBufferAttachments);

	_maxTexSlots = 32; // texture units per stage

	_doubleBuffered = false;

	// add default geometry for resetting
	R_GeometryInfo defGeom;
	defGeom.atrribsBinded = true;
	_vaos.add(defGeom);
}

RenderDevice::~RenderDevice()
{}

void RenderDevice::initStates()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	GLint value;
	glGetIntegerv(GL_SAMPLE_BUFFERS, &value);
	_defaultFBOMultisampled = value > 0;

	GLboolean doubleBuffered;
	glGetBooleanv(GL_DOUBLEBUFFER, &doubleBuffered);
	_doubleBuffered = doubleBuffered != 0;

	// Get the currently bound frame buffer object to avoid reset to invalid FBO
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &_defaultFBO);
}

bool RenderDevice::init()
{
	bool failed = false;

	char* vendor = (char*)glGetString(GL_VENDOR);
	char* renderer = (char*)glGetString(GL_RENDERER);
	char* version = (char*)glGetString(GL_VERSION);

	if (!version || !renderer || !vendor)
	{
		Log::Error("OpenGL not initialized. Make sure you have a valid OpenGL context");
		return false;
	}

	Log::Info("Initializing GL4 backend using OpenGL driver '%s' by '%s' on '%s'", version, vendor, renderer);

	// Init extensions
	if (!initOpenGLExtensions())
	{
		Log::Error("Could not find all required OpenGL function entry points");
		failed = true;
	}

	// Check that OpenGL 3.3 is available
	if (_Config.OpenGL.majorVersion * 10 + _Config.OpenGL.minorVersion < 33)
	{
		Log::Error("OpenGL 3.3 not available");
		failed = true;
	}

	// Check that required extensions are supported
	if (!_Config.OpenGL.EXT_texture_filter_anisotropic)
	{
		Log::Error("Extension EXT_texture_filter_anisotropic not supported");
		failed = true;
	}
	if (!_Config.OpenGL.EXT_texture_compression_s3tc)
	{
		Log::Error("Extension EXT_texture_compression_s3tc not supported");
		failed = true;
	}
	if (!_Config.OpenGL.EXT_texture_sRGB)
	{
		Log::Error("Extension EXT_texture_sRGB not supported");
		failed = true;
	}

	if (failed)
	{
		Log::Error("Failed to init renderer backend (OpenGL %d.%d), retrying with legacy OpenGL 2.1 backend", _Config.OpenGL.majorVersion, _Config.OpenGL.minorVersion);
		return false;
	}

	// Debug
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(glDebugOutput, nullptr);

	// Set capabilities
	_Config.DeviceCaps.texFloat = true;
	_Config.DeviceCaps.texNPOT = true;
	_Config.DeviceCaps.rtMultisampling = true;
	_Config.DeviceCaps.geometryShaders = true;
	_Config.DeviceCaps.tesselation = _Config.OpenGL.majorVersion >= 4 && _Config.OpenGL.minorVersion >= 1;
	_Config.DeviceCaps.computeShaders = _Config.OpenGL.majorVersion >= 4 && _Config.OpenGL.minorVersion >= 3;
	_Config.DeviceCaps.instancing = true;
	_Config.DeviceCaps.maxJointCount = 75; // currently, will be changed soon
	_Config.DeviceCaps.maxTexUnitCount = 96; // for most modern hardware it is 192 (GeForce 400+, Radeon 7000+, Intel 4000+). Although 96 should probably be enough.

	// Find maximum number of storage buffers in compute shader
	glGetIntegerv(GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS, (GLint *)&_maxComputeBufferAttachments);

	// Init states before creating test render buffer, to ensure binding the current FBO again
	initStates();

	// Find supported depth format (some old ATI cards only support 16 bit depth for FBOs)
	_depthFormat = GL_DEPTH_COMPONENT24;
	uint32 testBuf = createRenderBuffer(32, 32, R_TextureFormats::RGBA8, true, 1, 0);
	if (testBuf == 0)
	{
		_depthFormat = GL_DEPTH_COMPONENT16;
		Log::Warn("Render target depth precision limited to 16 bit");
	}
	else
	{
		destroyRenderBuffer(testBuf);
	}

	resetStates();

    return true;
}



// =================================================================================================
// Vertex layouts
// =================================================================================================

uint32 RenderDevice::registerVertexLayout(uint32 numAttribs, R_VertexLayoutAttrib *attribs)
{
	if (_numVertexLayouts == MaxNumVertexLayouts)
	{
		return 0;
	}

	_vertexLayouts[_numVertexLayouts].numAttribs = numAttribs;

	for (uint32 i = 0; i < numAttribs; ++i)
	{
		_vertexLayouts[_numVertexLayouts].attribs[i] = attribs[i];
	}

	return ++_numVertexLayouts;
}



// =================================================================================================
// Buffers
// =================================================================================================

void RenderDevice::beginRendering()
{
	CHECK_GL_ERROR

	//	Get the currently bound frame buffer object. 
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &_defaultFBO);
	resetStates();
}

uint32 RenderDevice::beginCreatingGeometry(uint32 vlObj)
{
	R_GeometryInfo vao;
	vao.layout = vlObj;

	uint32 vaoID;
	glGenVertexArrays(1, &vaoID);
	vao.vao = vaoID;

	return _vaos.add(vao);
}

void RenderDevice::finishCreatingGeometry(uint32 geoObj)
{
	assert1(geoObj > 0);

	R_GeometryInfo &curVao = _vaos.getRef(geoObj);
	glBindVertexArray(curVao.vao);

	// bind index buffer, if present
	if (curVao.indexBuf)
	{
		R_Buffer &buf = _buffers.getRef(curVao.indexBuf);
		assert1(buf.glObj > 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf.glObj);
	}

	uint32 newVertexAttribMask = 0;

	R_VertexLayout vl = _vertexLayouts[curVao.layout - 1];

	// Set vertex attrib pointers
	for (uint32 i = 0; i < vl.numAttribs; ++i)
	{
		R_VertexLayoutAttrib& attrib = vl.attribs[i];
		const R_VertexBufferSlot& vbSlot = curVao.vertexBufInfo[attrib.vbSlot];

		R_Buffer &buf = _buffers.getRef(curVao.vertexBufInfo[attrib.vbSlot].vbObj);
		assert1(buf.glObj != 0 && buf.type == GL_ARRAY_BUFFER || buf.type == GL_SHADER_STORAGE_BUFFER); // special case for compute buffer

		glBindBuffer(GL_ARRAY_BUFFER, buf.glObj);
        //bool isFloatingPoint = (vbSlot.type == R_DataType::T_FLOAT) || (vbSlot.type == R_DataType::T_DOUBLE);
		glVertexAttribPointer(i, attrib.size, dataTypes[vbSlot.type], GL_FALSE, vbSlot.stride, (char *)0 + vbSlot.offset + attrib.offset);

		newVertexAttribMask |= 1 << i;
	}


	for (uint32 i = 0; i < 16; ++i)
	{
		uint32 curBit = 1 << i;
		if ((newVertexAttribMask & curBit) != (_activeVertexAttribsMask & curBit))
		{
			if (newVertexAttribMask & curBit)
			{
				glEnableVertexAttribArray(i);
			}
			else
			{
				glDisableVertexAttribArray(i);
			}
		}
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void RenderDevice::setGeomVertexParams(uint32 geoObj, uint32 vbo, R_DataType type, uint32 offset, uint32 stride)
{
	R_GeometryInfo &curVao = _vaos.getRef(geoObj);
	R_Buffer &buf = _buffers.getRef(vbo);

	buf.geometryRefCount++;

	R_VertexBufferSlot attribInfo;
	attribInfo.vbObj = vbo;
    attribInfo.type = type;
	attribInfo.offset = offset;
	attribInfo.stride = stride;

	curVao.vertexBufInfo.push_back(attribInfo);
}

void RenderDevice::setGeomIndexParams(uint32 geoObj, uint32 indBuf, R_IndexFormat format)
{
	R_GeometryInfo &curVao = _vaos.getRef(geoObj);
	R_Buffer &buf = _buffers.getRef(indBuf);

	buf.geometryRefCount++;
	curVao.indexBuf = indBuf;
	curVao.indexBuf32Bit = (format == IDXFMT_32 ? true : false);
}

void RenderDevice::destroyGeometry(uint32& geoObj, bool destroyBindedBuffers)
{
	if (geoObj == 0)
	{
		return;
	}

	R_GeometryInfo &curVao = _vaos.getRef(geoObj);

	glDeleteVertexArrays(1, &curVao.vao);
	glBindVertexArray(0);

	if (destroyBindedBuffers)
	{
		for (unsigned int i = 0; i < curVao.vertexBufInfo.size(); ++i)
		{
			decreaseBufferRefCount(curVao.vertexBufInfo[i].vbObj);
			destroyBuffer(curVao.vertexBufInfo[i].vbObj);
		}

		decreaseBufferRefCount(curVao.indexBuf);
		destroyBuffer(curVao.indexBuf);
	}
	else
	{
		for (size_t i = 0; i < curVao.vertexBufInfo.size(); ++i)
		{
			decreaseBufferRefCount(curVao.vertexBufInfo[i].vbObj);
		}
		decreaseBufferRefCount(curVao.indexBuf);
	}

	_vaos.remove(geoObj);
	geoObj = 0;
}


uint32 RenderDevice::createVertexBuffer(uint32 size, const void *data, bool _isDynamic)
{
	return createBuffer(GL_ARRAY_BUFFER, size, data, _isDynamic);
}

uint32 RenderDevice::createIndexBuffer(uint32 size, const void *data, bool _isDynamic)
{
	return createBuffer(GL_ELEMENT_ARRAY_BUFFER, size, data, _isDynamic);
}

uint32 RenderDevice::createShaderStorageBuffer(uint32 size, const void *data, bool _isDynamic)
{
	if (_Config.DeviceCaps.computeShaders)
	{
		return createBuffer(GL_SHADER_STORAGE_BUFFER, size, data, _isDynamic);
	}
	else
	{
		Log::Error("Shader storage buffers are not supported on this OpenGL 4 device.");

		return 0;
	}
}

uint32 RenderDevice::createTextureBuffer(R_TextureFormats::List format, uint32 bufSize, const void *data, bool _isDynamic)
{
	R_TextureBuffer buf;

	buf.bufObj = createBuffer(GL_TEXTURE_BUFFER, bufSize, data, _isDynamic);

	glGenTextures(1, &buf.glTexID);
	glActiveTexture(GL_TEXTURE15);
	glBindTexture(GL_TEXTURE_BUFFER, buf.glTexID);

	switch (format)
	{
		case R_TextureFormats::RGBA8:
		buf.glFmt = GL_RGBA8;
		break;
		case R_TextureFormats::RGBA16F:
		buf.glFmt = GL_RGBA16F;
		break;
		case R_TextureFormats::RGBA32F:
		buf.glFmt = GL_RGBA32F;
		break;
		case R_TextureFormats::R32:
		buf.glFmt = GL_R32F;
		break;
		case R_TextureFormats::RG32:
		buf.glFmt = GL_RG32F;
		break;
		default:
		assert1(0);
		break;
	};

	// bind texture to buffer
	glTexBuffer(GL_TEXTURE_BUFFER, buf.glFmt, _buffers.getRef(buf.bufObj).glObj);

	glBindTexture(GL_TEXTURE_BUFFER, 0);
	if (_texSlots[15].texObj)
	{
		glBindTexture(_textures.getRef(_texSlots[15].texObj).type, _textures.getRef(_texSlots[15].texObj).glObj);
	}

	return _textureBuffs.add(buf);
}

void RenderDevice::destroyBuffer(uint32& bufObj)
{
	if (bufObj == 0)
	{
		return;
	}

	R_Buffer &buf = _buffers.getRef(bufObj);

	if (buf.geometryRefCount < 1)
	{
		glDeleteBuffers(1, &buf.glObj);

		_bufferMem -= buf.size;
		_buffers.remove(bufObj);
		bufObj = 0;
	}
}

void RenderDevice::destroyTextureBuffer(uint32& bufObj)
{
	if (bufObj == 0)
	{
		return;
	}

	R_TextureBuffer &buf = _textureBuffs.getRef(bufObj);
	destroyBuffer(buf.bufObj);

	glDeleteTextures(1, &buf.glTexID);

	_textureBuffs.remove(bufObj);
	bufObj = 0;
}

void RenderDevice::updateBufferData(uint32 bufObj, uint32 offset, uint32 size, const void *data)
{
	const R_Buffer& buf = _buffers.getRef(bufObj);
    assert1(buf.isDynamic);
	assert1(offset + size <= buf.size);

	glBindBuffer(buf.type, buf.glObj);

	if (offset == 0 && size == buf.size)
	{
		// Replacing the whole buffer can help the driver to avoid pipeline stalls
		glBufferData(buf.type, size, data, GL_DYNAMIC_DRAW);

		return;
	}

	glBufferSubData(buf.type, offset, size, data);
}

void* RenderDevice::mapBuffer(uint32 geoObj, uint32 bufObj, uint32 offset, uint32 size, R_BufferMappingTypes mapType)
{
	const R_Buffer& buf = _buffers.getRef(bufObj);
    assert1(buf.isDynamic);
	assert1(offset + size <= buf.size);

	glBindBuffer(buf.type, buf.glObj);

	if (offset == 0 && size == buf.size && mapType == Write)
	{
		return glMapBufferRange(buf.type, offset, size, bufferMappingTypes[mapType] | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
	}

	return glMapBufferRange(buf.type, offset, size, bufferMappingTypes[mapType]);
}

void RenderDevice::unmapBuffer(uint32 geoObj, uint32 bufObj)
{
	const R_Buffer& buf = _buffers.getRef(bufObj);
    assert1(buf.isDynamic);

	// multiple buffers can be mapped at the same time, so bind the one that needs to be unmapped
	glBindBuffer(buf.type, buf.glObj);

	glUnmapBuffer(buf.type);
}

// Helpers

uint32 RenderDevice::createBuffer(uint32 bufType, uint32 size, const void *data, bool _isDynamic)
{
	R_Buffer buf;

	buf.type = bufType;
	buf.size = size;
    buf.isDynamic = _isDynamic;

	glGenBuffers(1, &buf.glObj);
	glBindBuffer(buf.type, buf.glObj);
	glBufferData(buf.type, size, data, _isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	glBindBuffer(buf.type, 0);

	_bufferMem += size;
	return _buffers.add(buf);
}

void RenderDevice::decreaseBufferRefCount(uint32 bufObj)
{
	if (bufObj == 0) return;

	R_Buffer &buf = _buffers.getRef(bufObj);

	buf.geometryRefCount--;
}



// =================================================================================================
// Textures
// =================================================================================================

uint32 RenderDevice::calcTextureSize(R_TextureFormats::List format, int width, int height, int depth)
{
	switch (format)
	{
		case R_TextureFormats::RGBA8:
		return width * height * depth * 4;
		case R_TextureFormats::DXT1:
		return std::max(width / 4, 1) * std::max(height / 4, 1) * depth * 8;
		case R_TextureFormats::DXT3:
		return std::max(width / 4, 1) * std::max(height / 4, 1) * depth * 16;
		case R_TextureFormats::DXT5:
		return std::max(width / 4, 1) * std::max(height / 4, 1) * depth * 16;
		case R_TextureFormats::RGBA16F:
		return width * height * depth * 8;
		case R_TextureFormats::RGBA32F:
		return width * height * depth * 16;
		default:
		return 0;
	}
}

uint32 RenderDevice::createTexture(R_TextureTypes::List type, int width, int height, int depth, R_TextureFormats::List format, bool hasMips, bool genMips, bool compress, bool sRGB)
{
	assert1(depth > 0);

	if (!_Config.DeviceCaps.texNPOT)
	{
		// Check if texture is NPOT
		if ((width & (width - 1)) != 0 || (height & (height - 1)) != 0)
			Log::Warn("Texture has non-power-of-two dimensions although NPOT is not supported by GPU");
	}

	R_Texture tex;
	tex.type = textureTypes[type];
	tex.format = format;
	tex.width = width;
	tex.height = height;
	tex.depth = depth;
	tex.sRGB = sRGB /*&& _Config.sRGBLinearization*/; // FIXME
	tex.genMips = genMips;
	tex.hasMips = hasMips;

	switch (format)
	{
		case R_TextureFormats::RGBA8:
		tex.glFmt = tex.sRGB ? GL_SRGB8_ALPHA8 : GL_RGBA8;
		break;
		case R_TextureFormats::DXT1:
		tex.glFmt = tex.sRGB ? GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT : GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
		case R_TextureFormats::DXT3:
		tex.glFmt = tex.sRGB ? GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT : GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
		case R_TextureFormats::DXT5:
		tex.glFmt = tex.sRGB ? GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT : GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
		case R_TextureFormats::RGBA16F:
		tex.glFmt = GL_RGBA16F;
		break;
		case R_TextureFormats::RGBA32F:
		tex.glFmt = GL_RGBA32F;
		break;
		case R_TextureFormats::DEPTH:
		tex.glFmt = _depthFormat;
		break;
		default:
		fail1();
		break;
	};

	glGenTextures(1, &tex.glObj);
	glActiveTexture(GL_TEXTURE15);
	glBindTexture(tex.type, tex.glObj);

	float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	tex.samplerState = 0;
	applySamplerState(tex);

	glBindTexture(tex.type, 0);
	if (_texSlots[15].texObj)
		glBindTexture(_textures.getRef(_texSlots[15].texObj).type, _textures.getRef(_texSlots[15].texObj).glObj);

	// Calculate memory requirements
	tex.memSize = calcTextureSize(format, width, height, depth);
	if (hasMips || genMips) tex.memSize += ftoi_r(tex.memSize * 1.0f / 3.0f);
	if (type == R_TextureTypes::TexCube) tex.memSize *= 6;
	_textureMem += tex.memSize;

	return _textures.add(tex);
}

void RenderDevice::uploadTextureData(uint32 texObj, int slice, int mipLevel, const void *pixels)
{
	const R_Texture &tex = _textures.getRef(texObj);
	R_TextureFormats::List format = tex.format;

	glActiveTexture(GL_TEXTURE15);
	glBindTexture(tex.type, tex.glObj);

	int inputFormat = GL_RGBA, inputType = GL_UNSIGNED_BYTE;
	bool compressed = (format == R_TextureFormats::DXT1) || (format == R_TextureFormats::DXT3) || (format == R_TextureFormats::DXT5);

	switch (format)
	{
		case R_TextureFormats::RGBA16F:
		inputFormat = GL_RGBA;
		inputType = GL_FLOAT;
		break;

		case R_TextureFormats::RGBA32F:
		inputFormat = GL_RGBA;
		inputType = GL_FLOAT;
		break;

		case R_TextureFormats::DEPTH:
		inputFormat = GL_DEPTH_COMPONENT;
		inputType = GL_FLOAT;
		break;

	};

	// Calculate size of next mipmap using "floor" convention
	int width = std::max(tex.width >> mipLevel, 1), height = std::max(tex.height >> mipLevel, 1);

	if (tex.type == textureTypes[R_TextureTypes::Tex2D] || tex.type == textureTypes[R_TextureTypes::TexCube])
	{
		int target = (tex.type == textureTypes[R_TextureTypes::Tex2D]) ? GL_TEXTURE_2D : (GL_TEXTURE_CUBE_MAP_POSITIVE_X + slice);

		if (compressed)
			glCompressedTexImage2D(target, mipLevel, tex.glFmt, width, height, 0, calcTextureSize(format, width, height, 1), pixels);
		else
			glTexImage2D(target, mipLevel, tex.glFmt, width, height, 0, inputFormat, inputType, pixels);
	}
	else if (tex.type == textureTypes[R_TextureTypes::Tex3D])
	{
		int depth = std::max(tex.depth >> mipLevel, 1);

		if (compressed)
			glCompressedTexImage3D(GL_TEXTURE_3D, mipLevel, tex.glFmt, width, height, depth, 0, calcTextureSize(format, width, height, depth), pixels);
		else
			glTexImage3D(GL_TEXTURE_3D, mipLevel, tex.glFmt, width, height, depth, 0, inputFormat, inputType, pixels);
	}

	if (tex.genMips && (tex.type != GL_TEXTURE_CUBE_MAP || slice == 5))
	{
		glGenerateMipmap(tex.type);
	}

	glBindTexture(tex.type, 0);
	if (_texSlots[15].texObj)
	{
		glBindTexture(_textures.getRef(_texSlots[15].texObj).type, _textures.getRef(_texSlots[15].texObj).glObj);
	}
}

void RenderDevice::destroyTexture(uint32& texObj)
{
	if (texObj == 0)
	{
		return;
	}

	const R_Texture &tex = _textures.getRef(texObj);
	if (tex.glObj)
	{
		glDeleteTextures(1, &tex.glObj);
	}

	_textureMem -= tex.memSize;
	_textures.remove(texObj);
	texObj = 0;
}

bool RenderDevice::getTextureData(uint32 texObj, int slice, int mipLevel, void *buffer)
{
	const R_Texture &tex = _textures.getRef(texObj);

	int target = tex.type == textureTypes[R_TextureTypes::TexCube] ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D;
	if (target == GL_TEXTURE_CUBE_MAP) target = GL_TEXTURE_CUBE_MAP_POSITIVE_X + slice;

	int fmt, type, compressed = 0;
	glActiveTexture(GL_TEXTURE15);
	glBindTexture(tex.type, tex.glObj);

	switch (tex.format)
	{
		case R_TextureFormats::RGBA8:
		fmt = GL_RGBA;
		type = GL_UNSIGNED_BYTE;
		break;
		case R_TextureFormats::DXT1:
		case R_TextureFormats::DXT3:
		case R_TextureFormats::DXT5:
		compressed = 1;
		break;
		case R_TextureFormats::RGBA16F:
		case R_TextureFormats::RGBA32F:
		fmt = GL_RGBA;
		type = GL_FLOAT;
		break;
		default:
		return false;
	};

	if (compressed)
		glGetCompressedTexImage(target, mipLevel, buffer);
	else
		glGetTexImage(target, mipLevel, fmt, type, buffer);

	glBindTexture(tex.type, 0);
	if (_texSlots[15].texObj)
		glBindTexture(_textures.getRef(_texSlots[15].texObj).type, _textures.getRef(_texSlots[15].texObj).glObj);

	return true;
}

void RenderDevice::bindImageToTexture(uint32 texObj, void *eglImage)
{
	if (!_Config.OpenGL.OES_EGL_image)
		Log::Error("OES_egl_image not supported");
	else
	{
		const R_Texture &tex = _textures.getRef(texObj);
		glActiveTexture(GL_TEXTURE15);
		glBindTexture(tex.type, tex.glObj);
		glEGLImageTargetTexture2DOES(tex.type, eglImage);
		checkError();
		glBindTexture(tex.type, 0);
		if (_texSlots[15].texObj)
			glBindTexture(_textures.getRef(_texSlots[15].texObj).type, _textures.getRef(_texSlots[15].texObj).glObj);
	}
}



// =================================================================================================
// Shaders
// =================================================================================================

uint32 RenderDevice::createShader(const char *vertexShaderSrc, const char *fragmentShaderSrc, const char *geometryShaderSrc, const char *tessControlShaderSrc, const char *tessEvaluationShaderSrc, const char *computeShaderSrc)
{
	// Compile and link shader
	uint32 programObj = createShaderProgram(vertexShaderSrc, fragmentShaderSrc, geometryShaderSrc, tessControlShaderSrc, tessEvaluationShaderSrc, computeShaderSrc);
	if (programObj == 0)
	{
		return 0;
	}

	if (!linkShaderProgram(programObj))
	{
		return 0;
	}

	uint32 shaderId = _shaders.add(R_Shader());
	R_Shader& shader = _shaders.getRef(shaderId);
	shader.oglProgramObj = programObj;

	int attribCount;
	glGetProgramiv(programObj, GL_ACTIVE_ATTRIBUTES, &attribCount);

	for (uint32 i = 0; i < _numVertexLayouts; ++i)
	{
		R_VertexLayout &vl = _vertexLayouts[i];
		bool allAttribsFound = true;

		for (uint32 j = 0; j < 16; ++j)
		{
			shader.inputLayouts[i].attribIndices[j] = -1;
		}

		for (int j = 0; j < attribCount; ++j)
		{
			char name[32];
			uint32 size, type;
			glGetActiveAttrib(programObj, j, 32, 0x0, (int *)&size, &type, name);

			bool attribFound = false;
			for (uint32 k = 0; k < vl.numAttribs; ++k)
			{
				if (vl.attribs[k].semanticName.compare(name) == 0)
				{
					shader.inputLayouts[i].attribIndices[k] = glGetAttribLocation(programObj, name);
					attribFound = true;
				}
			}

			if (!attribFound)
			{
				allAttribsFound = false;
				break;
			}
		}

		shader.inputLayouts[i].valid = allAttribsFound;
	}

	return shaderId;
}

void RenderDevice::destroyShader(uint32& shaderId)
{
	if (shaderId == 0)
		return;

	R_Shader &shader = _shaders.getRef(shaderId);
	glDeleteProgram(shader.oglProgramObj);
	_shaders.remove(shaderId);
	shaderId = 0;
}

void RenderDevice::bindShader(uint32 shaderId)
{
	if (shaderId != 0)
	{
		R_Shader& shader = _shaders.getRef(shaderId);
		glUseProgram(shader.oglProgramObj);
	}
	else
	{
		glUseProgram(0);
	}

	_curShaderId = shaderId;
	_pendingMask |= PM_GEOMETRY;
}

int RenderDevice::getShaderConstLoc(uint32 shaderId, const char *name)
{
	R_Shader &shader = _shaders.getRef(shaderId);
	return glGetUniformLocation(shader.oglProgramObj, name);
}

int RenderDevice::getShaderSamplerLoc(uint32 shaderId, const char *name)
{
	R_Shader &shader = _shaders.getRef(shaderId);
	return glGetUniformLocation(shader.oglProgramObj, name);
}

int RenderDevice::getShaderBufferLoc(uint32 shaderId, const char *name)
{
	if (_Config.DeviceCaps.computeShaders)
	{
		R_Shader &shader = _shaders.getRef(shaderId);
		int idx = glGetProgramResourceIndex(shader.oglProgramObj, GL_SHADER_STORAGE_BLOCK, name);
		if (idx != -1)
		{
			const GLenum bufBindingPoint[1] = {GL_BUFFER_BINDING};
			glGetProgramResourceiv(shader.oglProgramObj, GL_SHADER_STORAGE_BLOCK, idx, 1, bufBindingPoint, 1, 0, &idx);
		}
		return idx;
	}
	else
	{
		Log::Error("Shader storage buffers are not supported on this device.");

		return -1;
	}
}

void RenderDevice::setShaderConst(int loc, R_ShaderConstType type, const void *values, uint32 count)
{
	switch (type)
	{
		case CONST_INT:
		glUniform1iv(loc, count, (const int *)values);
		break;
		case CONST_FLOAT:
		glUniform1fv(loc, count, (const float *)values);
		break;
		case CONST_FLOAT2:
		glUniform2fv(loc, count, (const float *)values);
		break;
		case CONST_FLOAT3:
		glUniform3fv(loc, count, (const float *)values);
		break;
		case CONST_FLOAT4:
		glUniform4fv(loc, count, (const float *)values);
		break;
		case CONST_FLOAT44:
		glUniformMatrix4fv(loc, count, false, (const float *)values);
		break;
		case CONST_FLOAT33:
		glUniformMatrix3fv(loc, count, false, (const float *)values);
		break;
	}
}

void RenderDevice::setShaderSampler(int loc, uint32 texUnit)
{
	glUniform1i(loc, (int)texUnit);
}

const char *RenderDevice::getDefaultVSCode()
{
	return defaultShaderVS;
}

const char *RenderDevice::getDefaultFSCode()
{
	return defaultShaderFS;
}

void RenderDevice::runComputeShader(uint32 shaderId, uint32 xDim, uint32 yDim, uint32 zDim)
{
	bindShader(shaderId);

	if (commitStates(~PM_GEOMETRY))
		glDispatchCompute(xDim, yDim, zDim);
}

// Helpers

uint32 RenderDevice::createShaderProgram(const char *vertexShaderSrc, const char *fragmentShaderSrc, const char *geometryShaderSrc, const char *tessControlShaderSrc, const char *tessEvalShaderSrc, const char *computeShaderSrc)
{
	int infologLength = 0;
	int charsWritten = 0;
	char* infoLog = 0x0;
	int status;

	_shaderLog = "";

	uint32 vs, fs, gs, tsC, tsE, cs;
	vs = fs = gs = tsC = tsE = cs = 0;

	// Vertex shader
	if (vertexShaderSrc)
	{
		vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs, 1, &vertexShaderSrc, 0x0);
		glCompileShader(vs);
		glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
		if (!status)
		{
			// Get info
			glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &infologLength);
			if (infologLength > 1)
			{
				infoLog = new char[infologLength];
				glGetShaderInfoLog(vs, infologLength, &charsWritten, infoLog);
				_shaderLog = _shaderLog + "[Vertex Shader]\n" + infoLog;
				delete[] infoLog; infoLog = 0x0;
			}

			glDeleteShader(vs);
			return 0;
		}
	}

	// Fragment shader
	if (fragmentShaderSrc)
	{
		fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs, 1, &fragmentShaderSrc, 0x0);
		glCompileShader(fs);
		glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
		if (!status)
		{
			glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &infologLength);
			if (infologLength > 1)
			{
				infoLog = new char[infologLength];
				glGetShaderInfoLog(fs, infologLength, &charsWritten, infoLog);
				_shaderLog = _shaderLog + "[Fragment Shader]\n" + infoLog;
				delete[] infoLog; infoLog = 0x0;
			}

			glDeleteShader(vs);
			glDeleteShader(fs);
			return 0;
		}
	}

	// Geometry shader
	if (geometryShaderSrc)
	{
		gs = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(gs, 1, &geometryShaderSrc, 0x0);
		glCompileShader(gs);
		glGetShaderiv(gs, GL_COMPILE_STATUS, &status);
		if (!status)
		{
			glGetShaderiv(gs, GL_INFO_LOG_LENGTH, &infologLength);
			if (infologLength > 1)
			{
				infoLog = new char[infologLength];
				glGetShaderInfoLog(gs, infologLength, &charsWritten, infoLog);
				_shaderLog = _shaderLog + "[Geometry Shader]\n" + infoLog;
				delete[] infoLog; infoLog = 0x0;
			}

			glDeleteShader(vs);
			glDeleteShader(fs);
			glDeleteShader(gs);
			return 0;
		}
	}

	// Tesselation control shader
	if (tessControlShaderSrc)
	{
		tsC = glCreateShader(GL_TESS_CONTROL_SHADER);
		glShaderSource(tsC, 1, &tessControlShaderSrc, 0x0);
		glCompileShader(tsC);
		glGetShaderiv(tsC, GL_COMPILE_STATUS, &status);
		if (!status)
		{
			glGetShaderiv(tsC, GL_INFO_LOG_LENGTH, &infologLength);
			if (infologLength > 1)
			{
				infoLog = new char[infologLength];
				glGetShaderInfoLog(tsC, infologLength, &charsWritten, infoLog);
				_shaderLog = _shaderLog + "[Tesselation Control Shader]\n" + infoLog;
				delete[] infoLog; infoLog = 0x0;
			}

			glDeleteShader(vs);
			glDeleteShader(fs);
			if (gs) glDeleteShader(gs);
			glDeleteShader(tsC);
			return 0;
		}
	}

	// Tesselation evaluation shader
	if (tessEvalShaderSrc)
	{
		tsE = glCreateShader(GL_TESS_EVALUATION_SHADER);
		glShaderSource(tsE, 1, &tessEvalShaderSrc, 0x0);
		glCompileShader(tsE);
		glGetShaderiv(tsE, GL_COMPILE_STATUS, &status);
		if (!status)
		{
			glGetShaderiv(tsE, GL_INFO_LOG_LENGTH, &infologLength);
			if (infologLength > 1)
			{
				infoLog = new char[infologLength];
				glGetShaderInfoLog(tsE, infologLength, &charsWritten, infoLog);
				_shaderLog = _shaderLog + "[Tesselation Evaluation Shader]\n" + infoLog;
				delete[] infoLog; infoLog = 0x0;
			}

			glDeleteShader(vs);
			glDeleteShader(fs);
			if (gs) glDeleteShader(gs);
			glDeleteShader(tsC);
			glDeleteShader(tsE);
			return 0;
		}
	}

	// Tesselation evaluation shader
	if (computeShaderSrc)
	{
		cs = glCreateShader(GL_COMPUTE_SHADER);
		glShaderSource(cs, 1, &computeShaderSrc, 0x0);
		glCompileShader(cs);
		glGetShaderiv(cs, GL_COMPILE_STATUS, &status);
		if (!status)
		{
			glGetShaderiv(cs, GL_INFO_LOG_LENGTH, &infologLength);
			if (infologLength > 1)
			{
				infoLog = new char[infologLength];
				glGetShaderInfoLog(cs, infologLength, &charsWritten, infoLog);
				_shaderLog = _shaderLog + "[Compute Shader]\n" + infoLog;
				delete[] infoLog; infoLog = 0x0;
			}

			// other shader types should not be present in compute context, but better check
			if (vs) glDeleteShader(vs);
			if (fs) glDeleteShader(fs);
			if (gs) glDeleteShader(gs);
			if (tsC) glDeleteShader(tsC);
			if (tsE) glDeleteShader(tsE);
			glDeleteShader(cs);
			return 0;
		}
	}

	// Shader program
	uint32 program = glCreateProgram();
	if (vs && fs)
	{
		glAttachShader(program, vs);
		glAttachShader(program, fs);

		glDeleteShader(vs);
		glDeleteShader(fs);
	}

	if (gs)
	{
		glAttachShader(program, gs);
		glDeleteShader(gs);
	}
	if (tsC)
	{
		glAttachShader(program, tsC);
		glDeleteShader(tsC);
	}
	if (tsE)
	{
		glAttachShader(program, tsE);
		glDeleteShader(tsE);
	}
	if (cs)
	{
		glAttachShader(program, cs);
		glDeleteShader(cs);
	}

	return program;
}

bool RenderDevice::linkShaderProgram(uint32 programObj)
{
	int infologLength = 0;
	int charsWritten = 0;
	char *infoLog = 0x0;
	int status;

	_shaderLog = "";

	glLinkProgram(programObj);
	glGetProgramiv(programObj, GL_INFO_LOG_LENGTH, &infologLength);
	if (infologLength > 1)
	{
		infoLog = new char[infologLength];
		glGetProgramInfoLog(programObj, infologLength, &charsWritten, infoLog);
		_shaderLog = _shaderLog + "[Linking]\n" + infoLog;
		delete[] infoLog; infoLog = 0x0;
	}

	glGetProgramiv(programObj, GL_LINK_STATUS, &status);
	if (!status) return false;

	return true;
}



// =================================================================================================
// Renderbuffers
// =================================================================================================

uint32 RenderDevice::createRenderBuffer(uint32 width, uint32 height, R_TextureFormats::List format, bool depth, uint32 numColBufs, uint32 samples)
{
	if ((format == R_TextureFormats::RGBA16F || format == R_TextureFormats::RGBA32F) && !_Config.DeviceCaps.texFloat)
	{
		return 0;
	}

	if (numColBufs > R_RenderBuffer::MaxColorAttachmentCount) return 0;

	uint32 maxSamples = 0;
	if (_Config.DeviceCaps.rtMultisampling)
	{
		GLint value;
		glGetIntegerv(GL_MAX_SAMPLES, &value);
		maxSamples = (uint32)value;
	}
	if (samples > maxSamples)
	{
		samples = maxSamples;
		Log::Warn("GPU does not support desired multisampling quality for render target");
	}

	R_RenderBuffer rb;
	rb.width = width;
	rb.height = height;
	rb.samples = samples;

	// Create framebuffers
	glGenFramebuffers(1, &rb.fbo);
	if (samples > 0)
	{
		glGenFramebuffers(1, &rb.fboMS);
	}

	if (numColBufs > 0)
	{
		// Attach color buffers
		for (uint32 j = 0; j < numColBufs; ++j)
		{
			// Create a color texture
			uint32 texObj = createTexture(R_TextureTypes::Tex2D, rb.width, rb.height, 1, format, false, false, false, false);
			assert1(texObj != 0);
			uploadTextureData(texObj, 0, 0, 0x0);
			rb.colTexs[j] = texObj;
			R_Texture &tex = _textures.getRef(texObj);
			glBindFramebuffer(GL_FRAMEBUFFER, rb.fbo);

			// Attach the texture
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + j, GL_TEXTURE_2D, tex.glObj, 0);

			if (samples > 0)
			{
				glBindFramebuffer(GL_FRAMEBUFFER, rb.fboMS);

				// Create a multisampled renderbuffer
				glGenRenderbuffers(1, &rb.colBufs[j]);
				glBindRenderbuffer(GL_RENDERBUFFER, rb.colBufs[j]);
				glRenderbufferStorageMultisample(GL_RENDERBUFFER, rb.samples, tex.glFmt, rb.width, rb.height);

				// Attach the renderbuffer
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + j, GL_RENDERBUFFER, rb.colBufs[j]);
			}
		}

		uint32 buffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
		glBindFramebuffer(GL_FRAMEBUFFER, rb.fbo);
		glDrawBuffers(numColBufs, buffers);

		if (samples > 0)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, rb.fboMS);
			glDrawBuffers(numColBufs, buffers);
		}
	}
	else
	{
		glBindFramebuffer(GL_FRAMEBUFFER, rb.fbo);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		if (samples > 0)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, rb.fboMS);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}
	}

	// Attach depth buffer
	if (depth)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, rb.fbo);

		// Create a depth texture
		uint32 texObj = createTexture(R_TextureTypes::Tex2D, rb.width, rb.height, 1, R_TextureFormats::DEPTH, false, false, false, false);
		assert1(texObj != 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
		uploadTextureData(texObj, 0, 0, 0x0);
		rb.depthTex = texObj;
		R_Texture &tex = _textures.getRef(texObj);

		// Attach the texture
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex.glObj, 0);

		if (samples > 0)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, rb.fboMS);

			// Create a multisampled renderbuffer
			glGenRenderbuffers(1, &rb.depthBuf);
			glBindRenderbuffer(GL_RENDERBUFFER, rb.depthBuf);
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, rb.samples, _depthFormat, rb.width, rb.height);

			// Attach the renderbuffer
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rb.depthBuf);
		}
	}

	uint32 rbObj = _rendBufs.add(rb);

	// Check if FBO is complete
	bool valid = true;
	glBindFramebuffer(GL_FRAMEBUFFER, rb.fbo);
	uint32 status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	glBindFramebuffer(GL_FRAMEBUFFER, _defaultFBO);
	if (status != GL_FRAMEBUFFER_COMPLETE) valid = false;

	if (samples > 0)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, rb.fboMS);
		status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		glBindFramebuffer(GL_FRAMEBUFFER, _defaultFBO);
		if (status != GL_FRAMEBUFFER_COMPLETE) valid = false;
	}

	if (!valid)
	{
		destroyRenderBuffer(rbObj);
		return 0;
	}

	return rbObj;
}

void RenderDevice::destroyRenderBuffer(uint32& rbObj)
{
	R_RenderBuffer& rb = _rendBufs.getRef(rbObj);

	glBindFramebuffer(GL_FRAMEBUFFER, _defaultFBO);

	if (rb.depthTex != 0)
	{
		destroyTexture(rb.depthTex);
	}
	rb.depthTex = 0;

	if (rb.depthBuf != 0)
	{
		glDeleteRenderbuffers(1, &rb.depthBuf);
	}
	rb.depthBuf = 0;

	for (uint32 i = 0; i < R_RenderBuffer::MaxColorAttachmentCount; ++i)
	{
		if (rb.colTexs[i] != 0)
		{
			destroyTexture(rb.colTexs[i]);
		}
		rb.colTexs[i] = 0;

		if (rb.colBufs[i] != 0)
		{
			glDeleteRenderbuffers(1, &rb.colBufs[i]);
		}
		rb.colBufs[i] = 0;
	}

	if (rb.fbo != 0)
	{
		glDeleteFramebuffers(1, &rb.fbo);
	}
	rb.fbo = 0;

	if (rb.fboMS != 0)
	{
		glDeleteFramebuffers(1, &rb.fboMS);
	}
	rb.fboMS = 0;

	_rendBufs.remove(rbObj);
	rbObj = 0;
}

void RenderDevice::getRenderBufferDimensions(uint32 rbObj, int *width, int *height)
{
	R_RenderBuffer& rb = _rendBufs.getRef(rbObj);

	*width = rb.width;
	*height = rb.height;
}

uint32 RenderDevice::getRenderBufferTex(uint32 rbObj, uint32 bufIndex)
{
	R_RenderBuffer& rb = _rendBufs.getRef(rbObj);

	if (bufIndex < R_RenderBuffer::MaxColorAttachmentCount)
	{
		return rb.colTexs[bufIndex];
	}
	else if (bufIndex == 32)
	{
		return rb.depthTex;
	}
	else
	{
		return 0;
	}
}

void RenderDevice::setRenderBuffer(uint32 rbObj)
{
	// Resolve render buffer if necessary
	if (_curRendBuf != 0)
	{
		resolveRenderBuffer(_curRendBuf);
	}

	// Set new render buffer
	_curRendBuf = rbObj;

	if (rbObj == 0)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _defaultFBO);
		if (_defaultFBO == 0)
		{
			if (_doubleBuffered)
				glDrawBuffer(_outputBufferIndex == 1 ? GL_BACK_RIGHT : GL_BACK_LEFT);
			else
				glDrawBuffer(_outputBufferIndex == 1 ? GL_FRONT_RIGHT : GL_FRONT_LEFT);
		}

		_fbWidth = _vpWidth + _vpX;
		_fbHeight = _vpHeight + _vpY;

		if (_defaultFBOMultisampled)
		{
			glEnable(GL_MULTISAMPLE);
		}
		else
		{
			glDisable(GL_MULTISAMPLE);
		}
	}
	else
	{
		// Unbind all m_DiffuseTextures to make sure that no FBO attachment is bound any more
		for (uint32 i = 0; i < 16; ++i)
		{
			setTexture(i, 0, 0, 0);
		}

		commitStates(PM_TEXTURES);

		R_RenderBuffer& rb = _rendBufs.getRef(rbObj);

		glBindFramebuffer(GL_FRAMEBUFFER, rb.fboMS != 0 ? rb.fboMS : rb.fbo);
		assert1(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
		_fbWidth = rb.width;
		_fbHeight = rb.height;

		if (rb.fboMS != 0)
		{
			glEnable(GL_MULTISAMPLE);
		}
		else
		{
			glDisable(GL_MULTISAMPLE);
		}
	}
}

bool RenderDevice::getRenderBufferData(uint32 rbObj, int bufIndex, int *width, int *height, int *compCount, void *dataBuffer, int bufferSize)
{
	int x, y, w, h;
	int format = GL_RGBA;
	int type = GL_FLOAT;
	beginRendering();
	glPixelStorei(GL_PACK_ALIGNMENT, 4);

	if (rbObj == 0)
	{
		if (bufIndex != 32 && bufIndex != 0) return false;
		if (width != 0x0) *width = _vpWidth;
		if (height != 0x0) *height = _vpHeight;

		x = _vpX; y = _vpY; w = _vpWidth; h = _vpHeight;

		glBindFramebuffer(GL_FRAMEBUFFER, _defaultFBO);
		if (bufIndex != 32)
		{
			if (_doubleBuffered)
				glReadBuffer(_outputBufferIndex == 1 ? GL_BACK_RIGHT : GL_BACK_LEFT);
			else
				glReadBuffer(_outputBufferIndex == 1 ? GL_FRONT_RIGHT : GL_FRONT_LEFT);
		}
	}
	else
	{
		resolveRenderBuffer(rbObj);
		R_RenderBuffer& rb = _rendBufs.getRef(rbObj);

		if (bufIndex == 32 && rb.depthTex == 0) return false;
		if (bufIndex != 32)
		{
			if ((unsigned)bufIndex >= R_RenderBuffer::MaxColorAttachmentCount || rb.colTexs[bufIndex] == 0)
			{
				return false;
			}
		}
		if (width != 0x0) *width = rb.width;
		if (height != 0x0) *height = rb.height;

		x = 0; y = 0; w = rb.width; h = rb.height;

		glBindFramebuffer(GL_FRAMEBUFFER, rb.fbo);
		if (bufIndex != 32)
		{
			glReadBuffer(GL_COLOR_ATTACHMENT0 + bufIndex);
		}
	}

	if (bufIndex == 32)
	{
		format = GL_DEPTH_COMPONENT;
		type = GL_FLOAT;
	}

	int comps = (bufIndex == 32 ? 1 : 4);
	if (compCount != 0x0) *compCount = comps;

	bool retVal = false;
	if (dataBuffer != 0x0 && bufferSize >= w * h * comps * (type == GL_FLOAT ? 4 : 1))
	{
		glFinish();
		glReadPixels(x, y, w, h, format, type, dataBuffer);
		retVal = true;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, _defaultFBO);

	return retVal;
}

// Helpers

void RenderDevice::resolveRenderBuffer(uint32 rbObj)
{
	R_RenderBuffer& rb = _rendBufs.getRef(rbObj);

	if (rb.fboMS == 0)
	{
		return;
	}

	glBindFramebuffer(GL_READ_FRAMEBUFFER, rb.fboMS);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, rb.fbo);

	bool depthResolved = false;
	for (uint32 i = 0; i < R_RenderBuffer::MaxColorAttachmentCount; ++i)
	{
		if (rb.colBufs[i] != 0)
		{
			glReadBuffer(GL_COLOR_ATTACHMENT0 + i);
			glDrawBuffer(GL_COLOR_ATTACHMENT0 + i);

			int mask = GL_COLOR_BUFFER_BIT;
			if (!depthResolved && rb.depthBuf != 0)
			{
				mask |= GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
				depthResolved = true;
			}
			glBlitFramebuffer(0, 0, rb.width, rb.height, 0, 0, rb.width, rb.height, mask, GL_NEAREST);
		}
	}

	if (!depthResolved && rb.depthBuf != 0)
	{
		glReadBuffer(GL_NONE);
		glDrawBuffer(GL_NONE);
		glBlitFramebuffer(0, 0, rb.width, rb.height, 0, 0, rb.width, rb.height, GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
	}

	glBindFramebuffer(GL_READ_FRAMEBUFFER, _defaultFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _defaultFBO);
}



// =================================================================================================
// Queries
// =================================================================================================

uint32 RenderDevice::createOcclusionQuery()
{
	uint32 queryObj;
	glGenQueries(1, &queryObj);
	return queryObj;
}

void RenderDevice::destroyQuery(uint32 queryObj)
{
	if (queryObj == 0) return;

	glDeleteQueries(1, &queryObj);
}

void RenderDevice::beginQuery(uint32 queryObj)
{
	glBeginQuery(GL_SAMPLES_PASSED, queryObj);
}

void RenderDevice::endQuery(uint32 /*queryObj*/)
{
	glEndQuery(GL_SAMPLES_PASSED);
}

uint32 RenderDevice::getQueryResult(uint32 queryObj)
{
	uint32 samples = 0;
	glGetQueryObjectuiv(queryObj, GL_QUERY_RESULT, &samples);
	return samples;
}



// =================================================================================================
// Internal state management
// =================================================================================================

void RenderDevice::checkError()
{
	uint32 error = glGetError();
	assert1(error != GL_INVALID_ENUM);
	assert1(error != GL_INVALID_VALUE);
	assert1(error != GL_INVALID_OPERATION);
	assert1(error != GL_OUT_OF_MEMORY);
	assert1(error != GL_STACK_OVERFLOW && error != GL_STACK_UNDERFLOW);
}

bool RenderDevice::applyVertexLayout(R_GeometryInfo& geo)
{
	uint32 newVertexAttribMask = 0;

	if (_curShaderId == 0)
	{
		return false;
	}

	if (geo.layout == 0)
	{
		return false;
	}

	R_VertexLayout &vl = _vertexLayouts[geo.layout - 1];
	R_Shader &shader = _shaders.getRef(_curShaderId);
	R_InputLayout &inputLayout = shader.inputLayouts[geo.layout - 1];

	if (!inputLayout.valid)
	{
		return false;
	}

	// Set vertex attrib pointers
	for (uint32 i = 0; i < vl.numAttribs; ++i)
	{
		int8 attribIndex = inputLayout.attribIndices[i];
		if (attribIndex >= 0)
		{
			R_VertexLayoutAttrib& attrib = vl.attribs[i];
			const R_VertexBufferSlot& vbSlot = geo.vertexBufInfo[attrib.vbSlot];

			assert1(_buffers.getRef(geo.vertexBufInfo[attrib.vbSlot].vbObj).glObj != 0 && _buffers.getRef(geo.vertexBufInfo[attrib.vbSlot].vbObj).type == GL_ARRAY_BUFFER);

			glBindBuffer(GL_ARRAY_BUFFER, _buffers.getRef(geo.vertexBufInfo[attrib.vbSlot].vbObj).glObj);
			glVertexAttribPointer(attribIndex, attrib.size, GL_FLOAT, GL_FALSE, vbSlot.stride, (char *)0 + vbSlot.offset + attrib.offset);

			newVertexAttribMask |= 1 << attribIndex;
		}
	}


	for (uint32 i = 0; i < 16; ++i)
	{
		uint32 curBit = 1 << i;
		if ((newVertexAttribMask & curBit) != (_activeVertexAttribsMask & curBit))
		{
			if (newVertexAttribMask & curBit)
			{
				glEnableVertexAttribArray(i);
			}
			else
			{
				glDisableVertexAttribArray(i);
			}
		}
	}
	_activeVertexAttribsMask = newVertexAttribMask;

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return true;
}

void RenderDevice::applySamplerState(R_Texture &tex)
{
	uint32 state = tex.samplerState;
	uint32 target = tex.type;

	const uint32 magFilters[] = {GL_LINEAR, GL_LINEAR, GL_NEAREST};
	const uint32 minFiltersMips[] = {GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_LINEAR, GL_NEAREST_MIPMAP_NEAREST};
	const uint32 maxAniso[] = {1, 2, 4, 0, 8, 0, 0, 0, 16};
	const uint32 wrapModes[] = {GL_CLAMP_TO_EDGE, GL_REPEAT, GL_CLAMP_TO_BORDER};

	if (tex.hasMips)
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFiltersMips[(state & SS_FILTER_MASK) >> SS_FILTER_START]);
	else
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, magFilters[(state & SS_FILTER_MASK) >> SS_FILTER_START]);

	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilters[(state & SS_FILTER_MASK) >> SS_FILTER_START]);
	glTexParameteri(target, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso[(state & SS_ANISO_MASK) >> SS_ANISO_START]);
	glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapModes[(state & SS_ADDRU_MASK) >> SS_ADDRU_START]);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapModes[(state & SS_ADDRV_MASK) >> SS_ADDRV_START]);
	glTexParameteri(target, GL_TEXTURE_WRAP_R, wrapModes[(state & SS_ADDRW_MASK) >> SS_ADDRW_START]);

	if (state & SS_COMP_LEQUAL)
	{
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	}
	else
	{
        glTexParameteri(target, GL_TEXTURE_COMPARE_MODE, GL_NONE);
	}
}

void RenderDevice::applyRenderStates()
{
	// Rasterizer state
	if (_newRasterState.hash != _curRasterState.hash)
	{
		// FILL MODE
		if (_newRasterState.fillMode == RS_FILL_SOLID)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		// CULLING
		if (_newRasterState.cullMode == RS_CULL_BACK)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
		}
		else if (_newRasterState.cullMode == RS_CULL_FRONT)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}

		// SCISSOR
		if (!_newRasterState.scissorEnable)
		{
			glDisable(GL_SCISSOR_TEST);
		}
		else
		{
			glEnable(GL_SCISSOR_TEST);
		}

		// MASK
		if (_newRasterState.renderTargetWriteMask)
		{
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		}
		else
		{
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		}

		_curRasterState.hash = _newRasterState.hash;
	}

	// Blend state
	if (_newBlendState.hash != _curBlendState.hash)
	{
		if (!_newBlendState.alphaToCoverageEnable)
		{
			glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
		}
		else
		{
			glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
		}

		if (!_newBlendState.blendEnable)
		{
			glDisable(GL_BLEND);
		}
		else
		{
            glEnable(GL_BLEND);
            glBlendFuncSeparate(oglBlendFuncs[_newBlendState.srcRGBBlendFunc], oglBlendFuncs[_newBlendState.destRGBBlendFunc],
                                oglBlendFuncs[_newBlendState.srcABlendFunc], oglBlendFuncs[_newBlendState.destABlendFunc]);
		}

		_curBlendState.hash = _newBlendState.hash;
	}

	// Depth-stencil state
	if (_newDepthStencilState.hash != _curDepthStencilState.hash)
	{
		if (_newDepthStencilState.depthWriteMask)
		{
			glDepthMask(GL_TRUE);
		}
		else
		{
			glDepthMask(GL_FALSE);
		}

		if (_newDepthStencilState.depthEnable)
		{
			uint32 oglDepthFuncs[8] = {GL_LEQUAL, GL_LESS, GL_EQUAL, GL_GREATER, GL_GEQUAL, GL_ALWAYS, GL_ALWAYS, GL_ALWAYS};

			glEnable(GL_DEPTH_TEST);
			glDepthFunc(oglDepthFuncs[_newDepthStencilState.depthFunc]);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}

		_curDepthStencilState.hash = _newDepthStencilState.hash;
	}

	// Number of vertices in patch. Used in tesselation.
	if (_tessPatchVerts != _lastTessPatchVertsValue)
	{
		glPatchParameteri(GL_PATCH_VERTICES, _tessPatchVerts);

		_lastTessPatchVertsValue = _tessPatchVerts;
	}
}



// =================================================================================================
// Public state management
// =================================================================================================

bool RenderDevice::commitStates(uint32 filter)
{
	if (_pendingMask & filter)
	{
		uint32 mask = _pendingMask & filter;

		// Set viewport
		if (mask & PM_VIEWPORT)
		{
			glViewport(_vpX, _vpY, _vpWidth, _vpHeight);
			_pendingMask &= ~PM_VIEWPORT;
		}

		CHECK_GL_ERROR

		// Update render state
		if (mask & PM_RENDERSTATES)
		{
			applyRenderStates();
			_pendingMask &= ~PM_RENDERSTATES;
		}

		CHECK_GL_ERROR

		// Set scissor rect
		if (mask & PM_SCISSOR)
		{
			glScissor(_scX, _scY, _scWidth, _scHeight);
			_pendingMask &= ~PM_SCISSOR;
		}

		CHECK_GL_ERROR

		// Bind m_DiffuseTextures and set sampler state
		if (mask & PM_TEXTURES)
		{
			for (uint32 i = 0; i < 16; ++i)
			{
				glActiveTexture(GL_TEXTURE0 + i);

				if (_texSlots[i].usage != R_TextureUsage::Texture && _texSlots[i].texObj != 0)
				{
					if (i >= MaxComputeImages)
					{
						continue;
					}

					R_Texture &tex = _textures.getRef(_texSlots[i].texObj);
					uint32 access[3] = {GL_READ_ONLY, GL_WRITE_ONLY, GL_READ_WRITE};

					glBindImageTexture(i, tex.glObj, 0, false, 0, access[_texSlots[i].usage - 1], tex.glFmt);
					glBindTexture(GL_TEXTURE_CUBE_MAP, 0); // as image units are different from texture units - clear binded texture units
					glBindTexture(GL_TEXTURE_3D, 0);
					glBindTexture(GL_TEXTURE_2D, 0);
				}
				else if (_texSlots[i].texObj != 0)
				{
					R_Texture &tex = _textures.getRef(_texSlots[i].texObj);
					glBindTexture(tex.type, tex.glObj);

					// Apply sampler state
					if (tex.samplerState != _texSlots[i].samplerState)
					{
						tex.samplerState = _texSlots[i].samplerState;
						applySamplerState(tex);
					}
				}
				else
				{
					glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
					glBindTexture(GL_TEXTURE_3D, 0);
					glBindTexture(GL_TEXTURE_2D, 0);
				}
			}

			_pendingMask &= ~PM_TEXTURES;
		}

		CHECK_GL_ERROR

		// Bind vertex buffers
		if (mask & PM_GEOMETRY)
		{
			R_GeometryInfo &geo = _vaos.getRef(_curGeometryIndex);

			glBindVertexArray(geo.vao);

			m_IsIndexFormat32 = geo.indexBuf32Bit;
			_pendingMask &= ~PM_GEOMETRY;
		}

		CHECK_GL_ERROR

		// Place memory barriers
		if (mask & PM_BARRIER)
		{
			if (_memBarriers != NotSet)
			{
				glMemoryBarrier(memoryBarrierType[(uint32)_memBarriers - 1]);
			}
			_pendingMask &= ~PM_BARRIER;
		}

		CHECK_GL_ERROR

		// Bind storage buffers
		if (mask & PM_COMPUTE)
		{
			for (size_t i = 0; i < _storageBufs.size(); ++i)
			{
				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, _storageBufs[i].slot, _storageBufs[i].oglObject);
			}

			_pendingMask &= ~PM_COMPUTE;
		}

		CHECK_GL_ERROR
	}

	return true;
}

void RenderDevice::resetStates()
{
	CHECK_GL_ERROR

	_curGeometryIndex = 1;
	_curRasterState.hash = 0xFFFFFFFF; _newRasterState.hash = 0;
	_curBlendState.hash = 0xFFFFFFFF; _newBlendState.hash = 0;
	_curDepthStencilState.hash = 0xFFFFFFFF; _newDepthStencilState.hash = 0;

	_memBarriers = NotSet;

	for (uint32 i = 0; i < 16; ++i)
	{
		setTexture(i, 0, 0, 0);
	}

	_storageBufs.clear();

	setColorWriteMask(true);
	_pendingMask = 0xFFFFFFFF;
	commitStates();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glBindFramebuffer(GL_FRAMEBUFFER, _defaultFBO);
}



// =================================================================================================
// Draw calls and clears
// =================================================================================================

void RenderDevice::clear(uint32 flags, float *colorRGBA, float depth)
{
	uint32 prevBuffers[4] = {0};

	if (_curRendBuf != 0x0)
	{
		R_RenderBuffer& rb = _rendBufs.getRef(_curRendBuf);

		if ((flags & CLR_DEPTH) && rb.depthTex == 0)
		{
			flags &= ~CLR_DEPTH;
		}

		// Store state of glDrawBuffers
		for (uint32 i = 0; i < 4; ++i)
		{
			glGetIntegerv(GL_DRAW_BUFFER0 + i, (int *)&prevBuffers[i]);
		}

		uint32 buffers[4], cnt = 0;

		if ((flags & CLR_COLOR_RT0) && rb.colTexs[0] != 0) buffers[cnt++] = GL_COLOR_ATTACHMENT0;
		if ((flags & CLR_COLOR_RT1) && rb.colTexs[1] != 0) buffers[cnt++] = GL_COLOR_ATTACHMENT1;
		if ((flags & CLR_COLOR_RT2) && rb.colTexs[2] != 0) buffers[cnt++] = GL_COLOR_ATTACHMENT2;
		if ((flags & CLR_COLOR_RT3) && rb.colTexs[3] != 0) buffers[cnt++] = GL_COLOR_ATTACHMENT3;

		if (cnt == 0)
		{
			flags &= ~(CLR_COLOR_RT0 | CLR_COLOR_RT1 | CLR_COLOR_RT2 | CLR_COLOR_RT3);
		}
		else
		{
			glDrawBuffers(cnt, buffers);
		}
	}

	uint32 oglClearMask = 0;

	if (flags & CLR_DEPTH)
	{
		oglClearMask |= GL_DEPTH_BUFFER_BIT;
		glClearDepth(depth);
	}
	if (flags & (CLR_COLOR_RT0 | CLR_COLOR_RT1 | CLR_COLOR_RT2 | CLR_COLOR_RT3))
	{
		oglClearMask |= GL_COLOR_BUFFER_BIT;
		if (colorRGBA)
			glClearColor(colorRGBA[0], colorRGBA[1], colorRGBA[2], colorRGBA[3]);
		else
			glClearColor(0, 0, 0, 0);
	}

	if (oglClearMask)
	{
		commitStates(PM_VIEWPORT | PM_SCISSOR | PM_RENDERSTATES);
		glClear(oglClearMask);
	}

	// Restore state of glDrawBuffers
	if (_curRendBuf != 0x0)
	{
		glDrawBuffers(4, prevBuffers);
	}

	CHECK_GL_ERROR
}

void RenderDevice::draw(R_PrimitiveType primType, uint32 firstVert, uint32 numVerts)
{
	if (commitStates())
	{
		assert1(_curGeometryIndex > 1);
		glDrawArrays(primitiveTypes[(uint32)primType], firstVert, numVerts);

		CHECK_GL_ERROR
	}
}

void RenderDevice::drawIndexed(R_PrimitiveType primType, uint32 firstIndex, uint32 numIndices, uint32 firstVert, uint32 numVerts, bool _softReset)
{
	if (commitStates())
	{
		firstIndex *= (m_IsIndexFormat32) ? 4u : 2u;

		assert1(_curGeometryIndex > 1);
		glDrawRangeElements(primitiveTypes[(uint32)primType], firstVert, firstVert + numVerts, numIndices, indexFormats[m_IsIndexFormat32], (char *)0 + firstIndex);
	
		if (_softReset)
		{
			glBindVertexArray(0);
		}

		CHECK_GL_ERROR
	}
}
