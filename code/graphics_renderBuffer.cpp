#include "sys.h"

void RenderBuffer::init(uint32_t width, uint32_t height)
{
	this->width = width;
	this->height = height;
	glGenRenderbuffers(1, &id);
	this->bind();
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	this->unbind();
}

uint32_t RenderBuffer::getID()
{
	return this->id;
}

uint32_t RenderBuffer::getWidth()
{
	return this->width;
}

uint32_t RenderBuffer::getHeight()
{
	return this->height;
}

void RenderBuffer::release()
{
	glDeleteRenderbuffers(1, &id);
}

void RenderBuffer::bind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, id);
}

void RenderBuffer::unbind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
