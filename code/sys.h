#pragma once


#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <limits>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <GL/glew.h>

#include <rapidjson/document.h>
#include <BP/btBulletDynamicsCommon.h>

// Enumerations
enum InputState
{
	IS_RELEASE = 0,
	IS_DOWN,
	IS_PRESS,
	IS_UP
};


enum MouseButtonType
{
	MBT_LEFT = 0,
	MBT_CENTER,
	MBT_RIGHT,
	MBT_SIZE
};


// Application
std::string app_getCaption();
uint32_t app_getWidth();
uint32_t app_getHeight();
void app_exit();

// Input
void input_init();
void input_event(SDL_Event& e);
void input_update();

// Keyboard
bool input_isKeyRelease(uint32_t key);
bool input_isKeyDown(uint32_t key);
bool input_isKeyPress(uint32_t key);
bool input_isKeyUp(uint32_t key);

// Mouse
bool input_isMouseRelease(MouseButtonType type);
bool input_isMouseDown(MouseButtonType type);
bool input_isMousePress(MouseButtonType type);
bool input_isMouseUp(MouseButtonType type);

void input_mousePosition(glm::vec2& v);

void input_setGrab(bool b);
bool input_isGrab();
void input_toggleGrab();


// Graphcis
