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

	const aiNode* child = root->mChildren[root->mNumChildren - 1];

	aiMesh* mesh = scene->mMeshes[child->mMeshes[child->mNumMeshes - 1]];

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;


	for (uint32_t i = 0; i < mesh->mNumVertices; i++)
	{
		// Vertices
		glm::vec3 v;
		v.x = mesh->mVertices[i].x;
		v.y = mesh->mVertices[i].z;
		v.z = mesh->mVertices[i].y;
		vertices.push_back(v);
		// Normals
		glm::vec3 vn;
		vn.x = mesh->mNormals[i].x;
		vn.y = mesh->mNormals[i].z;
		vn.z = mesh->mNormals[i].y;
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
			indexes.push_back(face.mIndices[j]);
		}
	}

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

void Mesh::render(ProgramWrapperMain& prog)
{
	prog.bindAttribute();
	
	vBuf.bind();
	prog.verticesPointer();
	vBuf.unbind();

	tBuf.bind();
	prog.texCoordsPointer();
	tBuf.unbind();

	nBuf.bind();
	prog.normalsPointer();
	nBuf.unbind();

	iBuf.bind();
	prog.drawElements(GL_TRIANGLES, iBuf.size());
	iBuf.unbind();

	prog.unbindAttribute();
}

void Mesh::release()
{
	iBuf.release();
	nBuf.release();
	tBuf.release();
	vBuf.release();
}
