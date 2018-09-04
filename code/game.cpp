#include "game.h"

// Program Manager
static ProgramWrapperMain progMain;
static ProgramWrapperTerrain progTerrain;

// Texture2D
static Texture2D texExample;
static CubeMap cubeMap;

// Meshes
static Mesh cube;
static Mesh sphere;
static Mesh cylinder;
static Mesh monkey;

// Terrain
static Terrain terrain;

// Camera
static Camera camera;

// Color

static std::vector<glm::vec3> colors = {
	glm::vec3(1.0f),
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 0.0f),
	glm::vec3(1.0f, 0.0f, 1.0f),
	glm::vec3(0.0f, 1.0f, 1.0f),
	glm::vec3(1.0f, 0.5f, 0.0f),
	glm::vec3(1.0f, 0.0f, 0.5f),
	glm::vec3(0.5f, 1.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.5f),
	glm::vec3(0.5f, 0.0f, 1.0f),
	glm::vec3(0.0f, 0.5f, 1.0f),
	glm::vec3(1.0f, 0.5f, 0.5f),
	glm::vec3(0.5f, 1.0f, 0.5f),
	glm::vec3(0.5f, 0.5f, 1.0f),
	glm::vec3(0.5f, 0.5f, 0.5f),
	glm::vec3(0.0f)
};

float pulseTime = 0.0f;
uint32_t index = 0;
uint32_t index2 = 1;

static float rot = 0.0f;

void renderMesh(const glm::vec3& pos);
uint32_t getColorIndex();
uint32_t getColorIndex2();

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

	progMain.init();
	progTerrain.init();

	// Texture2D
	texExample.init("data/textures/happyface.png");

	// CubeMap
	cubeMap.init("data/textures/skybox/skybox.json");

	// Meshes
	cube.init("data/meshes/cube.blend");
	sphere.init("data/meshes/sphere.blend");
	cylinder.init("data/meshes/cylender.blend");
	monkey.init("data/meshes/monkey.blend");

	// Terrain
	terrain.init("data/terrain/terrain1/terrain1.json");

	// Camera
	camera.init(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f));

	index = getColorIndex();
	index2 = getColorIndex2();

	input_setGrab(true);


}

void game_update(float delta)
{
	rot += 64.0f * delta;
	if (rot > 0)
	{
		rot -= 360.0f;
	}

	if (input_isKeyDown(SDL_SCANCODE_1))
	{
		meshType = (MeshType)(meshType + 1);
	}

	if (meshType >= MT_SIZE)
	{
		meshType = MT_CUBE;
	}

	if (input_isKeyDown(SDL_SCANCODE_TAB))
	{
		input_toggleGrab();
	}

	
	if (input_isGrab())
	{
		camera.update(delta);
	}
	

	pulseTime += delta * 0.25f;

	if (pulseTime >= 1.0f)
	{
		pulseTime = 0.0f;
		index = index2;
		index2 = getColorIndex2();
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
		glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));

	glClearColor(TO_FLOAT_C(0), TO_FLOAT_C(191), TO_FLOAT_C(255), 1.0F);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	progTerrain.bind();
	progTerrain.setProjection(camera.toProjMatrix());
	progTerrain.setView(camera.toViewMatrix());
	progTerrain.setModel(model);

	terrain.render(progTerrain);

	progTerrain.unbind();

	progMain.bind();

	progMain.setProjection(camera.toProjMatrix());
	progMain.setView(camera.toViewMatrix());

	glm::vec3 color = (1.0f - pulseTime) * colors[index] + pulseTime * colors[index2];

	progMain.setColor(color);

	renderMesh(glm::vec3(0.0f, 0.0f, -5.0f));
	renderMesh(glm::vec3(0.0f, 0.0f, 5.0f));
	renderMesh(glm::vec3(0.0f, -5.0f, 0.0f));
	renderMesh(glm::vec3(0.0f, 5.0f, 0.0f));
	renderMesh(glm::vec3(-5.0f, 0.0f, 0.0f));
	renderMesh(glm::vec3(5.0f, 0.0f, 0.0f));

	progMain.unbind();
}

void game_release()
{
	terrain.release();

	monkey.release();
	cylinder.release();
	sphere.release();
	cube.release();
	cubeMap.release();
	texExample.release();
	progTerrain.release();
	progMain.release();
}

void renderMesh(const glm::vec3& pos)
{
	glm::mat4 model =
		glm::translate(glm::mat4(1.0f), pos) *
		glm::rotate(glm::mat4(1.0f), glm::radians(rot), glm::vec3(1.0f, 1.0f, 1.0f));

	progMain.setModel(model);

	progMain.bindTex0(texExample);
	progMain.bindCube0(cubeMap);

	switch (meshType)
	{
	case MT_CUBE:
		cube.render(progMain);
		break;
	case MT_SPHERE:
		sphere.render(progMain);
		break;
	case MT_CYLINDER:
		cylinder.render(progMain);
		break;
	case MT_MONKEY:
		monkey.render(progMain);
		break;
	}

	progMain.unbindCube0(cubeMap);
	progMain.unbindTex0(texExample);

}

uint32_t getColorIndex()
{
	return rand() % colors.size();
}

uint32_t getColorIndex2()
{
	uint32_t value = getColorIndex();

	if (value == index)
	{
		return getColorIndex2();
	}

	return value;
}