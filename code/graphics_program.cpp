#include "sys.h"

void Program::printProgramLog()
{
	int len;
	char log[1024];
	glGetProgramiv(id, GL_INFO_LOG_LENGTH, &len);
	if (len > 0)
	{
		glGetProgramInfoLog(id, len, 0, log);
		std::cout << log << std::endl;
	}
}

void Program::addShader(Shader* shader)
{
	this->shaders.push_back(shader);
}

void Program::clearShader()
{
	this->shaders.clear();
}

void Program::create()
{
	this->id = glCreateProgram();
	for (uint32_t i = 0; i < shaders.size(); i++)
	{
		glAttachShader(this->id, this->shaders[i]->getID());
	}
	glLinkProgram(this->id);
	this->printProgramLog();

	this->attributes.create();

	this->uniforms.init(this);
}

void Program::bind()
{
	glUseProgram(this->id);
}

void Program::unbind()
{
	glUseProgram(0);
}

void Program::release()
{
	uniforms.release();
	attributes.release();
	for (uint32_t i = 0; i < this->shaders.size(); i++)
	{
		glDetachShader(this->id, this->shaders[i]->getID());
	}
	glDeleteProgram(this->id);
}

Attributes* Program::getAttr()
{
	return &this->attributes;
}

Uniforms* Program::getUniforms()
{
	return &this->uniforms;
}

uint32_t Program::getID()
{
	return this->id;
}
