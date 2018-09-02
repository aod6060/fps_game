#include "sys.h"

FIBITMAP* CubeMap::getContent(std::string path)
{
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(path.c_str(), 0);

	if (format == -1)
	{
		std::cout << "Could not find image: " << path << std::endl;
		return nullptr;
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
		FreeImage_Unload(bitmap);
	}

	FreeImage_FlipVertical(bitmap32);

	return bitmap32;
}

std::string CubeMap::getContents(std::string path)
{
	std::stringstream ss;
	std::string temp;
	std::ifstream in(path);
	while (std::getline(in, temp))
	{
		ss << temp << std::endl;
	}
	in.close();
	return ss.str();
}

void CubeMap::init(std::string path)
{
	rapidjson::Document document;
	std::string content = this->getContents(path);
	document.Parse(content.c_str());


	rapidjson::Value& type = document["type"];
	rapidjson::Value& version = document["version"];

	if (!strcmp(type.GetString(), "skybox") && version.GetInt() == 1)
	{
		this->loadCubemapTextures(document["textures"]);
	}
}

void CubeMap::loadCubemapTextures(rapidjson::Value& textures)
{
	glGenTextures(1, &id);

	this->bind(GL_TEXTURE0);
	// Left
	std::string left = textures["left"].GetString();
	std::cout << left << std::endl;
	// Right
	std::string right = textures["right"].GetString();
	std::cout << right << std::endl;
	// Back
	std::string back = textures["back"].GetString();
	std::cout << back << std::endl;
	// Front
	std::string front = textures["front"].GetString();
	std::cout << front << std::endl;
	// Down
	std::string down = textures["down"].GetString();
	std::cout << down << std::endl;
	// Up
	std::string up = textures["up"].GetString();
	std::cout << up << std::endl;

	std::vector<std::string> ts;
	ts.push_back(right);
	ts.push_back(left);
	ts.push_back(up);
	ts.push_back(down);
	ts.push_back(front);
	ts.push_back(back);

	for (uint32_t i = 0; i < ts.size(); i++)
	{
		FIBITMAP* bitmap = this->getContent(ts[i]);
		uint32_t w = FreeImage_GetWidth(bitmap);
		uint32_t h = FreeImage_GetHeight(bitmap);


		GLubyte* pixels = FreeImage_GetBits(bitmap);

		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0,
			GL_RGBA,
			w,
			h,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			pixels);

		FreeImage_Unload(bitmap);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	this->unbind(GL_TEXTURE0);
}

void CubeMap::bind(GLenum type)
{
	glActiveTexture(type);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);
}

void CubeMap::unbind(GLenum type)
{
	glActiveTexture(type);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CubeMap::release()
{
	glDeleteTextures(1, &id);
}