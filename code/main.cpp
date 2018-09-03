#include "game.h"



#define G_TIME 1.0f / 60.0f

static std::string g_cap = "FPS Test";
static uint32_t g_width = 800;
static uint32_t g_height = 600;
static bool g_running = true;

static SDL_Window* g_win = nullptr;
static SDL_GLContext g_context = nullptr;

int main(int argc, char** argv)
{
	srand(time(nullptr));

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	g_win = SDL_CreateWindow(
		g_cap.c_str(),
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		g_width,
		g_height,
		SDL_WINDOW_OPENGL
	);

	g_context = SDL_GL_CreateContext(g_win);

	glewInit();

	input_init();

	game_init();

	SDL_Event e;

	uint32_t pre = SDL_GetTicks();
	uint32_t next = 0;
	float delta = 0.0f;
	float time = 0.0f;

	while (g_running)
	{

		next = SDL_GetTicks();
		delta = (next - pre) / 1000.0f;
		pre = next;

		time += delta;

		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				g_running = false;
			}

			input_event(e);
		}

		
		game_update(delta);
		
		if (time >= G_TIME)
		{
			game_fixedUpdate();
		}

		game_render();


		if (time >= G_TIME)
		{
			time = 0.0f;
		}

		input_update();

		SDL_GL_SwapWindow(g_win);
	}

	game_release();

	SDL_GL_DeleteContext(g_context);
	SDL_DestroyWindow(g_win);

	SDL_Quit();

	return 0;
}

std::string app_getCaption()
{
	return g_cap;
}

uint32_t app_getWidth()
{
	return g_width;
}

uint32_t app_getHeight()
{
	return g_height;
}

void app_exit()
{
	g_running = false;
}

SDL_Window* app_getWindow()
{
	return g_win;
}