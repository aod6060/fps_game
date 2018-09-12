#include "game.h"

// Program Manager
static ProgramWrapperMain progMain;
static ProgramWrapperTerrain progTerrain;

//static BillboardManager billboardManager;
static BillboardManagerTest billboardManager;

// Terrain
//static Terrain terrain;
static TerrainProcedural terrain;

// Terrain Data
//static TerrainData terrainData;

// Mesh
static Mesh plane;
glm::vec3 planePos;

// Texture
static Texture2D happy;

// Camera
static Camera camera;

// Water
static Mesh waterMesh;
static Texture2D waterTex;

// Mini map
static MiniMap miniMap;

void drawMesh(
	Mesh& mesh, 
	Texture2D& tex, 
	const glm::vec3& pos, 
	const glm::vec3& scale);

void game_init()
{
	glEnable(GL_DEPTH_TEST);

	progMain.init();
	progTerrain.init();

	// Terrain
	terrain.init();

	// Camera

	input_setGrab(true);

	//terrainData.init();

	plane.init("data/meshes/plain_test.blend");

	happy.init("data/textures/happyface.png");

	// Fake water
	waterMesh.init("data/meshes/water.blend");
	waterTex.init("data/textures/fake_water.png");


	billboardManager.init();

	int i = 0;

	uint32_t amount = rand() % 1024 + 1;

	while(i < amount)
	{
		int32_t size = terrain.getData()->size;

		float _x = (rand() % size) - (size / 2);
		float _z = (rand() % size) - (size / 2);

		if (
			terrain.getTerrainType(_x, _z) == TT_SNOW)
		{
			float _y = terrain.getHeight(_x, _z);

			glm::vec3 pos = glm::vec3(_x, _y, _z);

			billboardManager.addBillboard(pos, glm::vec2(10, 10), rand() % billboardManager.maxTreeSize);

			++i;
		}
		else
		{
			continue;
		}
	}

	bool look = false;

	while (!look)
	{
		int32_t size = terrain.getData()->size;

		float _x = (rand() % size) - (size / 2);
		float _z = (rand() % size) - (size / 2);

		if (terrain.getTerrainType(_x, _z) == TT_BEACH)
		{
			camera.init(
				glm::vec3(_x, terrain.getHeight(_x, _z) + 5.5f, _z),
				glm::vec2(0.0f, 0.0f)
			);
			look = true;
			continue;
		}

		//if(terrain.get)
		//camera.init(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f));
	}

	look = false;

	while (!look)
	{
		int32_t size = terrain.getData()->size;

		float _x = (rand() % size) - (size / 2);
		float _z = (rand() % size) - (size / 2);

		if (terrain.getTerrainType(_x, _z) == TT_BEACH)
		{
			/*
			camera.init(
				glm::vec3(_x, terrain.getHeight(_x, _z) + 5.5f, _z),
				glm::vec2(0.0f, 0.0f)
			);
			*/

			planePos = glm::vec3(_x, terrain.getHeight(_x, _z) + 2.0f, _z);

			look = true;
			continue;
		}

		//if(terrain.get)
		//camera.init(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f));
	}

	miniMap.init();
}

void game_update(float delta)
{
	if (input_isKeyDown(SDL_SCANCODE_TAB))
	{
		input_toggleGrab();
	}

	if (input_isKeyDown(SDL_SCANCODE_1))
	{
		miniMap.toggleEnabled();
	}

	if (input_isGrab())
	{
		camera.update(delta);
	}

	miniMap.update(camera, terrain.getData());

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

	progMain.bind();

	progMain.setProjection(camera.toProjMatrix());
	progMain.setView(camera.toViewMatrix());

	drawMesh(
		waterMesh, 
		waterTex, 
		glm::vec3(0.0f, 1.0f, 0.0f) * terrain.getScale() * 0.251f,
		glm::vec3(2.0f, 0.0f, 2.0f));

	drawMesh(
		plane,
		terrain.getData()->biomesMap,
		planePos,
		glm::vec3(1.0f, 1.0f, 0.0f));
	progMain.unbind();

	progTerrain.bind();
	progTerrain.setProjection(camera.toProjMatrix());
	progTerrain.setView(camera.toViewMatrix());
	progTerrain.setModel(model);

	progTerrain.drawTerrain(terrain);

	progTerrain.unbind();

	billboardManager.render(camera.toProjMatrix(), camera.toViewMatrix(), camera.getPos());

	//glClear(GL_DEPTH_BUFFER_BIT);
	miniMap.render(terrain.getData());

}

void game_release()
{
	miniMap.release();

	billboardManager.release();

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
	const glm::vec3& pos,
	const glm::vec3& scale)
{
	progMain.setModel(
		glm::translate(glm::mat4(1.0f), pos) *
		glm::scale(glm::mat4(1.0f), scale));

	progMain.drawMesh(mesh, tex);
}