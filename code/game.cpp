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
static Mesh sphere;
static Mesh cylinder;
static Mesh monkey;

static float rot = 0.0f;

enum MeshType
{
	MT_CUBE = 0,
	MT_SPHERE,
	MT_CYLINDER,
	MT_MONKEY,
	MT_SIZE
};

static MeshType meshType = MeshType::MT_CUBE;

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
	sphere.init("data/meshes/sphere.blend");
	cylinder.init("data/meshes/cylender.blend");
	monkey.init("data/meshes/monkey.blend");
}

void game_update(float delta)
{
	rot += 64.0f * delta;
	if (rot > 0)
	{
		rot -= 360.0f;
	}

	if (input_isKeyDown(SDL_SCANCODE_TAB))
	{
		meshType = (MeshType)(meshType + 1);
	}

	if (meshType >= MT_SIZE)
	{
		meshType = MT_CUBE;
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
		//glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
		glm::rotate(glm::mat4(1.0f), glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

	glClearColor(TO_FLOAT_C(0), TO_FLOAT_C(191), TO_FLOAT_C(255), 1.0F);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	prog.bind();

	prog.getUniforms()->setMat4("proj", proj);
	prog.getUniforms()->setMat4("view", view);
	prog.getUniforms()->setMat4("model", model);

	texExample.bind(GL_TEXTURE0);
	
	switch (meshType)
	{
	case MT_CUBE:
		cube.render(prog);
		break;
	case MT_SPHERE:
		sphere.render(prog);
		break;
	case MT_CYLINDER:
		cylinder.render(prog);
		break;
	case MT_MONKEY:
		monkey.render(prog);
		break;
	}

	texExample.unbind(GL_TEXTURE0);

	prog.unbind();
}

void game_release()
{
	monkey.release();
	cylinder.release();
	sphere.release();
	cube.release();
	texExample.release();
	prog.release();
	frag.release();
	vertex.release();
}

