#include "sys.h"


void Attributes::set(std::string name, uint32_t id)
{
	this->attributes[name] = id;
}

void Attributes::enable(std::string name)
{
	glEnableVertexAttribArray(this->attributes[name]);
}

void Attributes::disable(std::string name)
{
	glDisableVertexAttribArray(this->attributes[name]);
}

void Attributes::pointer(
	std::string name, 
	uint32_t size, 
	GLenum type)
{
	glVertexAttribPointer(
		this->attributes[name], 
		size, 
		type, 
		GL_FALSE, 
		0, 
		0);
}

void Attributes::create()
{
	glGenVertexArrays(1, &id);
}

void Attributes::bind()
{
	glBindVertexArray(this->id);
}

void Attributes::unbind()
{
	glBindVertexArray(0);
}

void Attributes::release()
{
	glDeleteVertexArrays(1, &id);
}

uint32_t Attributes::getID()
{
	return this->id;
}