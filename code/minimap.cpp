#include "sys.h"


void MiniMap::init()
{
	// Shader
	vertex.create(GL_VERTEX_SHADER, "data/shaders/minimap.vert");
	fragment.create(GL_FRAGMENT_SHADER, "data/shaders/minimap.frag");

	// Program
	program.addShader(&vertex);
	program.addShader(&fragment);
	program.create();
	program.bind();

	// Attribute
	program.getAttr()->set("vertices", 0);
	program.getAttr()->set("texCoords", 1);
	program.getAttr()->bind();
	program.getAttr()->enable("vertices");
	program.getAttr()->enable("texCoords");
	program.getAttr()->unbind();
	program.getAttr()->disable("vertices");
	program.getAttr()->disable("texCoords");

	// Uniform
	program.getUniforms()->create("proj");
	program.getUniforms()->create("view");
	program.getUniforms()->create("model");
	program.getUniforms()->create("tex0");
	program.getUniforms()->set1i("tex0", 0);
	program.unbind();

	// Vertex Buffer
	vBuf.add(0.0f, 0.0f, 0.0f);
	vBuf.add(1.0f, 0.0f, 0.0f);
	vBuf.add(0.0f, 1.0f, 0.0f);
	vBuf.add(1.0f, 1.0f, 0.0f);
	vBuf.create();
	vBuf.upload();

	vBuf2.add(-0.5f, -0.5f, 0.0f);
	vBuf2.add(0.5f, -0.5f, 0.0f);
	vBuf2.add(-0.5f, 0.5f, 0.0f);
	vBuf2.add(0.5f, 0.5f, 0.0f);
	vBuf2.create();
	vBuf2.upload();

	// Texcoord Buffer
	tBuf.add(0.0f, 0.0f);
	tBuf.add(1.0f, 0.0f);
	tBuf.add(0.0f, 1.0f);
	tBuf.add(1.0f, 1.0f);
	tBuf.create();
	tBuf.upload();

	// Index Buffer
	iBuf.add(0, 1, 2);
	iBuf.add(2, 1, 3);
	iBuf.create();
	iBuf.upload();

	// Player
	player.init("data/textures/camera_cursor.png");
}

void MiniMap::update(Camera& camera, TerrainData* data)
{
	if (enabled)
	{
		pos.x = camera.getPos().x + data->size / 2;
		pos.y = camera.getPos().z + data->size / 2;
		cameraPos.x = camera.getPos().x + data->size / 2;
		cameraPos.y = camera.getPos().z + data->size / 2;

		zrot = camera.getRot().y;

		if (cameraPos.x < 128.0f)
		{
			cameraPos.x = 128.0f;
		}

		if (cameraPos.x > data->biomesMap.getWidth() - 128.0f)
		{
			cameraPos.x = data->biomesMap.getWidth() - 128.0f;
		}


		if (cameraPos.y < 128.0f)
		{
			cameraPos.y = 128.0f;
		}

		if (cameraPos.y > data->biomesMap.getHeight() - 128.0f)
		{
			cameraPos.y = data->biomesMap.getHeight() - 128.0f;
		}


		if (pos.x < 4.0f)
		{
			pos.x = 4.0f;
		}

		if (pos.x > data->biomesMap.getWidth() - 4.0f)
		{
			pos.x = data->biomesMap.getWidth() - 4.0f;
		}

		if (pos.y < 4.0f)
		{
			pos.y = 4.0f;
		}

		if (pos.y > data->biomesMap.getHeight() - 4.0f)
		{
			pos.y = data->biomesMap.getHeight() - 4.0f;
		}


	}
}

void MiniMap::render(TerrainData* data)
{
	if (enabled)
	{
		glm::mat4 proj =
			glm::ortho(
				0.0f,
				256.0f,
				256.0f,
				0.0f);

		glm::mat4 view =
			glm::translate(glm::mat4(1.0f), glm::vec3(-(this->cameraPos - glm::vec2(128.0f)), 0.0f));

		glm::mat4 model =
			glm::translate(glm::mat4(1.0f), glm::vec3(0.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(data->biomesMap.getWidth(), data->biomesMap.getHeight(), 0.0f));

		glDisable(GL_DEPTH_TEST);

		glViewport(5, app_getHeight() - 256 - 5, 256, 256);

		program.bind();
		program.getUniforms()->setMat4("proj", proj);
		program.getUniforms()->setMat4("view", view);

		this->renderQuad(data->biomesMap, model);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		model =
			glm::translate(glm::mat4(1.0f), glm::vec3(pos, 0.0f)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(zrot), glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(player.getWidth(), player.getHeight(), 0));

		this->renderQuad2(player, model);
		glDisable(GL_BLEND);

		program.unbind();

		glEnable(GL_DEPTH_TEST);
	}
}

void MiniMap::release()
{
	player.release();
	iBuf.release();
	tBuf.release();
	vBuf2.release();
	vBuf.release();
	program.release();
	fragment.release();
	vertex.release();
}

void MiniMap::renderQuad(Texture2D& quad, const glm::mat4& model)
{
	program.getUniforms()->setMat4("model", model);

	quad.bind(GL_TEXTURE0);

	program.getAttr()->bind();

	vBuf.bind();
	program.getAttr()->pointer("vertices", 3, GL_FLOAT);
	vBuf.unbind();

	tBuf.bind();
	program.getAttr()->pointer("texCoords", 2, GL_FLOAT);
	tBuf.unbind();

	iBuf.bind();
	glDrawElements(GL_TRIANGLES, iBuf.size(), GL_UNSIGNED_INT, 0);
	iBuf.unbind();

	program.getAttr()->unbind();

	quad.unbind(GL_TEXTURE0);
}

void MiniMap::renderQuad2(Texture2D& quad, const glm::mat4& model)
{
	program.getUniforms()->setMat4("model", model);

	quad.bind(GL_TEXTURE0);

	program.getAttr()->bind();

	vBuf2.bind();
	program.getAttr()->pointer("vertices", 3, GL_FLOAT);
	vBuf2.unbind();

	tBuf.bind();
	program.getAttr()->pointer("texCoords", 2, GL_FLOAT);
	tBuf.unbind();

	iBuf.bind();
	glDrawElements(GL_TRIANGLES, iBuf.size(), GL_UNSIGNED_INT, 0);
	iBuf.unbind();

	program.getAttr()->unbind();

	quad.unbind(GL_TEXTURE0);
}

bool MiniMap::isEnabled()
{
	return this->enabled;
}

void MiniMap::setEnabled(bool enabled)
{
	this->enabled = enabled;
}

void MiniMap::toggleEnabled()
{
	this->setEnabled(!this->isEnabled());
}
