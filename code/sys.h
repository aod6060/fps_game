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
#include <SDL/SDL_ttf.h>

#include <freeimage/FreeImage.h>

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

// Vertex Buffer
class VertexBuffer
{
private:
	uint32_t id;
	std::vector<float> list;
public:
	void add(float x);
	void add(float x, float y);
	void add(const glm::vec2& v);
	void add(float x, float y, float z);
	void add(const glm::vec3& v);
	void add(float x, float y, float z, float w);
	void add(const glm::vec4& v);
	void addAll(std::vector<float>& v);
	void addAll(std::vector<glm::vec2>& v);
	void addAll(std::vector<glm::vec3>& v);
	void addAll(std::vector<glm::vec4>& v);
	void clear();
	void create();
	void upload();
	void bind();
	void unbind();
	void release();
	uint32_t size();
};

// Index Buffer
class IndexBuffer
{
private:
	uint32_t id;
	std::vector<uint32_t> list;
public:
	void add(uint32_t x);
	void add(uint32_t x, uint32_t y);
	void add(uint32_t x, uint32_t y, uint32_t z);
	void add(uint32_t x, uint32_t y, uint32_t z, uint32_t w);
	void addAll(std::vector<uint32_t>& v);
	void addAll(std::vector<glm::ivec2>& v);
	void addAll(std::vector<glm::ivec3>& v);
	void addAll(std::vector<glm::ivec4>& v);
	void clear();
	void create();
	void upload();
	void bind();
	void unbind();
	void release();
	uint32_t size();
};

// Attributes
class Attributes
{
private:
	uint32_t id;
	std::map <std::string, uint32_t> attributes;
public:
	void set(std::string name, uint32_t id);
	void enable(std::string name);
	void disable(std::string name);
	void pointer(std::string name, uint32_t size, GLenum type);
	void create();
	void bind();
	void unbind();
	void release();
	uint32_t getID();
};

class Program;

// Uniforms
class Uniforms
{
private:
	Program* prog = nullptr;
	std::map<std::string, uint32_t> uniforms;
public:
	void init(Program* prog);
	void release();

	void create(std::string name);

	void set1i(std::string name, int x);
	void set2i(std::string name, int x, int y);
	void set2i(std::string name, const glm::ivec2& v);
	void set3i(std::string name, int x, int y, int z);
	void set3i(std::string name, const glm::ivec3& v);
	void set4i(std::string name, int x, int y, int z, int w);
	void set4i(std::string name, const glm::ivec4& v);

	void set1f(std::string name, float x);
	void set2f(std::string name, float x, float y);
	void set2f(std::string name, const glm::vec2& v);
	void set3f(std::string name, float x, float y, float z);
	void set3f(std::string name, const glm::vec3& v);
	void set4f(std::string name, float x, float y, float z, float w);
	void set4f(std::string name, const glm::vec4& v);

	void setMat2(std::string name, const glm::mat2& m);
	void setMat3(std::string name, const glm::mat3& m);
	void setMat4(std::string name, const glm::mat4& m);
};

// Shader
class Shader
{
private:
	uint32_t id = 0;

	std::string getContent(std::string path);
	void printShaderLog();
public:

	void create(GLenum type, std::string path);
	uint32_t getID();
	void release();
};

// Program
class Program
{
private:
	uint32_t id = 0;
	std::vector<Shader*> shaders;
	Attributes attributes;
	Uniforms uniforms;

	void printProgramLog();
public:
	void addShader(Shader* shader);
	void clearShader();

	void create();
	void bind();
	void unbind();
	void release();

	Attributes* getAttr();
	Uniforms* getUniforms();
	uint32_t getID();
};

// Texture2D
class Texture2D
{
private:
	uint32_t id = 0;
	uint32_t width = 1;
	uint32_t height = 1;

	//std::vector<uint32_t> getContents(std::string path, uint32_t& w, uint32_t& h);
public:

	void init(std::string path);

	void bind(GLenum type);

	void unbind(GLenum type);

	void release();

	uint32_t getWidth();

	uint32_t getHeight();

	uint32_t getID();

};

// CubeMap
class CubeMap
{
private:
	uint32_t id;

	FIBITMAP* getContent(std::string path);

	std::string getContents(std::string path);

	void loadCubemapTextures(rapidjson::Value& textures);

public:

	void init(std::string path);

	void bind(GLenum type);

	void unbind(GLenum type);

	void release();

};


// Meshes
class Mesh
{
private:
	VertexBuffer vBuf;
	VertexBuffer tBuf;
	VertexBuffer nBuf;

	IndexBuffer iBuf;

public:

	void init(std::string path);

	void render(Program& prog);

	void release();
};