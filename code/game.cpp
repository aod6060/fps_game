#include "game.h"

// Program Manager
static ProgramWrapperMain progMain;
static ProgramWrapperTerrain progTerrain;

// Terrain
//static Terrain terrain;
static TerrainProcedural terrain;

// Terrain Data
//static TerrainData terrainData;

// Mesh
static Mesh plane;

// Texture
static Texture2D happy;

// Camera
static Camera camera;

// Water
static Mesh waterMesh;
static Texture2D waterTex;

void drawMesh(Mesh& mesh, Texture2D& tex, const glm::vec3& pos);

void game_init()
{
	glEnable(GL_DEPTH_TEST);

	progMain.init();
	progTerrain.init();

	// Terrain
	terrain.init();

	// Camera
	camera.init(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f));

	input_setGrab(true);

	//terrainData.init();

	plane.init("data/meshes/plain_test.blend");

	happy.init("data/textures/happyface.png");

	// Fake water
	waterMesh.init("data/meshes/water.blend");
	waterTex.init("data/textures/fake_water.png");

}

void game_update(float delta)
{
	if (input_isKeyDown(SDL_SCANCODE_TAB))
	{
		input_toggleGrab();
	}

	
	if (input_isGrab())
	{
		camera.update(delta);
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

	glViewport(0, 0, app_getWidth(), app_getHeight());
	glClearColor(TO_FLOAT_C(0), TO_FLOAT_C(191), TO_FLOAT_C(255), 1.0F);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	progTerrain.bind();
	progTerrain.setProjection(camera.toProjMatrix());
	progTerrain.setView(camera.toViewMatrix());
	progTerrain.setModel(model);

	progTerrain.drawTerrain(terrain);

	progTerrain.unbind();

	progMain.bind();

	progMain.setProjection(camera.toProjMatrix());
	progMain.setView(camera.toViewMatrix());

	drawMesh(waterMesh, waterTex, glm::vec3(0.0f, 1.0f, 0.0f) * terrain.getScale() * 0.26f);

	drawMesh(
		plane,
		terrain.getData()->biomesMap,
		glm::vec3(0.0f, 0.0f, -5.0f));

	progMain.unbind();
}

void game_release()
{

	waterTex.release();
	waterMesh.release();

	happy.release();

	plane.release();

	terrain.release();

	progTerrain.release();
	progMain.release();
}

void drawMesh(
	Mesh& mesh, 
	Texture2D& tex, 
	const glm::vec3& pos)
{
	progMain.setModel(glm::translate(glm::mat4(1.0f), pos));
	progMain.drawMesh(mesh, tex);
}