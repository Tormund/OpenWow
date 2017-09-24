#pragma once

// ASSIMP
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/DefaultLogger.hpp>
//#pragma comment(lib, "assimp.lib")

struct Vertex
{
	vec3 m_pos;
	vec2 m_tex;
	vec3 m_normal;

	Vertex() {}

	Vertex(const vec3& pos, cvec2 tex, const vec3& normal)
	{
		m_pos = pos;
		m_tex = tex;
		m_normal = normal;
	}
};

struct AssimpMesh
{
	AssimpMesh();
	~AssimpMesh();

	bool Init(const vector<Vertex>& Vertices, const vector<unsigned>& Indices);

	GLuint vertexBuffer;
	GLuint indexBuffer;

	unsigned int NumIndices;
	unsigned int MaterialIndex;
};

struct Material
{
	Texture* textures[12];
};

class AssimpModel
{
public:
	AssimpModel() {};
	~AssimpModel()
	{
		Clear();
	};
	bool LoadMesh(const File& _file);
	void Render();

private:
	bool InitFromScene(const aiScene* pScene, const File& _file);
	AssimpMesh* InitMesh(const aiMesh* paiMesh);
	bool InitMaterials(const aiScene* pScene, const File& _file);
	void Clear();

#define INVALID_MATERIAL 0xFFFFFFFF

	vector<AssimpMesh*> meshes;
	vector<Material*> materials;
};