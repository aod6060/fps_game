#include "sys.h"


void IndexBuffer::add(uint32_t x)
{
	this->list.push_back(x);
}

void IndexBuffer::add(uint32_t x, uint32_t y)
{
	this->list.push_back(x);
	this->list.push_back(y);
}

void IndexBuffer::add(uint32_t x, uint32_t y, uint32_t z)
{
	this->list.push_back(x);
	this->list.push_back(y);
	this->list.push_back(z);
}

void IndexBuffer::add(uint32_t x, uint32_t y, uint32_t z, uint32_t w)
{
	this->list.push_back(x);
	this->list.push_back(y);
	this->list.push_back(z);
	this->list.push_back(w);
}

void IndexBuffer::clear()
{
	this->list.clear();
}

void IndexBuffer::create()
{
	glGenBuffers(1, &id);
}

void IndexBuffer::upload()
{
	bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, list.size() * sizeof(uint32_t), list.data(), GL_DYNAMIC_DRAW);
	unbind();
}

void IndexBuffer::bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void IndexBuffer::unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::release()
{
	glDeleteBuffers(1, &id);
}

uint32_t IndexBuffer::size()
{
	return list.size();
}

void IndexBuffer::addAll(std::vector<uint32_t>& v)
{
	for (uint32_t i = 0; i < v.size(); i++)
	{
		add(v[i]);
	}
}

void IndexBuffer::addAll(std::vector<glm::ivec2>& v)
{
	for (uint32_t i = 0; i < v.size(); i++)
	{
		add(v[i].x, v[i].y);
	}
}

void IndexBuffer::addAll(std::vector<glm::ivec3>& v)
{
	for (uint32_t i = 0; i < v.size(); i++)
	{
		add(v[i].x, v[i].y, v[i].z);
	}
}

void IndexBuffer::addAll(std::vector<glm::ivec4>& v)
{
	for (uint32_t i = 0; i < v.size(); i++)
	{
		add(v[i].x, v[i].y, v[i].z, v[i].w);
	}
}
