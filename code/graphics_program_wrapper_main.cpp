#include "sys.h"

void ProgramWrapperMain::init()
{
	// Shaders
	vertex.create(GL_VERTEX_SHADER, "data/shaders/main.vert");
	fragment.create(GL_FRAGMENT_SHADER, "data/shaders/main.frag");
	// Program
	program.addShader(&vertex);
	program.addShader(&fragment);
	program.create();

	program.bind();
	// Attributes
	program.getAttr()->set("vertices", 0);
	program.getAttr()->set("texCoords", 1);
	program.getAttr()->set("normals", 2);
	program.getAttr()->bind();
	program.getAttr()->enable("vertices");
	program.getAttr()->enable("texCoords");
	program.getAttr()->enable("normals");
	program.getAttr()->unbind();
	program.getAttr()->disable("vertices");
	program.getAttr()->disable("texCoords");
	program.getAttr()->disable("normals");
	// Uniforms
	program.getUniforms()->create("proj");
	program.getUniforms()->create("view");
	program.getUniforms()->create("model");
	program.getUniforms()->create("tex0");
	program.getUniforms()->set1i("tex0", 0);
	program.getUniforms()->create("cube0");
	program.getUniforms()->set1i("cube0", 1);
	program.getUniforms()->create("color");
	program.unbind();
}

void ProgramWrapperMain::bind()
{
	program.bind();
}

void ProgramWrapperMain::unbind()
{
	program.unbind();
}

void ProgramWrapperMain::release()
{
	program.release();
	fragment.release();
	vertex.release();
}

Program* ProgramWrapperMain::getProgram()
{
	return &this->program;
}

void ProgramWrapperMain::bindAttribute()
{
	program.getAttr()->bind();
}

void ProgramWrapperMain::unbindAttribute()
{
	program.getAttr()->unbind();
}

void ProgramWrapperMain::verticesPointer()
{
	program.getAttr()->pointer("vertices", 3, GL_FLOAT);
}

void ProgramWrapperMain::texCoordsPointer()
{
	program.getAttr()->pointer("texCoords", 2, GL_FLOAT);
}

void ProgramWrapperMain::normalsPointer()
{
	program.getAttr()->pointer("normals", 3, GL_FLOAT);
}

void ProgramWrapperMain::drawArrays(GLenum type, uint32_t count)
{
	glDrawArrays(type, 0, count);
}

void ProgramWrapperMain::drawElements(GLenum type, uint32_t count)
{
	glDrawElements(type, count, GL_UNSIGNED_INT, 0);
}

void ProgramWrapperMain::setProjection(const glm::mat4& m)
{
	program.getUniforms()->setMat4("proj", m);
}

void ProgramWrapperMain::setView(const glm::mat4& m)
{
	program.getUniforms()->setMat4("view", m);
}

void ProgramWrapperMain::setModel(const glm::mat4& m)
{
	program.getUniforms()->setMat4("model", m);
}

void ProgramWrapperMain::setColor(const glm::vec3& c)
{
	program.getUniforms()->set3f("color", c);
}

void ProgramWrapperMain::bindTex0(Texture2D& tex)
{
	tex.bind(GL_TEXTURE0);
}

void ProgramWrapperMain::unbindTex0(Texture2D& tex)
{
	tex.unbind(GL_TEXTURE0);
}

void ProgramWrapperMain::bindCube0(CubeMap& cube)
{
	cube.bind(GL_TEXTURE1);
}

void ProgramWrapperMain::unbindCube0(CubeMap& cube)
{
	cube.unbind(GL_TEXTURE1);
}
