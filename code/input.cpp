#include "sys.h"



// Keyboard
static InputState keys[SDL_NUM_SCANCODES];

// Mouse Button
static InputState mouseB[MBT_SIZE];

// Mouse Positoin
static glm::vec2 pos;

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
	pos = glm::vec2(0, 0);
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
		pos.x = 0;
		pos.y = 0;
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
	v = pos;
}

void input_setGrab(bool b)
{
	SDL_SetRelativeMouseMode((b) ? SDL_TRUE : SDL_FALSE);
}

bool input_isGrab()
{
	return SDL_GetRelativeMouseMode() ? true : false;
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
	if (input_isGrab())
	{
		pos.x = mt.xrel;
		pos.y = mt.yrel;
	}
	else
	{
		pos.x = mt.x;
		pos.y = mt.y;
	}
}