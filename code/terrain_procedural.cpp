#include "sys.h"

void TerrainProcedural::init()
{
	this->data.init();

	// Create Vertex Buffers
	for (int32_t y = 0; y < data.size; y++)
	{
		for (int32_t x = 0; x < data.size; x++)
		{
			// Vertices
			glm::vec3 v;
			v.x = x - (float)(data.size / 2);
			v.y = data.maskedElevation[y * data.size + x] * this->scale;
			v.z = y - (float)(data.size / 2);
			vBuf.add(v);

			// TexCoords
			glm::vec2 tv;
			tv.x = x / (float)data.size;
			tv.y = y / (float)data.size;
			tBuf.add(tv);

		}
	}

	vBuf.create();
	vBuf.upload();

	tBuf.create();
	tBuf.upload();

	for (uint32_t y = 0; y < data.size - 1; y++)
	{
		for (uint32_t x = 0; x < data.size - 1; x++)
		{
			// Triangle 1
			iBuf.add(y * data.size + x);
			iBuf.add(y * data.size + (x + 1));
			iBuf.add((y + 1) * data.size + x);

			// Triangle 2
			iBuf.add((y + 1) * data.size + x);
			iBuf.add(y * data.size + (x + 1));
			iBuf.add((y + 1) * data.size + (x + 1));
		}
	}

	iBuf.create();
	iBuf.upload();

	this->setTiling(32.0f);

	channelBlack.init("data/textures/prop/ocean.png");
	channelRed.init("data/textures/prop/beach_sand2.png");
	channelGreen.init("data/textures/prop/dirt_2.png");
	channelBlue.init("data/textures/prop/dirt_2.png");


	// Blur BlendMap
	RenderBuffer depth;
	Framebuffer frameBuffer;
	ProgramWrapperBlur blur;

	/*
	Mesh mesh;
	mesh.init("data/meshes/screen.blend");
	*/
	VertexBuffer v;
	VertexBuffer t;
	IndexBuffer i;

	v.add(glm::vec3(0.0f, 0.0f, 0.0f));
	v.add(glm::vec3(1.0f, 0.0f, 0.0f));
	v.add(glm::vec3(0.0f, 1.0f, 0.0f));
	v.add(glm::vec3(1.0f, 1.0f, 0.0f));
	v.create();
	v.upload();

	t.add(glm::vec2(0.0f, 1.0f));
	t.add(glm::vec2(1.0f, 1.0f));
	t.add(glm::vec2(0.0f, 0.0f));
	t.add(glm::vec2(1.0f, 0.0f));
	t.create();
	t.upload();

	i.add(0, 1, 2);
	i.add(2, 1, 3);
	i.create();
	i.upload();

	// size
	uint32_t width = data.blendMap.getWidth();
	uint32_t height = data.blendMap.getHeight();

	// Init depth
	depth.init(width, height);
	// Init Blend Map
	blendMap.init(width, height, GL_LINEAR, GL_LINEAR);
	// Setup Framebuffer
	frameBuffer.init();

	frameBuffer.bind();
	frameBuffer.addRenderBuffer(GL_DEPTH_ATTACHMENT, depth);
	frameBuffer.addTexture(GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blendMap);
	frameBuffer.unbind();
	frameBuffer.checkErrors();

	blur.init();

	frameBuffer.bind();


	glViewport(0, 0, width, height);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	blur.bind();

	blur.setSize(512.0f);

	blur.setProjection(glm::ortho(0.0f, (float)width, (float)height, 0.0f));
	blur.setView(glm::mat4(1.0f));
	blur.setModel(
		glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(width, height, 0.0f)));

	blur.drawBuffer(
		v,
		t,
		i,
		data.blendMap
	);

	blur.unbind();

	frameBuffer.unbind();


	// Release
	blur.release();
	frameBuffer.release();
	depth.release();

	v.release();
	t.release();
	i.release();
}

void TerrainProcedural::release()
{
	vBuf.release();
	tBuf.release();
	nBuf.release();
	iBuf.release();
	channelBlack.release();
	channelRed.release();
	channelGreen.release();
	channelBlue.release();
	this->data.release();
}

void TerrainProcedural::setScale(float scale)
{
	this->scale = scale;
}

float TerrainProcedural::getScale()
{
	return scale;
}

void TerrainProcedural::setTiling(float tiling)
{
	this->tiling = tiling;
}

float TerrainProcedural::getTiling()
{
	return this->tiling;
}

TerrainType TerrainProcedural::getTerrainType(float x, float z)
{
	uint32_t _x = x + (data.size / 2);
	uint32_t _y = z + (data.size / 2);

	if (_y > data.size - 1 && _x > data.size - 1) 
	{
		return TT_OCEAN;
	}
	else
	{
		return data.terrainType[_y * data.size + _x];
	}
}

float TerrainProcedural::getHeight(float x, float z)
{
	uint32_t _x = x + (data.size / 2);
	uint32_t _y = z + (data.size / 2);

	if (_y > data.size - 1 && _x > data.size - 1)
	{
		return 0.0f;
	}
	else
	{
		return data.maskedElevation[_y * data.size + _x] * this->scale;
	}
}

TerrainData* TerrainProcedural::getData()
{
	return &this->data;
}

Texture2D* TerrainProcedural::getBlendMap()
{
	return &this->blendMap;
}

Texture2D* TerrainProcedural::getChannelBlack()
{
	return &this->channelBlack;
}

Texture2D* TerrainProcedural::getChannelRed()
{
	return &this->channelRed;
}

Texture2D* TerrainProcedural::getChannelGreen()
{
	return &this->channelGreen;
}

Texture2D* TerrainProcedural::getChannelBlue()
{
	return &this->channelBlue;
}

VertexBuffer* TerrainProcedural::getVertexBuffer()
{
	return &this->vBuf;
}

VertexBuffer* TerrainProcedural::getTexCoordBuffer()
{
	return &this->tBuf;
}

VertexBuffer* TerrainProcedural::getNormalBuffer()
{
	return &this->nBuf;
}

IndexBuffer* TerrainProcedural::getIndexBuffer()
{
	return &this->iBuf;
}

glm::vec3 TerrainProcedural::getRandomLocation(TerrainType type)
{
	bool look = false;
	glm::vec3 location;

	while (!look)
	{
		int32_t size = data.size;

		float _x = (rand() % size) - (size / 2);
		float _z = (rand() % size) - (size / 2);

		if (this->getTerrainType(_x, _z) == type)
		{
			location = glm::vec3(_x, this->getHeight(_x, _z), _z);
			look = true;
			continue;
		}
	}

	return location;
}