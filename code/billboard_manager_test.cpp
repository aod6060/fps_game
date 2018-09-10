#include "sys.h"


void BillboardManagerTest::init()
{
	// Shaders
	vertex.create(GL_VERTEX_SHADER, "data/shaders/billboard_test.vert");
	fragment.create(GL_FRAGMENT_SHADER, "data/shaders/billboard_test.frag");
	
	// Program
	program.addShader(&vertex);
	program.addShader(&fragment);
	program.create();
	
	program.bind();

	// Attribute
	program.getAttr()->set("vertices", 0);
	program.getAttr()->set("texCoords", 1);
	program.getAttr()->set("model", 2);
	program.getAttr()->bind();
	program.getAttr()->enable("vertices");
	program.getAttr()->enable("texCoords");
	program.getAttr()->enable("model", 0);
	program.getAttr()->enable("model", 1);
	program.getAttr()->enable("model", 2);
	program.getAttr()->enable("model", 3);
	program.getAttr()->unbind();
	program.getAttr()->disable("vertices");
	program.getAttr()->disable("texCoords");
	program.getAttr()->disable("model", 0);
	program.getAttr()->disable("model", 1);
	program.getAttr()->disable("model", 2);
	program.getAttr()->disable("model", 3);
	// Uniforms
	program.getUniforms()->create("proj");
	program.getUniforms()->create("view");
	program.getUniforms()->create("billboard");
	program.getUniforms()->set1i("billboard", 0);
	program.unbind();

	// Texture
	tree.init("data/textures/test_tree.png");
	tree2.init("data/textures/test_tree2.png");
	tree3.init("data/textures/test_tree3.png");
	tree4.init("data/textures/test_tree4.png");

	// Geometry
	// Vertex Buffer
	vBuf.add(-0.5f, 1.0f, 0.0f);
	vBuf.add(0.5f, 1.0f, 0.0f);
	vBuf.add(-0.5f, 0.0f, 0.0f);
	vBuf.add(0.5f, 0.0f, 0.0f);
	vBuf.create();
	vBuf.upload();
	
	// TexCoord Buffer
	tBuf.add(0.0f, 0.0f);
	tBuf.add(1.0f, 0.0f);
	tBuf.add(0.0f, 1.0f);
	tBuf.add(1.0f, 1.0f);
	tBuf.create();
	tBuf.upload();

	// Index Buffer
	iBuf.add(0, 1, 2);
	iBuf.add(2, 1, 3);
	iBuf.create();
	iBuf.upload();

	// Matrix Buffer
	matrixBuffer.create();
}

void BillboardManagerTest::release()
{
	matrixBuffer.release();
	iBuf.release();
	tBuf.release();
	vBuf.release();
	tree4.release();
	tree3.release();
	tree2.release();
	tree.release();
	program.release();
	fragment.release();
	vertex.release();
}

void BillboardManagerTest::addBillboard(
	glm::vec3 pos,
	glm::vec2 scale,
	uint32_t tex)
{
	switch (tex)
	{
	case 0:
		this->tree_b.push_back({ pos, scale });
		break;
	case 1:
		this->tree_b2.push_back({ pos, scale });
		break;
	case 2:
		this->tree_b3.push_back({ pos, scale });
		break;
	case 3:
		this->tree_b4.push_back({ pos, scale });
		break;
	}
}

static float _toAngle(const glm::vec3& a, const glm::vec3& b)
{
	float la = glm::length(a);
	float lb = glm::length(b);

	float d = glm::dot(a, b);

	return glm::acos(d / (la * lb));

}

void BillboardManagerTest::render(
	const glm::mat4& proj,
	const glm::mat4& view,
	const glm::vec3& cameraPos)
{

	std::vector<glm::mat4> models;

	this->program.bind();

	this->program.getUniforms()->setMat4("proj", proj);
	this->program.getUniforms()->setMat4("view", view);

	// Draw Tree 1
	this->convertBillboardToMatrix(models, this->tree_b, cameraPos);
	this->matrixBuffer.addAll(models);
	this->matrixBuffer.upload();
	this->render(tree, models.size());
	this->matrixBuffer.clear();
	models.clear();
	// Draw Tree 2
	this->convertBillboardToMatrix(models, this->tree_b2, cameraPos);
	this->matrixBuffer.addAll(models);
	this->matrixBuffer.upload();
	this->render(tree2, models.size());
	this->matrixBuffer.clear();
	models.clear();
	// Draw Tree 3
	this->convertBillboardToMatrix(models, this->tree_b3, cameraPos);
	this->matrixBuffer.addAll(models);
	this->matrixBuffer.upload();
	this->render(tree3, models.size());
	this->matrixBuffer.clear();
	models.clear();
	// Draw Tree 4
	this->convertBillboardToMatrix(models, this->tree_b4, cameraPos);
	this->matrixBuffer.addAll(models);
	this->matrixBuffer.upload();
	this->render(tree4, models.size());
	this->matrixBuffer.clear();
	models.clear();

	this->program.unbind();
}

void BillboardManagerTest::render(Texture2D& tex, uint32_t size)
{
	tex.bind(GL_TEXTURE0);

	program.getAttr()->bind();

	vBuf.bind();
	program.getAttr()->pointer("vertices", 3, GL_FLOAT);
	vBuf.unbind();

	tBuf.bind();
	program.getAttr()->pointer("texCoords", 2, GL_FLOAT);
	tBuf.unbind();

	// Bind Instance
	matrixBuffer.bind();
	uint32_t vec4Size = sizeof(glm::vec4);
	program.getAttr()->pointer("model", 0, 4, GL_FLOAT, 4 * vec4Size, (void*)0);
	program.getAttr()->pointer("model", 1, 4, GL_FLOAT, 4 * vec4Size, (void*)(vec4Size));
	program.getAttr()->pointer("model", 2, 4, GL_FLOAT, 4 * vec4Size, (void*)(2 * vec4Size));
	program.getAttr()->pointer("model", 3, 4, GL_FLOAT, 4 * vec4Size, (void*)(3 * vec4Size));
	program.getAttr()->divisor("model", 0, 1);
	program.getAttr()->divisor("model", 1, 1);
	program.getAttr()->divisor("model", 2, 1);
	program.getAttr()->divisor("model", 3, 1);
	matrixBuffer.unbind();

	iBuf.bind();
	glDrawElementsInstanced(
		GL_TRIANGLES,
		iBuf.size(),
		GL_UNSIGNED_INT,
		0,
		size
	);
	iBuf.unbind();

	program.getAttr()->unbind();

	tex.unbind(GL_TEXTURE0);
}

void BillboardManagerTest::convertBillboardToMatrix(
	std::vector<glm::mat4>& ms,
	std::vector<BillboardTest>& bs,
	glm::vec3 cameraPos)
{
	for (uint32_t i = 0; i < bs.size(); i++)
	{
		glm::vec3 objToCamera;
		glm::vec3 lookAt;
		glm::vec3 upAux;

		objToCamera.x = cameraPos.x - bs[i].pos.x;
		objToCamera.y = 0;
		objToCamera.z = cameraPos.z - bs[i].pos.z;

		lookAt.x = 0;
		lookAt.y = 0;
		lookAt.z = 1;

		objToCamera = glm::normalize(objToCamera);

		upAux = glm::cross(lookAt, objToCamera);

		float a = _toAngle(lookAt, objToCamera);

		glm::mat4 m =
			glm::translate(glm::mat4(1.0f), bs[i].pos) *
			glm::rotate(glm::mat4(1.0f), a, glm::vec3(upAux.x, upAux.y, upAux.z)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(bs[i].scale, 0.0f));

		ms.push_back(m);
	}
}