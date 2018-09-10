#include "sys.h"


void VertexBuffer::add(float x)
{
	list.push_back(x);
}

void VertexBuffer::add(float x, float y)
{
	list.push_back(x);
	list.push_back(y);
}

void VertexBuffer::add(const glm::vec2& v)
{
	this->add(v.x, v.y);
}

void VertexBuffer::add(float x, float y, float z)
{
	list.push_back(x);
	list.push_back(y);
	list.push_back(z);
}

void VertexBuffer::add(const glm::vec3& v)
{
	this->add(v.x, v.y, v.z);
}

void VertexBuffer::add(float x, float y, float z, float w)
{
	list.push_back(x);
	list.push_back(y);
	list.push_back(z);
	list.push_back(w);
}

void VertexBuffer::add(const glm::vec4& v)
{
	this->add(v.x, v.y, v.z, v.w);
}

void VertexBuffer::add(const glm::mat4& m)
{
	const float* pSrc = (const float*)glm::value_ptr(m);

	for (uint32_t i = 0; i < 16; i++)
	{
		this->add(pSrc[i]);
	}
}

void VertexBuffer::clear()
{
	this->list.clear();
}

void VertexBuffer::create()
{
	glGenBuffers(1, &this->id);
}

void VertexBuffer::upload()
{
	this->bind();
	glBufferData(GL_ARRAY_BUFFER, this->size() * sizeof(float), list.data(), GL_DYNAMIC_DRAW);
	this->unbind();
}

void VertexBuffer::bind()
{
	// Oo0RHzmRjCM
	glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VertexBuffer::unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::release()
{
	glDeleteBuffers(1, &id);
}

uint32_t VertexBuffer::size()
{
	return list.size();
}

void VertexBuffer::addAll(std::vector<float>& v)
{
	for (uint32_t i = 0; i < v.size(); i++)
	{
		this->add(v[i]);
	}
}

void VertexBuffer::addAll(std::vector<glm::vec2>& v)
{
	for (uint32_t i = 0; i < v.size(); i++)
	{
		this->add(v[i]);
	}
}

void VertexBuffer::addAll(std::vector<glm::vec3>& v)
{
	for (uint32_t i = 0; i < v.size(); i++)
	{
		this->add(v[i]);
	}
}

void VertexBuffer::addAll(std::vector<glm::vec4>& v)
{
	for (uint32_t i = 0; i < v.size(); i++)
	{
		this->add(v[i]);
	}
}

void VertexBuffer::addAll(std::vector<glm::mat4>& m)
{
	for (uint32_t i = 0; i < m.size(); i++)
	{
		this->add(m[i]);
	}
}