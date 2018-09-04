#include "sys.h"

void Terrain::init(std::string path)
{
	// Load Terrain Description File
	std::stringstream ss;
	std::string temp;
	std::ifstream in(path);
	while (std::getline(in, temp))
	{
		ss << temp << std::endl;
	}
	in.close();
	rapidjson::Document doc;
	doc.Parse(ss.str().c_str());

	rapidjson::Value& type = doc["type"];
	rapidjson::Value& version = doc["version"];

	
	if (!strcmp(type.GetString(), "terrain") && version.GetInt() == 1)
	{
		// Create Heights
		std::string path = doc["height-map"].GetString();

		FREE_IMAGE_FORMAT format = FreeImage_GetFileType(path.c_str(), 0);

		if (format == -1)
		{
			std::cout << "Could not find image: " << path << std::endl;
			return;
		}

		if (format == FIF_UNKNOWN)
		{
			std::cout << "Couldn't determine file format attemping to get the..." << std::endl;
			format = FreeImage_GetFIFFromFilename(path.c_str());
			if (!FreeImage_FIFSupportsReading(format))
			{
				std::cout << "Detected image format cannot be read!" << std::endl;
			}
		}

		FIBITMAP* bitmap = FreeImage_Load(format, path.c_str());
		FreeImage_FlipVertical(bitmap);

		this->width = FreeImage_GetWidth(bitmap);
		this->height = FreeImage_GetHeight(bitmap);

		this->heights.resize(this->width * this->height);

		
		for (uint32_t y = 0; y < this->height; y++)
		{
			for (uint32_t x = 0; x < this->width; x++)
			{
				RGBQUAD color;
				FreeImage_GetPixelColor(bitmap, x, y, &color);

				float avg = (color.rgbRed + color.rgbGreen + color.rgbBlue) / 3.0f;

				heights[y * width + x] = avg / 255.0f;
			}
		}

		FreeImage_Unload(bitmap);

		// Load Blend Map
		path = doc["blend-map"].GetString();
		std::cout << path << std::endl;
		this->blendMap.init(path);

		// Load Channel Black
		path = doc["channel-black"].GetString();
		std::cout << path << std::endl;
		this->channelBlack.init(path);

		// Load Channel Red
		path = doc["channel-red"].GetString();
		std::cout << path << std::endl;
		this->channelRed.init(path);

		// Load Channel Green
		path = doc["channel-green"].GetString();
		std::cout << path << std::endl;
		this->channelGreen.init(path);

		// Load Channel Blue
		path = doc["channel-blue"].GetString();
		std::cout << path << std::endl;
		this->channelBlue.init(path);

		// Create Vertex Buffer
		for (uint32_t y = 0; y < height; y++)
		{
			for (uint32_t x = 0; x < width; x++)
			{
				// Vertices
				glm::vec3 v;
				v.x = x;
				v.y = heights[y * width + x] * this->scale;
				v.z = y;
				vBuf.add(v);
				// TexCoords
				glm::vec2 tv;
				tv.x = x / (float)width;
				tv.y = y / (float)height;
				tBuf.add(tv);
			}
		}

		vBuf.create();
		vBuf.upload();

		tBuf.create();
		tBuf.upload();

		// Create Index Buffer
		for (uint32_t y = 0; y < height - 1; y++)
		{
			for (uint32_t x = 0; x < width - 1; x++)
			{
				//uint32_t v1 = y * width + x;
				// Triangle 1
				iBuf.add(y * width + x);
				iBuf.add(y * width + (x + 1));
				iBuf.add((y + 1) * width + x);
				// Triangle 2
				iBuf.add((y + 1) * width + x);
				iBuf.add(y * width + (x + 1));
				iBuf.add((y + 1) * width + (x + 1));
			}
		}

		iBuf.create();
		iBuf.upload();
	}
	else
	{
		std::cout << "Error: either the file format is wrong version, wrong type, or file not found..." << std::endl;
	}

	this->setTiling(32.0f);
}

void Terrain::render(ProgramWrapperTerrain& prog)
{
	prog.setTiling(this->tiling);


	prog.bindBlendMap(this->blendMap);
	prog.bindChannelBlack(this->channelBlack);
	prog.bindChannelRed(this->channelRed);
	prog.bindChannelGreen(this->channelGreen);
	prog.bindChannelBlue(this->channelBlue);

	prog.bindAttribute();

	vBuf.bind();
	prog.verticesPointer();
	vBuf.unbind();

	tBuf.bind();
	prog.texCoordsPointer();
	tBuf.unbind();

	iBuf.bind();
	prog.drawElements(GL_TRIANGLES, iBuf.size());
	iBuf.unbind();

	prog.unbindAttribute();

	prog.unbindBlendMap(this->blendMap);
	prog.unbindChannelBlack(this->channelBlack);
	prog.unbindChannelRed(this->channelRed);
	prog.unbindChannelGreen(this->channelGreen);
	prog.unbindChannelBlue(this->channelBlue);
}

void Terrain::release()
{
	iBuf.release();
	vBuf.release();
	channelBlue.release();
	channelGreen.release();
	channelRed.release();
	channelBlack.release();
	blendMap.release();
}

void Terrain::setScale(float scale)
{
	this->scale = scale;
}

float Terrain::getScale()
{
	return this->scale;
}

void Terrain::setTiling(float tiling)
{
	this->tiling = tiling;
}

float Terrain::getTiling()
{
	return this->tiling;
}
