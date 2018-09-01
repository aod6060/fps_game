#include "game.h"

// Shader
static Shader vertex;
static Shader frag;
// Program
static Program prog;
// Texture2D
static Texture2D texExample;
// Meshes
static Mesh cube;

static float rot = 0.0f;

void game_init()
{
	glEnable(GL_DEPTH_TEST);

	// Shaders
	vertex.create(GL_VERTEX_SHADER, "data/shaders/main.vert");
	frag.create(GL_FRAGMENT_SHADER, "data/shaders/main.frag");

	// Program
	prog.addShader(&vertex);
	prog.addShader(&frag);
	prog.create();
	prog.bind();

	// Attributes
	prog.getAttr()->set("vertices", 0);
	prog.getAttr()->set("texCoords", 1);
	prog.getAttr()->set("normals", 2);
	prog.getAttr()->bind();
	prog.getAttr()->enable("vertices");
	prog.getAttr()->enable("texCoords");
	prog.getAttr()->enable("normals");
	prog.getAttr()->unbind();
	prog.getAttr()->disable("vertices");
	prog.getAttr()->disable("texCoords");
	prog.getAttr()->disable("normals");

	// Uniforms
	prog.getUniforms()->create("proj");
	prog.getUniforms()->create("view");
	prog.getUniforms()->create("model");
	prog.getUniforms()->create("tex0");
	prog.getUniforms()->set1i("tex0", 0);
	prog.unbind();

	// Texture2D
	texExample.init("data/textures/happyface.png");

	// Meshes
	cube.init("data/meshes/cube.blend");

}

void game_update(float delta)
{
	rot += 64.0f * delta;
	if (rot > 0)
	{
		rot -= 360.0f;
	}
}

void game_fixedUpdate()
{

}

#define TO_FLOAT_C(r) (r / 255.0f)

void game_render()
{
	glm::mat4 proj =
		glm::perspective(
			glm::radians(45.0f), 
			(float)app_getWidth() / (float)app_getHeight(), 
			1.0f, 
			1024.0f);

	glm::mat4 view =
		glm::mat4(1.0f);

	glm::mat4 model =
		glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) * 
		glm::scale(glm::mat4(1.0f), glm::vec3(3.0f));
		//glm::rotate(glm::mat4(1.0f), glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

	glClearColor(TO_FLOAT_C(0), TO_FLOAT_C(191), TO_FLOAT_C(255), 1.0F);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	prog.bind();

	prog.getUniforms()->setMat4("proj", proj);
	prog.getUniforms()->setMat4("view", view);
	prog.getUniforms()->setMat4("model", model);



	texExample.bind(GL_TEXTURE0);
	cube.render(prog);
	texExample.unbind(GL_TEXTURE0);

	//prog.getAttr()->bind();

	// https://www.youtube.com/watch?v=t3Dlt_wyLTg


	//texExample.bind(GL_TEXTURE0);
	/*
	vBuf.bind();
	prog.getAttr()->pointer("vertices", 3, GL_FLOAT);
	vBuf.unbind();

	cBuf.bind();
	prog.getAttr()->pointer("vertexColors", 4, GL_FLOAT);
	cBuf.unbind();

	tBuf.bind();
	prog.getAttr()->pointer("texCoords", 2, GL_FLOAT);
	tBuf.unbind();

	iBuf.bind();
	//glDrawArrays(GL_TRIANGLES, 0, vBuf.size() / 3);
	glDrawElements(GL_TRIANGLES, iBuf.size(), GL_UNSIGNED_INT, 0);
	iBuf.unbind();

	//texExample.unbind(GL_TEXTURE0);
	*/


	//prog.getAttr()->unbind();

	prog.unbind();
}

void game_release()
{
	cube.release();
	texExample.release();
	prog.release();
	frag.release();
	vertex.release();
}

