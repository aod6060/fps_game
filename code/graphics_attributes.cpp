#include "sys.h"


void Attributes::set(std::string name, uint32_t id)
{
	this->attributes[name] = id;
}

std::map<std::string, uint32_t>* Attributes::getAttr()
{
	return &this->attributes;
}

void Attributes::enable(std::string name)
{
	glEnableVertexAttribArray(this->attributes[name]);
}

void Attributes::enable(std::string name, uint32_t index)
{
	glEnableVertexAttribArray(this->attributes[name] + index);
}

void Attributes::disable(std::string name)
{
	glDisableVertexAttribArray(this->attributes[name]);
}

void Attributes::disable(std::string name, uint32_t index)
{
	glEnableVertexAttribArray(this->attributes[name] + index);
}

void Attributes::pointer(
	std::string name, 
	uint32_t size, 
	GLenum type)
{
	//std::cout << this->attributes[name] << std::endl;

	glVertexAttribPointer(
		this->attributes[name], 
		size, 
		type, 
		GL_FALSE, 
		0, 
		0);
}

void Attributes::pointer(
	std::string name,
	uint32_t index,
	uint32_t size,
	GLenum type,
	uint32_t offset,
	const void* pointer)
{
	glVertexAttribPointer(
		attributes[name] + index,
		size,
		type,
		GL_FALSE,
		offset,
		pointer);
}

void Attributes::divisor(
	std::string name, 
	uint32_t index,
	uint32_t divisor)
{
	glVertexAttribDivisor(this->attributes[name] + index, divisor);
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