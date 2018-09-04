#include "sys.h"

void ProgramWrapperTerrain::init()
{
	// Shader
	vertex.create(GL_VERTEX_SHADER, "data/shaders/terrain.vert");
	fragment.create(GL_FRAGMENT_SHADER, "data/shaders/terrain.frag");
	// Program
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

	// Uniforms
	program.getUniforms()->create("proj");
	program.getUniforms()->create("view");
	program.getUniforms()->create("model");
	program.getUniforms()->create("tiling");

	program.getUniforms()->create("blendMap");
	program.getUniforms()->set1i("blendMap", 0);

	program.getUniforms()->create("channelBlack");
	program.getUniforms()->set1i("channelBlack", 1);

	program.getUniforms()->create("channelRed");
	program.getUniforms()->set1i("channelRed", 2);

	program.getUniforms()->create("channelGreen");
	program.getUniforms()->set1i("channelGreen", 3);

	program.getUniforms()->create("channelBlue");
	program.getUniforms()->set1i("channelBlue", 4);

	program.unbind();
}

void ProgramWrapperTerrain::bind()
{
	program.bind();
}

void ProgramWrapperTerrain::unbind()
{
	program.unbind();
}

void ProgramWrapperTerrain::release()
{
	program.release();
	fragment.release();
	vertex.release();
}

Program* ProgramWrapperTerrain::getProgram()
{
	return &program;
}

void ProgramWrapperTerrain::bindAttribute()
{
	program.getAttr()->bind();
}

void ProgramWrapperTerrain::unbindAttribute()
{
	program.getAttr()->unbind();
}

void ProgramWrapperTerrain::verticesPointer()
{
	program.getAttr()->pointer("vertices", 3, GL_FLOAT);
}

void ProgramWrapperTerrain::texCoordsPointer()
{
	program.getAttr()->pointer("texCoords", 2, GL_FLOAT);
}

void ProgramWrapperTerrain::drawArrays(GLenum type, int size)
{
	glDrawArrays(type, 0, size);
}

void ProgramWrapperTerrain::drawElements(GLenum type, int size)
{
	glDrawElements(type, size, GL_UNSIGNED_INT, 0);
}

void ProgramWrapperTerrain::setProjection(const glm::mat4& proj)
{
	program.getUniforms()->setMat4("proj", proj);
}

void ProgramWrapperTerrain::setView(const glm::mat4& view)
{
	program.getUniforms()->setMat4("view", view);
}

void ProgramWrapperTerrain::setModel(const glm::mat4& model)
{
	program.getUniforms()->setMat4("model", model);
}

void ProgramWrapperTerrain::setTiling(float tiling)
{
	program.getUniforms()->set1f("tiling", tiling);
}

void ProgramWrapperTerrain::bindBlendMap(Texture2D& blendMap)
{
	blendMap.bind(GL_TEXTURE0);
}

void ProgramWrapperTerrain::bindChannelBlack(Texture2D& channelBlack)
{
	channelBlack.bind(GL_TEXTURE1);
}

void ProgramWrapperTerrain::bindChannelRed(Texture2D& channelRed)
{
	channelRed.bind(GL_TEXTURE2);
}

void ProgramWrapperTerrain::bindChannelGreen(Texture2D& channelGreen)
{
	channelGreen.bind(GL_TEXTURE3);
}

void ProgramWrapperTerrain::bindChannelBlue(Texture2D& channelBlue)
{
	channelBlue.bind(GL_TEXTURE4);
}

void ProgramWrapperTerrain::unbindBlendMap(Texture2D& blendMap)
{
	blendMap.unbind(GL_TEXTURE0);
}

void ProgramWrapperTerrain::unbindChannelBlack(Texture2D& channelBlack)
{
	channelBlack.unbind(GL_TEXTURE1);
}

void ProgramWrapperTerrain::unbindChannelRed(Texture2D& channelRed)
{
	channelRed.unbind(GL_TEXTURE2);
}

void ProgramWrapperTerrain::unbindChannelGreen(Texture2D& channelGreen)
{
	channelGreen.unbind(GL_TEXTURE3);
}

void ProgramWrapperTerrain::unbindChannelBlue(Texture2D& channelBlue)
{
	channelBlue.unbind(GL_TEXTURE4);
}

