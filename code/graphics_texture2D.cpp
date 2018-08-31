#include "sys.h"

void Texture2D::init(std::string path)
{
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(path.c_str(), 0);

	if (format == -1)
	{
		std::cout << "Could not find image: " << path << std::endl;
		return;
	}

	if (format == FIF_UNKNOWN)
	{
		std::cout << "Couldn't determine file format attempting to get the..." << std::endl;
		format = FreeImage_GetFIFFromFilename(path.c_str());
		if (!FreeImage_FIFSupportsReading(format))
		{
			std::cout << "Detected image format cannot be read!" << std::endl;
		}
	}

	FIBITMAP* bitmap = FreeImage_Load(format, path.c_str());
	int bitPerPixel = FreeImage_GetBPP(bitmap);
	FIBITMAP* bitmap32;
	if (bitPerPixel == 32)
	{
		bitmap32 = bitmap;
	}
	else
	{
		bitmap32 = FreeImage_ConvertTo32Bits(bitmap);
	}

	FreeImage_FlipVertical(bitmap32);

	this->width = FreeImage_GetWidth(bitmap32);
	this->height = FreeImage_GetHeight(bitmap32);
	GLubyte* textureData = FreeImage_GetBits(bitmap32);

	this->bind(GL_TEXTURE0);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		this->width,
		this->height,
		0,
		GL_BGRA,
		GL_UNSIGNED_BYTE,
		textureData);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	this->unbind(GL_TEXTURE0);

	FreeImage_Unload(bitmap32);

	if (bitPerPixel != 32)
	{
		FreeImage_Unload(bitmap);
	}
}

void Texture2D::bind(GLenum type)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture2D::unbind(GLenum type)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::release()
{
	glDeleteTextures(1, &id);
}

uint32_t Texture2D::getWidth()
{
	return this->width;
}

uint32_t Texture2D::getHeight()
{
	return this->height;
}

uint32_t Texture2D::getID()
{
	return this->id;
}
