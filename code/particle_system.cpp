#include "sys.h"


void ParticleSystem::init(
	const glm::vec3& position, 
	const glm::vec3& velocity,
	const glm::vec3& gravity,
	float maxLife)
{
	this->position = position;
	this->velocity = velocity;
	this->gravity = gravity;
	this->maxLife = maxLife;

	// Shader
	vertex.create(GL_VERTEX_SHADER, "data/shaders/particle.vert");
	fragment.create(GL_FRAGMENT_SHADER, "data/shaders/particle.frag");
	
	// Program
	program.addShader(&vertex);
	program.addShader(&fragment);
	program.create();
	
	program.bind();

	// Attributes
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

	// Uniforms
	program.getUniforms()->create("proj");
	program.getUniforms()->create("view");
	program.getUniforms()->create("tex0");
	program.getUniforms()->set1i("tex0", 0);
	program.getUniforms()->create("color");

	program.unbind();

	// Vertex Buffer
	vbuf.add(-0.5f, 0.5f, 0.0f);
	vbuf.add(0.5f, 0.5f, 0.0f);
	vbuf.add(-0.5f, -0.5f, 0.0f);
	vbuf.add(0.5f, -0.5f, 0.0f);
	vbuf.create();
	vbuf.upload();

	tbuf.add(0.0f, 0.0f);
	tbuf.add(1.0f, 0.0f);
	tbuf.add(0.0f, 1.0f);
	tbuf.add(1.0f, 1.0f);
	tbuf.create();
	tbuf.upload();

	// Index Buffer
	ibuf.add(0, 1, 2);
	ibuf.add(2, 1, 3);
	ibuf.create();
	ibuf.upload();

	// Offset Buffer
	matrix_buffer.create();

	tex.init("data/textures/particles/grass_hopper.png");

	monkey.init("data/meshes/monkey.blend");
}

static void _combine(glm::mat4& model, glm::mat4& rot)
{
	for (uint32_t i = 0; i < 3; i++)
	{
		model[i].x = rot[i].x;
		model[i].y = rot[i].y;
		model[i].z = rot[i].z;
	}
}

void ParticleSystem::update(float delta, Camera& camera, TerrainProcedural& terrain)
{
	// Erase
	for (uint32_t i = 0; i < 20; i++)
	{
		uint32_t old = -1;

		for (uint32_t i = 0; i < this->particles.size(); i++)
		{
			if (particles[i].life >= this->maxLife)
			{
				old = i;
				break;
			}

		}
		if (old != -1)
		{
			particles.erase(particles.begin() + old);
		}
		else
		{
			break;
		}
	}

	for (uint32_t i = 0; i < 1; i++)
	{
		glm::vec3 v;
		v.x = (2.0f * ((rand() % 255) / 255.0f)) - 1.0f;
		v.z = (2.0f * ((rand() % 255) / 255.0f)) - 1.0f;

		// Add
		Particle particle;
		particle.life = 0.0f;
		float size = 256.0f;

		glm::vec3 offset = glm::vec3(
			(2.0f * ((rand() % 255) / 255.0f)) - 1.0f,
			0.0f,
			(2.0f * ((rand() % 255) / 255.0f)) - 1.0f
		) * size;

		particle.position = terrain.getRandomLocation(TT_GRASS);
		glm::vec3 d(
			(2.0f * ((rand() % 255) / 255.0f)) - 1.0f,
			3.0f * ((rand() % 255) / 255.0f),
			(2.0f * ((rand() % 255) / 255.0f)) - 1.0f
		);

		d *= 10.0f;

		particle.velocity = d;
		particles.push_back(particle);
	}

	// Update Vector
	std::vector<glm::mat4> matrix;

	for (uint32_t i = 0; i < particles.size(); i++)
	{
		particles[i].velocity -= gravity * delta + glm::vec3(
			(2.0f * ((rand() % 255) / 255.0f)) - 1.0f,
			2.0f * ((rand() % 255) / 255.0f),
			(2.0f * ((rand() % 255) / 255.0f)) - 1.0f
		) * ((rand() % 100) + 1.0f) * delta;
		particles[i].position += particles[i].velocity * delta;
		particles[i].life += delta;
		//offsets.push_back(particles[i].position);

		glm::mat4 model =
			glm::translate(glm::mat4(1.0f), particles[i].position);

		glm::mat4 iv = glm::transpose(camera.toViewMatrix());

		
		_combine(model, iv);

		model *= glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));

		matrix.push_back(model);
	}

	//std::cout << matrix.size() << std::endl;

	// Clear
	matrix_buffer.clear();

	// Push Matrix Buffer
	matrix_buffer.addAll(matrix);
	matrix_buffer.upload();

}

void ParticleSystem::render(Camera& camera)
{
	uint32_t s = sizeof(glm::vec4);

	program.bind();
	program.getUniforms()->setMat4("proj", camera.toProjMatrix());
	program.getUniforms()->setMat4("view", camera.toViewMatrix());
	program.getUniforms()->set3f("color", glm::vec3(1.0f));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);

	tex.bind(GL_TEXTURE0);

	program.getAttr()->bind();

	//monkey.getVertexBuffer()->bind();
	vbuf.bind();
	program.getAttr()->pointer("vertices", 3, GL_FLOAT);
	vbuf.unbind();
	//monkey.getVertexBuffer()->unbind();

	//monkey.getTexCoordBuffer()->bind();
	tbuf.bind();
	program.getAttr()->pointer("texCoords", 2, GL_FLOAT);
	tbuf.unbind();
	//monkey.getTexCoordBuffer()->unbind();

	matrix_buffer.bind();
	program.getAttr()->pointer("model", 0, 4, GL_FLOAT, s * 4, (void*)0);
	program.getAttr()->pointer("model", 1, 4, GL_FLOAT, s * 4, (void*)(s));
	program.getAttr()->pointer("model", 2, 4, GL_FLOAT, s * 4, (void*)(s * 2));
	program.getAttr()->pointer("model", 3, 4, GL_FLOAT, s * 4, (void*)(s * 3));
	program.getAttr()->divisor("model", 0, 1);
	program.getAttr()->divisor("model", 1, 1);
	program.getAttr()->divisor("model", 2, 1);
	program.getAttr()->divisor("model", 3, 1);
	matrix_buffer.unbind();

	//monkey.getIndexBuffer()->bind();
	ibuf.bind();
	glDrawElementsInstanced(
		GL_TRIANGLES,
		//ibuf.size(),
		monkey.getIndexBuffer()->size(),
		GL_UNSIGNED_INT,
		0,
		matrix_buffer.size() / 16
	);
	//monkey.getIndexBuffer()->unbind();
	ibuf.unbind();

	program.getAttr()->unbind();

	tex.unbind(GL_TEXTURE0);

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);

	program.unbind();
}

void ParticleSystem::release()
{
	monkey.release();
	tex.release();
	matrix_buffer.release();
	ibuf.release();
	tbuf.release();
	vbuf.release();
	program.release();
	fragment.release();
	vertex.release();
}

void ParticleSystem::setPosition(const glm::vec3& position)
{
	this->position = position;
}

glm::vec3 ParticleSystem::getPosition()
{
	return this->position;
}

void ParticleSystem::setMaxLife(float maxLife)
{
	this->maxLife = maxLife;
}

float ParticleSystem::getMaxLife()
{
	return this->maxLife;
}