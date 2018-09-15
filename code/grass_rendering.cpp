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

void GrassRendering::init(TerrainProcedural& prog)
{
	this->loadGrassPath();

	// Mesh
	this->grass.init("data/meshes/grass.blend", true);
	
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
	grass1.init(this->getGrassPath());
	grass2.init(this->getGrassPath());
	grass3.init(this->getGrassPath());
	grass4.init(this->getGrassPath());
	grass5.init(this->getGrassPath());
	grass6.init(this->getGrassPath());
	grass7.init(this->getGrassPath());
	grass8.init(this->getGrassPath());
	grass9.init(this->getGrassPath());
	grass10.init(this->getGrassPath());

	int32_t size = prog.getData()->size;

	grass1_buf.create();
	grass2_buf.create();
	grass3_buf.create();
	grass4_buf.create();
	grass5_buf.create();
	grass6_buf.create();
	grass7_buf.create();
	grass8_buf.create();
	grass9_buf.create();
	grass10_buf.create();

	std::ofstream test("test.txt");

	for (int32_t y = 0; y < size; y++)
	{
		for (int32_t x = 0; x < size; x++)
		{
			//int32_t i = y * size + x;

			float _x = x - size / 2.0f;
			//float _y = data->maskedElevation[i] * scale;
			float _z = y - size / 2.0f;

			
			if (prog.getTerrainType(_x, _z) == TT_GRASS)
			{
				//int32_t tp = rand() % 3;
				//int32_t tp = rand() % 10;
				//int32_t tp = ((glm::perlin(glm::vec3(x / 32.0f, y / 32.0f, prog.getData()->seed)) + 1.0f) * 0.5f) * 11.0f;
				
				int32_t m1 = rand() % 11;
				int32_t m2 = rand() % 11;
				int32_t m3 = rand() % 11;

				int32_t tp = (m1 + m2 + m3) / 3;

				float _y = prog.getHeight(_x, _z);

				float rot = rand() % 360;

				//std::cout << t << std::endl;

				test << tp << std::endl;

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
					grass3_buf.add(test);
				}
				else if (tp == 3)
				{
					grass4_buf.add(test);
				}
				else if (tp == 4)
				{
					grass5_buf.add(test);
				}
				else if (tp == 5)
				{
					grass6_buf.add(test);
				}
				else if (tp == 6)
				{
					grass7_buf.add(test);
				}
				else if (tp == 7)
				{
					grass8_buf.add(test);
				}
				else if (tp == 8)
				{
					grass9_buf.add(test);
				}
				else if (tp == 9)
				{
					grass10_buf.add(test);
				}
			}
		}
	}

	std::cout << (grass1_buf.size()) << ", " << (grass2_buf.size()) << ", " << (grass3_buf.size()) << std::endl;
	test.close();

	grass1_buf.upload();
	grass2_buf.upload();
	grass3_buf.upload();
	grass4_buf.upload();
	grass5_buf.upload();
	grass6_buf.upload();
	grass7_buf.upload();
	grass8_buf.upload();
	grass9_buf.upload();
	grass10_buf.upload();

}

void GrassRendering::update(float delta)
{
}

void GrassRendering::render(Camera& camera)
{
	program.bind();
	program.getUniforms()->setMat4("proj", camera.toProjMatrix());
	program.getUniforms()->setMat4("view", camera.toViewMatrix());

	this->render(grass1, grass1_buf);
	this->render(grass2, grass2_buf);
	this->render(grass3, grass3_buf);
	this->render(grass4, grass4_buf);
	this->render(grass5, grass5_buf);
	this->render(grass6, grass6_buf);
	this->render(grass7, grass7_buf);
	this->render(grass8, grass8_buf);
	this->render(grass9, grass9_buf);
	this->render(grass10, grass10_buf);

	program.unbind();
}

void GrassRendering::release()
{
	grass10_buf.release();
	grass9_buf.release();
	grass8_buf.release();
	grass7_buf.release();
	grass6_buf.release();
	grass5_buf.release();
	grass4_buf.release();
	grass3_buf.release();
	grass2_buf.release();
	grass1_buf.release();

	grass10.release();
	grass9.release();
	grass8.release();
	grass7.release();
	grass6.release();
	grass5.release();
	grass4.release();
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
		buf.size() / 16
	);
	grass.getIndexBuffer()->unbind();

	program.getAttr()->unbind();
	tex.unbind(GL_TEXTURE0);
}

void GrassRendering::loadGrassPath()
{
	std::ifstream in("data/textures/grass/grass.txt");
	std::string temp;
	while (std::getline(in, temp))
	{
		this->grass_path.push_back(temp);
	}
	in.close();
}

std::string GrassRendering::getGrassPath()
{
	return this->grass_path[rand() % grass_path.size()];
}
