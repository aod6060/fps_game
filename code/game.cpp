#include "game.h"

// Shader
static Shader vertex;
static Shader frag;
// Program
static Program prog;
// Vertex Buffer
static VertexBuffer vBuf;
// Color Buffer
static VertexBuffer cBuf;

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
	prog.getAttr()->set("vertexColors", 1);
	prog.getAttr()->bind();
	prog.getAttr()->enable("vertices");
	prog.getAttr()->enable("vertexColors");
	prog.getAttr()->unbind();
	prog.getAttr()->disable("vertices");
	prog.getAttr()->disable("vertexColors");

	// Uniforms
	prog.getUniforms()->create("proj");
	prog.getUniforms()->create("view");
	prog.getUniforms()->create("model");

	prog.unbind();

	// Vertex Buffer
	vBuf.add(0.0f, 1.0f, 0.0f);
	vBuf.add(1.0f, -1.0f, 0.0f);
	vBuf.add(-1.0f, -1.0f, 0.0f);
	vBuf.create();
	vBuf.upload();

	// Color Buffer
	cBuf.add(1.0f, 0.0f, 0.0f, 1.0f);
	cBuf.add(0.0f, 1.0f, 0.0f, 1.0f);
	cBuf.add(0.0f, 0.0f, 1.0f, 1.0f);
	cBuf.create();
	cBuf.upload();


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
		glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

	glClearColor(TO_FLOAT_C(0), TO_FLOAT_C(191), TO_FLOAT_C(255), 1.0F);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	prog.bind();

	prog.getUniforms()->setMat4("proj", proj);
	prog.getUniforms()->setMat4("view", view);
	prog.getUniforms()->setMat4("model", model);

	prog.getAttr()->bind();

	vBuf.bind();
	prog.getAttr()->pointer("vertices", 3, GL_FLOAT);
	vBuf.unbind();

	cBuf.bind();
	prog.getAttr()->pointer("vertexColors", 4, GL_FLOAT);
	cBuf.unbind();

	glDrawArrays(GL_TRIANGLES, 0, vBuf.size() / 3);

	prog.getAttr()->unbind();

	prog.unbind();
}

void game_release()
{
	cBuf.release();
	vBuf.release();
	prog.release();
	frag.release();
	vertex.release();
}

