#include "sys.h"



// Keyboard
static InputState keys[SDL_NUM_SCANCODES];

// Mouse Button
static InputState mouseB[MBT_SIZE];

// Mouse Positoin
//static glm::vec2 pos;
static bool isGrab = false;
static int mx = 0;
static int my = 0;
static int preMX = 0;
static int preMY = 0;
static int mx_rel = 0;
static int my_rel = 0;

// Event Methdos
static void _onKeyDown(SDL_KeyboardEvent& key);
static void _onKeyUp(SDL_KeyboardEvent& key);
static void _onMouseButtonDown(SDL_MouseButtonEvent& mb);
static void _onMouseButtonUp(SDL_MouseButtonEvent& mb);
static void _onMouseMotion(SDL_MouseMotionEvent& mt);


// Input
void input_init()
{
	// Keyboard
	for (uint32_t i = 0; i < SDL_NUM_SCANCODES; i++)
	{
		keys[i] = IS_RELEASE;
	}
	// Mouse Buttons
	for (uint32_t i = 0; i < MBT_SIZE; i++)
	{
		mouseB[i] = IS_RELEASE;
	}
	// Position
}

void input_event(SDL_Event& e)
{
	switch (e.type)
	{
	case SDL_KEYDOWN:
		_onKeyDown(e.key);
		break;
	case SDL_KEYUP:
		_onKeyUp(e.key);
		break;
	case SDL_MOUSEBUTTONDOWN:
		_onMouseButtonDown(e.button);
		break;
	case SDL_MOUSEBUTTONUP:
		_onMouseButtonUp(e.button);
		break;
	case SDL_MOUSEMOTION:
		_onMouseMotion(e.motion);
		break;
	}
}

void input_update()
{
	// Keyboard
	for (uint32_t i = 0; i < SDL_NUM_SCANCODES; i++)
	{
		if (keys[i] == IS_DOWN)
		{
			keys[i] = IS_PRESS;
		}
		if (keys[i] == IS_UP)
		{
			keys[i] = IS_RELEASE;
		}
	}
	// Mouse Button
	for (uint32_t i = 0; i < MBT_SIZE; i++)
	{
		if (mouseB[i] == IS_DOWN)
		{
			mouseB[i] = IS_PRESS;
		}
		if (mouseB[i] == IS_UP)
		{
			mouseB[i] = IS_RELEASE;
		}
	}

	// Position
	if (input_isGrab())
	{
		//std::cout << "Relative Mode" << std::endl;

		// Mouse X Position
		if (mx == 0)
		{
			mx = app_getWidth() / 2;
			preMX = app_getWidth() / 2;
			SDL_WarpMouseInWindow(app_getWindow(), mx, my);
		}
		else if (mx == app_getWidth() - 1)
		{
			mx = app_getWidth() / 2;
			preMX = app_getWidth() / 2;
			SDL_WarpMouseInWindow(app_getWindow(), mx, my);
		}

		if (my == 0)
		{
			my = app_getHeight() / 2;
			preMY = app_getHeight() / 2;
			SDL_WarpMouseInWindow(app_getWindow(), mx, my);
		}
		else if (my == app_getHeight() - 1)
		{
			my = app_getHeight() / 2;
			preMY = app_getHeight() / 2;
			SDL_WarpMouseInWindow(app_getWindow(), mx, my);
		}

		mx_rel = mx - preMX;
		my_rel = my - preMY;

		preMX = mx;
		preMY = my;
	}
}

// Keyboard
bool input_isKeyRelease(uint32_t key)
{
	return keys[key] == IS_RELEASE;
}

bool input_isKeyDown(uint32_t key)
{
	return keys[key] == IS_DOWN;
}

bool input_isKeyPress(uint32_t key)
{
	return keys[key] == IS_PRESS;
}

bool input_isKeyUp(uint32_t key)
{
	return keys[key] == IS_UP;
}

// Mouse
bool input_isMouseRelease(MouseButtonType type)
{
	return keys[type] == IS_RELEASE;
}

bool input_isMouseDown(MouseButtonType type)
{
	return keys[type] == IS_RELEASE;
}

bool input_isMousePress(MouseButtonType type)
{
	return keys[type] == IS_RELEASE;
}

bool input_isMouseUp(MouseButtonType type)
{
	return keys[type] == IS_RELEASE;
}

void input_mousePosition(glm::vec2& v)
{
	if (input_isGrab())
	{
		v.x = mx_rel;
		v.y = my_rel;
	}
	else
	{
		v.x = mx;
		v.y = my;
	}
}

void input_setGrab(bool b)
{
	//SDL_SetRelativeMouseMode((b) ? SDL_TRUE : SDL_FALSE);
	isGrab = b;


	if (isGrab)
	{
		SDL_SetWindowGrab(app_getWindow(), SDL_TRUE);
		mx = app_getWidth() / 2;
		my = app_getHeight() / 2;
		preMX = mx;
		preMY = my;

		//SDL_WarpMouseGlobal(mx, my);

		SDL_WarpMouseInWindow(app_getWindow(), mx, my);

		SDL_ShowCursor(0);
	}
	else
	{
		SDL_SetWindowGrab(app_getWindow(), SDL_FALSE);
		SDL_ShowCursor(1);
	}
}

bool input_isGrab()
{
	//return SDL_GetRelativeMouseMode() ? true : false;
	return isGrab;
}

void input_toggleGrab()
{
	input_setGrab(!input_isGrab());
}

// Event Methdos
void _onKeyDown(SDL_KeyboardEvent& key)
{
	if (keys[key.keysym.scancode] == IS_RELEASE)
	{
		keys[key.keysym.scancode] = IS_DOWN;
	}
}

void _onKeyUp(SDL_KeyboardEvent& key)
{
	if (keys[key.keysym.scancode] == IS_PRESS)
	{
		keys[key.keysym.scancode] = IS_UP;
	}
}

void _onMouseButtonDown(SDL_MouseButtonEvent& mb)
{
	int i = mb.button - 1;
	if (mouseB[i] == IS_RELEASE)
	{
		mouseB[i] = IS_DOWN;
	}
}

void _onMouseButtonUp(SDL_MouseButtonEvent& mb)
{
	int i = mb.button - 1;
	if (mouseB[i] == IS_PRESS)
	{
		mouseB[i] = IS_UP;
	}
}

void _onMouseMotion(SDL_MouseMotionEvent& mt)
{
	mx = mt.x;
	my = mt.y;
}