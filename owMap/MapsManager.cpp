#include "stdafx.h"

// General
#include "MapsManager.h"

// 8x8x2 version with triangle strips, size = 8*18 + 7*2
template <class V>
void stripify(V *in, V *out)
{
	for (int row = 0; row < 8; row++)
	{
		V *thisrow = &in[indexMapBuf(0, row * 2)];
		V *nextrow = &in[indexMapBuf(0, (row + 1) * 2)];

		if (row > 0)
			*out++ = thisrow[0];

		for (int col = 0; col < 9; col++)
		{
			*out++ = thisrow[col];
			*out++ = nextrow[col];
		}

		if (row < 7)
			*out++ = nextrow[8];
	}
}

// high res version, size = 16*18 + 7*2 + 8*2
template <class V>
void stripify2(V *in, V *out)
{
	for (int row = 0; row < 8; row++)
	{
		V *thisrow = &in[indexMapBuf(0, row * 2)];
		V *nextrow = &in[indexMapBuf(0, row * 2 + 1)];
		V *overrow = &in[indexMapBuf(0, (row + 1) * 2)];

		if (row > 0) *out++ = thisrow[0];// jump end
		for (int col = 0; col < 8; col++)
		{
			*out++ = thisrow[col];
			*out++ = nextrow[col];
		}
		*out++ = thisrow[8];
		*out++ = overrow[8];
		*out++ = overrow[8];// jump start
		*out++ = thisrow[0];// jump end
		*out++ = thisrow[0];
		for (int col = 0; col < 8; col++)
		{
			*out++ = overrow[col];
			*out++ = nextrow[col];
		}
		if (row < 8) *out++ = overrow[8];
		if (row < 7) *out++ = overrow[8];// jump start
	}
}

MapsManager::MapsManager()
{
	// default strip indices
	short *defstrip = new short[stripsize];
	for (int i = 0; i < stripsize; i++)
		defstrip[i] = i; // note: this is ugly and should be handled in stripify

	mapstrip = new short[stripsize];
	stripify<short>(defstrip, mapstrip);
	delete[] defstrip;

	//

	defstrip = new short[stripsize2];
	for (int i = 0; i < stripsize2; i++)
		defstrip[i] = i; // note: this is ugly and should be handled in stripify
	mapstrip2 = new short[stripsize2];
	stripify2<short>(defstrip, mapstrip2);
	delete[] defstrip;


	//CreateTextureBuffers();
}

MapsManager::~MapsManager()
{

	if (mapstrip)
		delete[] mapstrip;

	if (mapstrip2)
		delete[] mapstrip2;
}

void MapsManager::CreateTextureBuffers()
{
	//GLuint detailtexcoords;
	vec2* vt;
	float tx, ty;

	// init texture coordinates for detail map:
	vt = dataDetail;
	const float detail_half = 0.5f * detail_size / 8.0f;
	for (int j = 0; j < 17; j++)
	{
		for (int i = 0; i < ((j % 2) ? 8 : 9); i++)
		{
			tx = detail_size / 8.0f * i;
			ty = detail_size / 8.0f * j * 0.5f;
			if (j % 2)
			{
				// offset by half
				tx += detail_half;
			}
			*vt++ = vec2(tx, ty);
		}
	}

	glGenBuffersARB(1, &detailtexcoords);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, detailtexcoords);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, C_MapBufferSize * 2 * sizeof(float), dataDetail, GL_STATIC_DRAW_ARB);

	//

	// init texture coordinates for alpha map:
	vt = dataAlpha;
	const float alpha_half = 0.5f * 1.0f / 8.0f;
	for (int j = 0; j < 17; j++)
	{
		for (int i = 0; i < ((j % 2) ? 8 : 9); i++)
		{
			tx = 1.0f / 8.0f * i;
			ty = 1.0f / 8.0f * j * 0.5f;
			if (j % 2)
			{
				// offset by half
				tx += alpha_half;
			}
			//*vt++ = vec2(tx*0.95f, ty*0.95f);
			const int divs = 32;
			const float inv = 1.0f / divs;
			const float mul = (divs - 1.0f);
			*vt++ = vec2(tx*(mul*inv), ty*(mul*inv));
		}
	}


	//GLuint alphatexcoords;
	glGenBuffersARB(1, &alphatexcoords);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, alphatexcoords);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, C_MapBufferSize * 2 * sizeof(float), dataAlpha, GL_STATIC_DRAW_ARB);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
}

void MapsManager::ActivateTextureBuffers()
{
	glClientActiveTextureARB(GL_TEXTURE0_ARB);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, detailtexcoords);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);

	glClientActiveTextureARB(GL_TEXTURE1_ARB);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, alphatexcoords);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);
}
