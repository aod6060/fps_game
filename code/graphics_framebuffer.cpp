#include "sys.h"

void Framebuffer::init()
{
	glGenFramebuffers(1, &id);
}

void Framebuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void Framebuffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::release()
{
	glDeleteFramebuffers(1, &id);
}

void Framebuffer::checkErrors()
{
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Couldn't create framebuffer" << std::endl;
	}
}

void Framebuffer::addTexture(GLenum attach, GLenum texType, Texture2D& texture)
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, attach, texType, texture.getID(), 0);
}

void Framebuffer::addRenderBuffer(GLenum attach, RenderBuffer& renderBuffer)
{
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attach, GL_RENDERBUFFER, renderBuffer.getID());
}