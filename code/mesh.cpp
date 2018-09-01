#include "sys.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


void Mesh::init(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path.c_str(), 
		aiProcess_Triangulate | 
		aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	const aiNode* root = scene->mRootNode;

	uint32_t numMesh = root->mNumMeshes;
	uint32_t numChild = root->mNumChildren;

	std::cout << numMesh << std::endl;
	std::cout << numChild << std::endl;

	const aiNode* child = root->mChildren[root->mNumChildren - 1];

	std::cout << child->mNumMeshes << std::endl;
	std::cout << child->mNumChildren << std::endl;

	aiMesh* mesh = scene->mMeshes[child->mMeshes[child->mNumMeshes - 1]];

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;


	for (uint32_t i = 0; i < mesh->mNumVertices; i++)
	{
		// Vertices
		glm::vec3 v;
		v.x = mesh->mVertices[i].x;
		v.y = mesh->mVertices[i].y;
		v.z = mesh->mVertices[i].z;
		vertices.push_back(v);
		// Normals
		glm::vec3 vn;
		vn.x = mesh->mNormals[i].x;
		vn.y = mesh->mNormals[i].y;
		vn.z = mesh->mNormals[i].z;
		normals.push_back(vn);
		// TexCoords
		glm::vec2 vt;
		vt.x = mesh->mTextureCoords[0][i].x;
		vt.y = mesh->mTextureCoords[0][i].y;
		texCoords.push_back(vt);
	}


	std::vector<uint32_t> indexes;

	for (uint32_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (uint32_t j = 0; j < face.mNumIndices; j++)
		{
			indexes.push_back(face.mIndices[i]);
		}
	}

	std::cout << vertices.size() << ", " << normals.size() << ", " << texCoords.size() << std::endl;
	std::cout << indexes.size() << std::endl;

	// Vertex Buffer
	vBuf.addAll(vertices);
	vBuf.create();
	vBuf.upload();

	// TexCoord Buffer
	tBuf.addAll(texCoords);
	tBuf.create();
	tBuf.upload();

	// Normal Buffer
	nBuf.addAll(normals);
	nBuf.create();
	nBuf.upload();

	// Index Buffer
	iBuf.addAll(indexes);
	iBuf.create();
	iBuf.upload();


}

void Mesh::render(Program& prog)
{
	prog.getAttr()->bind();
	
	vBuf.bind();
	prog.getAttr()->pointer("vertices", 3, GL_FLOAT);
	vBuf.unbind();

	tBuf.bind();
	prog.getAttr()->pointer("texCoords", 2, GL_FLOAT);
	tBuf.unbind();

	nBuf.bind();
	prog.getAttr()->pointer("normals", 3, GL_FLOAT);
	nBuf.unbind();

	iBuf.bind();
	glDrawElements(GL_TRIANGLES, iBuf.size(), GL_UNSIGNED_INT, 0);
	iBuf.unbind();

	prog.getAttr()->unbind();
}

void Mesh::release()
{
	iBuf.release();
	nBuf.release();
	tBuf.release();
	vBuf.release();
}
