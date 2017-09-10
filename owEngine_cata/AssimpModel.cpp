#include "stdafx.h"



// General
#include "AssimpModel.h"

void createAILogger()
{
	Assimp::Logger::LogSeverity severity = Assimp::Logger::VERBOSE;
	Assimp::DefaultLogger::create("", severity, aiDefaultLogStream_STDOUT);
	Assimp::DefaultLogger::create("assimp_log.txt", severity, aiDefaultLogStream_FILE);
	Assimp::DefaultLogger::get()->info("this is my info-call");
}

AssimpMesh::AssimpMesh()
{
	vertexBuffer = INVALID_OGL_VALUE;
	indexBuffer = INVALID_OGL_VALUE;
	NumIndices = 0;
	MaterialIndex = INVALID_MATERIAL;
}

AssimpMesh::~AssimpMesh()
{
	if (vertexBuffer != INVALID_OGL_VALUE)
	{
		glDeleteBuffers(1, &vertexBuffer);
	}

	if (indexBuffer != INVALID_OGL_VALUE)
	{
		glDeleteBuffers(1, &indexBuffer);
	}
}

bool AssimpMesh::Init(const vector<Vertex>& Vertices, const vector<unsigned int>& Indices)
{
	NumIndices = Indices.size();

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, 8 * 4 * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * NumIndices, &Indices[0], GL_STATIC_DRAW);

	return true;
}

void AssimpMesh::Render()
{

}

// ---------------------------------------------------------------------------------------------------------

void AssimpModel::Clear()
{
	/*for(unsigned int i = 0; i < m_Textures.size(); i++) {
		SAFE_DELETE(m_Textures[i]);
	}*/
}

bool AssimpModel::LoadMesh(const File& _file)
{
	Clear();
	createAILogger();
	bool Ret = false;

	File f = _file;

	if (!f.Open())
	{
		Debug::Error("Model[%s]: Error opening file.", _file.Path_Name().c_str());
		return false;
	}

	Assimp::Importer Importer;

	const aiScene* pScene = Importer.ReadFile(LocalFile::gamedata + _file.Path_Name(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

	if (pScene)
	{
		Ret = InitFromScene(pScene, _file);
	}
	else
	{
		Debug::Error("Error parsing '%s': '%s'", _file.Path_Name().c_str(), Importer.GetErrorString());
	}

	return Ret;
}

bool AssimpModel::InitFromScene(const aiScene* pScene, const File& _file)
{
	meshes.resize(pScene->mNumMeshes);
	materials.resize(pScene->mNumMaterials);

	Debug::Info("Mesh cnt = %d", pScene->mNumMeshes);

	for (unsigned int i = 0; i < pScene->mNumMeshes; i++)
	{
		const aiMesh* paiMesh = pScene->mMeshes[i];
		meshes[i] = InitMesh(paiMesh);
	}

	InitMaterials(pScene, _file);

	return true;
}

AssimpMesh* AssimpModel::InitMesh(const aiMesh* paiMesh)
{
	AssimpMesh* mesh = new AssimpMesh;
	mesh->MaterialIndex = paiMesh->mMaterialIndex;

	vector<Vertex> Vertices;
	vector<unsigned> Indices;

	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++)
	{
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;
		const aiVector3D* pNormal = &(paiMesh->mNormals[i]);

		Vertex v(
			vec3(pPos->x, pPos->y, pPos->z),
			vec2(pTexCoord->x, pTexCoord->y),
			vec3(pNormal->x, pNormal->y, pNormal->z)
		);
		Vertices.push_back(v);
	}

	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++)
	{
		const aiFace& Face = paiMesh->mFaces[i];
		assert(Face.mNumIndices == 3);
		Indices.push_back(Face.mIndices[0]);
		Indices.push_back(Face.mIndices[1]);
		Indices.push_back(Face.mIndices[2]);
	}

	mesh->Init(Vertices, Indices);

	return mesh;
}

bool AssimpModel::InitMaterials(const aiScene* pScene, const File& _file)
{
	bool Ret = true;

	Debug::Info("Test [%s]", (_file.Path()).c_str());

	// »нициализируем материал
	for (unsigned int i = 0; i < pScene->mNumMaterials; i++)
	{
		const aiMaterial* pMaterial = pScene->mMaterials[i];

		materials[i] = new Material;

		//if(pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {

		for (uint8_t type = aiTextureType_NONE; type < aiTextureType_UNKNOWN; type++)
		{
			aiString Path;
			if (pMaterial->GetTexture((aiTextureType)type, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
			{
				Debug::Info("TYPE %d exists!!!! [%s]", type, Path.data);
				materials[i]->textures[type] = _TexturesMgr->Add(_file.Path() + Path.data);
				Debug::Info("Texture [%s] loaded.", (_file.Path() + Path.data).c_str());
			}
			else
			{
				materials[i]->textures[type] = nullptr;
			}
		}


		/*pMaterial->GetTexture()

		aiTextureType_DIFFUSE = 0x1,

			aiTextureType_SPECULAR = 0x2,

			aiTextureType_AMBIENT = 0x3,

			aiTextureType_EMISSIVE = 0x4,
			aiTextureType_HEIGHT = 0x5,
			aiTextureType_NORMALS = 0x6,
			aiTextureType_SHININESS = 0x7,
			aiTextureType_OPACITY = 0x8,
			aiTextureType_DISPLACEMENT = 0x9,
			aiTextureType_LIGHTMAP = 0xA,
			aiTextureType_REFLECTION = 0xB,*/
			//}

			// «агружаем белую текстуру если модель не имеет собственной
		if (!materials[i])
		{
			//m_Textures[i] = new Texture(GL_TEXTURE_2D, "../Content/white.png");
			//Ret = m_Textures[i]->Load();
		}
	}

	return Ret;
}

void AssimpModel::Render()
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, meshes[i]->vertexBuffer);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(3 * 4));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)((3 + 2) * 4));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes[i]->indexBuffer);

		const unsigned MaterialIndex = meshes[i]->MaterialIndex;

		if (MaterialIndex < materials.size() && materials[MaterialIndex])
		{
			// Diffuse texture
			if (materials[MaterialIndex]->textures[aiTextureType_DIFFUSE] != nullptr)
			{
				materials[MaterialIndex]->textures[aiTextureType_DIFFUSE]->Bind();
			}
			else
			{
				_TexturesMgr->White()->Bind();
			}

			// Specular texture
			if (materials[MaterialIndex]->textures[aiTextureType_SHININESS] != nullptr)
			{
				materials[MaterialIndex]->textures[aiTextureType_SHININESS]->Bind(5);
			}
			else
			{
				_TexturesMgr->White()->Bind(5);
			}
		}

		glDrawElements(GL_TRIANGLES, meshes[i]->NumIndices, GL_UNSIGNED_INT, 0);

		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}



}