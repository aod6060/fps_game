#include "sys.h"

void ProgramWrapperBlur::init()
{
	vertex.create(GL_VERTEX_SHADER, "data/shaders/filter/blur.vert");
	fragment.create(GL_FRAGMENT_SHADER, "data/shaders/filter/blur.frag");

	program.addShader(&vertex);
	program.addShader(&fragment);
	program.create();

	program.bind();

	// Attributes
	program.getAttr()->set("vertices", 0);
	program.getAttr()->set("texCoords", 1);
	program.getAttr()->bind();
	program.getAttr()->enable("vertices");
	program.getAttr()->enable("texCoords");
	program.getAttr()->unbind();
	program.getAttr()->disable("vertices");
	program.getAttr()->disable("texCoords");

	// Uniform
	program.getUniforms()->create("proj");
	program.getUniforms()->create("view");
	program.getUniforms()->create("model");
	program.getUniforms()->create("inputTex0");
	program.getUniforms()->set1i("inputTex0", 0);
	program.getUniforms()->create("size");

	program.unbind();

}

void ProgramWrapperBlur::bind()
{
	program.bind();
}

void ProgramWrapperBlur::unbind()
{
	program.unbind();
}

void ProgramWrapperBlur::release()
{
	program.release();
	fragment.release();
	vertex.release();
}

Program* ProgramWrapperBlur::getProgram()
{
	return &this->program;
}

void ProgramWrapperBlur::bindAttribute()
{
	program.getAttr()->bind();
}

void ProgramWrapperBlur::unbindAttribute()
{
	program.getAttr()->unbind();
}

void ProgramWrapperBlur::drawArrays(GLenum type, int size)
{
	glDrawArrays(type, 0, size);
}

void ProgramWrapperBlur::drawElements(GLenum type, int size)
{
	glDrawElements(type, size, GL_UNSIGNED_INT, 0);
}

void ProgramWrapperBlur::verticesPointer()
{
	program.getAttr()->pointer("vertices", 3, GL_FLOAT);
}

void ProgramWrapperBlur::texCoordsPointer()
{
	program.getAttr()->pointer("texCoords", 2, GL_FLOAT);
}

void ProgramWrapperBlur::setProjection(const glm::mat4& proj)
{
	program.getUniforms()->setMat4("proj", proj);
}

void ProgramWrapperBlur::setView(const glm::mat4& view)
{
	program.getUniforms()->setMat4("view", view);
}

void ProgramWrapperBlur::setModel(const glm::mat4& model)
{
	program.getUniforms()->setMat4("model", model);
}

void ProgramWrapperBlur::setSize(float size)
{
	program.getUniforms()->set1f("size", size);
}

void ProgramWrapperBlur::bindInputTex(Texture2D& tex)
{
	tex.bind(GL_TEXTURE0);
}

void ProgramWrapperBlur::unbindInputTex(Texture2D& tex)
{
	tex.unbind(GL_TEXTURE0);
}

void ProgramWrapperBlur::drawMesh(Mesh& mesh, Texture2D& tex)
{
	this->bindInputTex(tex);

	this->bindAttribute();

	mesh.getVertexBuffer()->bind();
	this->verticesPointer();
	mesh.getVertexBuffer()->unbind();

	mesh.getTexCoordBuffer()->bind();
	this->texCoordsPointer();
	mesh.getTexCoordBuffer()->unbind();

	mesh.getIndexBuffer()->bind();
	this->drawElements(GL_TRIANGLES, mesh.getIndexBuffer()->size());
	mesh.getIndexBuffer()->unbind();

	this->unbindAttribute();

	this->unbindInputTex(tex);
}

void ProgramWrapperBlur::drawBuffer(
	VertexBuffer& vBuf,
	VertexBuffer& tBuf,
	IndexBuffer& iBuf,
	Texture2D& tex)
{
	this->bindInputTex(tex);

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

	this->unbindInputTex(tex);
}
