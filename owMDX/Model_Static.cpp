#include "stdafx.h"

// General
#include "MDX.h"

void MDX::initStatic(File& f)
{
	m_OriginalVertexes = (M2Vertex*)(f.GetData() + header.vertices.offset);

	initCommon(f);

	// Create buffer



	//delete[] m_Vertices;
	//delete[] m_Texcoords;
	//delete[] m_Normals;

	//if (colors) delete[] colors;
	//if (transparency) delete[] transparency;
}