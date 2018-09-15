#include "sys.h"



glm::vec3 randColor()
{
	return glm::vec3(
		(rand() % 255) / 255.0f,
		(rand() % 255) / 255.0f,
		(rand() % 255) / 255.0f
	);
}

void TestBillboard::init(const glm::vec3& location)
{
	this->loadParticles();

	this->tex.init(this->getParticles());

	this->location = location + glm::vec3(0.0f, 1.0f, 0.0f);
	// Shaders
	vertex.create(GL_VERTEX_SHADER, "data/shaders/test_billboard.vert");
	fragment.create(GL_FRAGMENT_SHADER, "data/shaders/test_billboard.frag");
	
	// Program
	program.addShader(&vertex);
	program.addShader(&fragment);
	program.create();

	// Attributes
	program.bind();

	program.getAttr()->set("vertices", 0);
	program.getAttr()->set("texCoords", 1);
	program.getAttr()->bind();
	program.getAttr()->enable("vertices");
	program.getAttr()->enable("texCoords");
	program.getAttr()->unbind();
	program.getAttr()->disable("vertices");
	program.getAttr()->disable("texCoords");
	
	// Uniforms
	program.getUniforms()->create("proj");
	program.getUniforms()->create("view");
	program.getUniforms()->create("model");
	program.getUniforms()->create("tex0");
	program.getUniforms()->set1i("tex0", 0);
	program.getUniforms()->create("colorize");
	program.getUniforms()->set3f("colorize", randColor());

	program.unbind();

	// Vertex Buffer
	vbuf.add(-0.5f, 0.5f, 0.0f);
	vbuf.add(0.5f, 0.5f, 0.0f);
	vbuf.add(-0.5f, -0.5f, 0.0f);
	vbuf.add(0.5f, -0.5f, 0.0f);
	vbuf.create();
	vbuf.upload();

	tbuf.add(0.0f, 0.0f);
	tbuf.add(1.0f, 0.0f);
	tbuf.add(0.0f, 1.0f);
	tbuf.add(1.0f, 1.0f);
	tbuf.create();
	tbuf.upload();

	// Index Buffer
	ibuf.add(0, 1, 2);
	ibuf.add(2, 1, 3);
	ibuf.create();
	ibuf.upload();

	std::cout << vbuf.size() << ", " << ibuf.size() << std::endl;
}

static void _combine(glm::mat4& model, glm::mat4& rot)
{
	for (uint32_t i = 0; i < 3; i++)
	{
		model[i].x = rot[i].x;
		model[i].y = rot[i].y;
		model[i].z = rot[i].z;
	}
}

void TestBillboard::render(Camera& camera)
{
	glm::mat4 model =
		glm::translate(glm::mat4(1.0f), location);

	glm::mat4 rot = glm::transpose(camera.toViewMatrix());

	_combine(model, rot);

	model *= glm::scale(glm::mat4(1.0f), glm::vec3(4.0f, 4.0f, 0.0f));

	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_COLOR, GL_ONE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	program.bind();
	program.getUniforms()->setMat4("proj", camera.toProjMatrix());
	program.getUniforms()->setMat4("view", camera.toViewMatrix());
	program.getUniforms()->setMat4("model", model);

	tex.bind(GL_TEXTURE0);

	program.getAttr()->bind();

	vbuf.bind();
	program.getAttr()->pointer("vertices", 3, GL_FLOAT);
	vbuf.unbind();

	tbuf.bind();
	program.getAttr()->pointer("texCoords", 2, GL_FLOAT);
	tbuf.unbind();

	ibuf.bind();
	glDrawElements(
		GL_TRIANGLES, 
		ibuf.size(), 
		GL_UNSIGNED_INT, 
		0);
	ibuf.unbind();

	program.getAttr()->unbind();

	tex.unbind(GL_TEXTURE0);

	program.unbind();

	glDisable(GL_BLEND);

}

void TestBillboard::release()
{
	ibuf.release();
	tbuf.release();
	vbuf.release();
	program.release();
	fragment.release();
	vertex.release();
	tex.release();
}

void TestBillboard::loadParticles()
{
	std::ifstream in("data/textures/particles/particles.txt");
	std::string temp;
	while (std::getline(in, temp))
	{
		ppath.push_back(temp);
	}
	in.close();
}

std::string TestBillboard::getParticles()
{
	return ppath[rand() % ppath.size()];
}