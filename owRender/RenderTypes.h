#pragma once

// ---------------------------------------------------------
// Vertex layout
// ---------------------------------------------------------

struct R_VertexLayoutAttrib
{
	string  semanticName;
	uint32  vbSlot;
	uint32  size;
	uint32  offset;
};

struct R_VertexLayout
{
	uint32              numAttribs;
	R_VertexLayoutAttrib  attribs[16];
};


// ---------------------------------------------------------
// Buffers
// ---------------------------------------------------------

struct R_Buffer
{
	R_Buffer() :
		type(0),
		glObj(0),
		size(0),
		geometryRefCount(0)
	{}

	//

	uint32  type;
	uint32  glObj;
	uint32  size;
	int		geometryRefCount;
};

struct R_VertexBufferSlot
{
	R_VertexBufferSlot() :
		vbObj(0),
		offset(0),
		stride(0)
	{}

	R_VertexBufferSlot(uint32 vbObj, uint32 offset, uint32 stride) :
		vbObj(vbObj),
		offset(offset),
		stride(stride)
	{}

	//

	uint32  vbObj;
	uint32  offset;
	uint32  stride;
};

struct R_GeometryInfo
{
	R_GeometryInfo() :
		vao(0),
		indexBuf(0),
		layout(0),
		indexBuf32Bit(false),
		atrribsBinded(false)
	{}

	//

	vector< R_VertexBufferSlot > vertexBufInfo;
	uint32 vao;
	uint32 indexBuf;
	uint32 layout;
	bool indexBuf32Bit;
	bool atrribsBinded;
};

struct R_ShaderStorage
{
	R_ShaderStorage(uint8 targetSlot, uint32 glObj) :
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

struct R_Texture
{
	R_Texture() :
		glObj(0),
		glFmt(0),
		type(0),
		format(R_TextureFormats::Unknown),
		width(0),
		height(0),
		depth(0),
		memSize(0),
		samplerState(0),
		sRGB(false),
		hasMips(false),
		genMips(false)
	{}

	//

	uint32                  glObj;
	uint32                  glFmt;
	int                     type;
	R_TextureFormats::List  format;
	int                     width, height, depth;
	int                     memSize;
	uint32                  samplerState;
	bool                    sRGB;
	bool                    hasMips, genMips;
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

struct R_TextureBuffer
{
	R_TextureBuffer() :
		bufObj(0),
		glFmt(0),
		glTexID(0)
	{}

	//

	uint32  bufObj;
	uint32  glFmt;
	uint32	glTexID;
};



// ---------------------------------------------------------
// Shaders
// ---------------------------------------------------------

struct R_InputLayout
{
	R_InputLayout() : valid(false)
	{
		memset(attribIndices, 0, sizeof(attribIndices));
	}

	//

	bool  valid;
	int8  attribIndices[16];
};

struct R_Shader
{
	R_Shader() :
		oglProgramObj(0)
	{}

	//

	uint32			oglProgramObj;
	R_InputLayout	inputLayouts[MaxNumVertexLayouts];
};



// ---------------------------------------------------------
// Render buffers
// ---------------------------------------------------------

struct R_RenderBuffer
{
	static const uint32 MaxColorAttachmentCount = 4;

	//

	R_RenderBuffer() :
		fbo(0),
		fboMS(0),
		width(0),
		height(0),
		depthTex(0),
		depthBuf(0),
		samples(0)
	{
		for (uint32 i = 0; i < MaxColorAttachmentCount; ++i)
		{
			colTexs[i] = colBufs[i] = 0;
		}
	}

	//

	uint32  fbo, fboMS;
	uint32  width, height;
	uint32  samples;

	uint32  depthTex, colTexs[MaxColorAttachmentCount];
	uint32  depthBuf, colBufs[MaxColorAttachmentCount];  // Used for multisampling
};