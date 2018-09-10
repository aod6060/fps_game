#include "sys.h"


void ProgramWrapperBillboard::init()
{
	// Shaders
	vertex.create(GL_VERTEX_SHADER, "data/shaders/billboard.vert");
	fragment.create(GL_FRAGMENT_SHADER, "data/shaders/billboard.frag");
	// Program
	program.addShader(&vertex);
	program.addShader(&fragment);
	program.create();
	program.bind();
	// Attribute
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
	program.getUniforms()->create("cameraPos");
	program.getUniforms()->create("billboard");
	program.getUniforms()->set1i("billboard", 0);
	program.unbind();
	// Vertex Buffers
	vBuf.add(-0.5f, 1.0f, 0.0f);
	vBuf.add(0.5f, 1.0f, 0.0f);
	vBuf.add(-0.5f, 0.0f, 0.0f);
	vBuf.add(0.5f, 0.0f, 0.0f);
	vBuf.create();
	vBuf.upload();

	tBuf.add(glm::vec2(0.0f, 0.0f));
	tBuf.add(glm::vec2(1.0f, 0.0f));
	tBuf.add(glm::vec2(0.0f, 1.0f));
	tBuf.add(glm::vec2(1.0f, 1.0f));
	tBuf.create();
	tBuf.upload();

	// Index Buffer
	iBuf.add(0, 1, 2);
	iBuf.add(2, 1, 3);
	iBuf.create();
	iBuf.upload();
}

void ProgramWrapperBillboard::bind()
{
	program.bind();
}

void ProgramWrapperBillboard::unbind()
{
	program.unbind();
}

void ProgramWrapperBillboard::release()
{
	iBuf.release();
	tBuf.release();
	vBuf.release();
	program.release();
	fragment.release();
	vertex.release();
}

Program* ProgramWrapperBillboard::getProgram()
{
	return &program;
}

void ProgramWrapperBillboard::bindAttribute()
{
	program.getAttr()->bind();
}

void ProgramWrapperBillboard::unbindAttribute()
{
	program.getAttr()->unbind();
}

void ProgramWrapperBillboard::verticesPointer()
{
	program.getAttr()->pointer("vertices", 3, GL_FLOAT);
}

void ProgramWrapperBillboard::texCoordsPointer()
{
	program.getAttr()->pointer("texCoords", 2, GL_FLOAT);
}

void ProgramWrapperBillboard::drawElements(GLenum type, int size)
{
	glDrawElements(type, size, GL_UNSIGNED_INT, 0);
}

void ProgramWrapperBillboard::setProjection(const glm::mat4& proj)
{
	program.getUniforms()->setMat4("proj", proj);
}

void ProgramWrapperBillboard::setView(const glm::mat4& view)
{
	program.getUniforms()->setMat4("view", view);
}

void ProgramWrapperBillboard::setModel(const glm::mat4& model)
{
	program.getUniforms()->setMat4("model", model);
}

void ProgramWrapperBillboard::setCameraPos(const glm::vec3& cameraPos)
{
	program.getUniforms()->set3f("cameraPos", cameraPos);
}

void ProgramWrapperBillboard::bindBillboard(Texture2D& billboard)
{
	billboard.bind(GL_TEXTURE0);
}

void ProgramWrapperBillboard::unbindBillboard(Texture2D& billboard)
{
	billboard.unbind(GL_TEXTURE0);
}

void ProgramWrapperBillboard::draw(Texture2D& billboard)
{
	this->bindBillboard(billboard);

	this->bindAttribute();

	vBuf.bind();
	this->verticesPointer();
	vBuf.unbind();

	tBuf.bind();
	this->texCoordsPointer();
	tBuf.unbind();

	iBuf.bind();
	this->drawElements(GL_TRIANGLES, iBuf.size());
	iBuf.unbind();

	this->unbindAttribute();

	this->unbindBillboard(billboard);
}
