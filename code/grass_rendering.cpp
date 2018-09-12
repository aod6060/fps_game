#include "sys.h"


void GrassRendering::add(int type, glm::vec3 pos)
{
	if (type == 0)
	{
		grass1_buf.add(glm::translate(glm::mat4(1.0f), pos));
	}
	else if (type == 1)
	{
		grass2_buf.add(glm::translate(glm::mat4(1.0f), pos));
	}
	else if (type == 2)
	{
		grass3_buf.add(glm::translate(glm::mat4(1.0f), pos));
	}
}

void GrassRendering::upload()
{
	grass1_buf.upload();
	grass2_buf.upload();
	grass3_buf.upload();
}

void GrassRendering::init(TerrainData* data, float scale)
{
	// Mesh
	this->grass.init("data/meshes/grass.blend");
	
	// Shader
	this->vertex.create(GL_VERTEX_SHADER, "data/shaders/grass.vert");
	this->fragment.create(GL_FRAGMENT_SHADER, "data/shaders/grass.frag");
	
	// Program
	program.addShader(&this->vertex);
	program.addShader(&this->fragment);
	program.create();

	program.bind();
	
	// Attribute
	program.getAttr()->set("vertices", 0);
	program.getAttr()->set("texCoords", 1);
	program.getAttr()->set("model", 2);
	program.getAttr()->bind();
	program.getAttr()->enable("vertices");
	program.getAttr()->enable("texCoords");
	program.getAttr()->enable("model", 0);
	program.getAttr()->enable("model", 1);
	program.getAttr()->enable("model", 2);
	program.getAttr()->enable("model", 3);
	program.getAttr()->unbind();
	program.getAttr()->disable("vertices");
	program.getAttr()->disable("texCoords");
	program.getAttr()->disable("model", 0);
	program.getAttr()->disable("model", 1);
	program.getAttr()->disable("model", 2);
	program.getAttr()->disable("model", 3);
	
	// Uniform
	program.getUniforms()->create("proj");
	program.getUniforms()->create("view");
	program.getUniforms()->create("grass");
	program.getUniforms()->set1i("grass", 0);
	
	program.unbind();

	// Matrix Buffers
	grass1.init("data/textures/grass.png");
	grass2.init("data/textures/grass2.png");
	grass3.init("data/textures/grass3.png");

	int32_t size = data->size;

	grass1_buf.create();
	grass2_buf.create();
	//grass3_buf.create();

	std::ofstream test("test.txt");

	for (int32_t y = 0; y < size - 1; y++)
	{
		for (int32_t x = 0; x < size - 1; x++)
		{
			uint32_t i = y * size + x;

			TerrainType type = data->terrainType[i];

			if (type == TT_GRASS)
			{
				int32_t tp = rand() % 2;

				float _x = x - size / 2;
				float _y = data->maskedElevation[i] * scale;
				float _z = y - size / 2;

				float rot = rand() % 360;

				//std::cout << t << std::endl;

				glm::mat4 test =
					glm::translate(glm::mat4(1.0f), glm::vec3(_x, _y, _z)) *
					glm::rotate(glm::mat4(1.0f), glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

				if (tp == 0)
				{
					grass1_buf.add(test);
				}
				else if (tp == 1)
				{
					grass2_buf.add(test);
				}
				else if (tp == 2)
				{
					//grass3_buf.add(test);
					//test << t << ": " << _x << ", " << _y << ", " << _z << std::endl;
				}
			}
		}
	}

	test.close();

	grass1_buf.upload();
	grass2_buf.upload();
	//grass3_buf.upload();

	//this->upload();

}

void GrassRendering::render(Camera& camera)
{
	program.bind();
	program.getUniforms()->setMat4("proj", camera.toProjMatrix());
	program.getUniforms()->setMat4("view", camera.toViewMatrix());

	this->render(grass1, grass1_buf);
	this->render(grass2, grass2_buf);
	//this->render(grass2, grass3_buf);

	program.unbind();
}

void GrassRendering::release()
{
	grass3_buf.release();
	grass2_buf.release();
	grass1_buf.release();
	grass3.release();
	grass2.release();
	grass1.release();
	program.release();
	fragment.release();
	vertex.release();
	this->grass.release();
}

void GrassRendering::render(Texture2D& tex, VertexBuffer& buf)
{
	uint32_t vec4Size = sizeof(glm::vec4);
	tex.bind(GL_TEXTURE0);
	program.getAttr()->bind();

	grass.getVertexBuffer()->bind();
	program.getAttr()->pointer("vertices", 3, GL_FLOAT);
	grass.getVertexBuffer()->unbind();

	grass.getTexCoordBuffer()->bind();
	program.getAttr()->pointer("texCoords", 2, GL_FLOAT);
	grass.getTexCoordBuffer()->unbind();

	buf.bind();
	program.getAttr()->pointer("model", 0, 4, GL_FLOAT, 4 * vec4Size, (void*)0);
	program.getAttr()->pointer("model", 1, 4, GL_FLOAT, 4 * vec4Size, (void*)(vec4Size));
	program.getAttr()->pointer("model", 2, 4, GL_FLOAT, 4 * vec4Size, (void*)(vec4Size * 2));
	program.getAttr()->pointer("model", 3, 4, GL_FLOAT, 4 * vec4Size, (void*)(vec4Size * 3));
	program.getAttr()->divisor("model", 0, 1);
	program.getAttr()->divisor("model", 1, 1);
	program.getAttr()->divisor("model", 2, 1);
	program.getAttr()->divisor("model", 3, 1);
	buf.unbind();

	grass.getIndexBuffer()->bind();
	glDrawElementsInstanced(
		GL_TRIANGLES,
		grass.getIndexBuffer()->size(),
		GL_UNSIGNED_INT,
		0,
		buf.size()
	);

	grass.getIndexBuffer()->unbind();

	program.getAttr()->unbind();
	tex.unbind(GL_TEXTURE0);
}
