#include "../stdafx.h"

// General
#include "MDX.h"

void MDX::initStatic(File& f)
{
    m_OriginalVertexes = (M2Vertex*)(f.GetData() + header.vertices.offset);

    /*m_OriginalVertexes = new M2Vertex[header.vertices.size];
    memcpy(m_OriginalVertexes, (f.GetData() + header.vertices.offset), sizeof(M2Vertex) * header.vertices.size);*/

	initCommon(f);

	// Create buffer



	//delete[] m_Vertices;
	//delete[] m_Texcoords;
	//delete[] m_Normals;

	//if (colors) delete[] colors;
	//if (transparency) delete[] transparency;
}