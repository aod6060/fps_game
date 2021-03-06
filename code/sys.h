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

#include <LUA/lua.hpp>

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
SDL_Window* app_getWindow();

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
	bool isStatic;
public:
	void add(float x);
	void add(float x, float y);
	void add(const glm::vec2& v);
	void add(float x, float y, float z);
	void add(const glm::vec3& v);
	void add(float x, float y, float z, float w);
	void add(const glm::vec4& v);
	void add(const glm::mat4& m);
	void addAll(std::vector<float>& v);
	void addAll(std::vector<glm::vec2>& v);
	void addAll(std::vector<glm::vec3>& v);
	void addAll(std::vector<glm::vec4>& v);
	void addAll(std::vector<glm::mat4>& m);
	void clear();
	void create(bool isStatic = false);
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
	std::map<std::string, uint32_t>* getAttr();

	void enable(std::string name);
	void enable(std::string name, uint32_t index);
	void disable(std::string name);
	void disable(std::string name, uint32_t index);

	void pointer(std::string name, uint32_t size, GLenum type);
	void pointer(std::string name, uint32_t index, uint32_t size, GLenum type, uint32_t offset, const void* pointer);
	void divisor(std::string name, uint32_t index, uint32_t divisor);
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

class IProgramWrapper
{
public:
	virtual void init() = 0;
	virtual void bind() = 0;
	virtual void unbind() = 0;
	virtual void release() = 0;

	virtual Program* getProgram() = 0;
};

// Texture2D
class Texture2D
{
private:
	uint32_t id = 0;
	uint32_t width = 1;
	uint32_t height = 1;
public:

	void init(std::string path);

	void init(void* pixel, int width, int height, GLenum min = GL_NEAREST, GLenum mag = GL_NEAREST);

	void init(int width, int height, GLenum min = GL_NEAREST, GLenum mag = GL_NEAREST);
	
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

class RenderBuffer
{
private:
	uint32_t id = 0;
	uint32_t width = 1;
	uint32_t height = 1;
public:
	void init(uint32_t width, uint32_t height);

	uint32_t getID();

	uint32_t getWidth();
	uint32_t getHeight();

	void bind();

	void unbind();

	void release();
};

class Framebuffer
{
private:
	uint32_t id = 0;


public:

	void init();

	void bind();

	void unbind();

	void release();

	void checkErrors();

	void addTexture(GLenum attach, GLenum texType, Texture2D& texture);

	void addRenderBuffer(GLenum attach, RenderBuffer& renderBuffer);
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

	void init(std::string path, bool isStatic = false);

	//void render(ProgramWrapperMain& prog);

	void release();

	VertexBuffer* getVertexBuffer();
	VertexBuffer* getTexCoordBuffer();
	VertexBuffer* getNormalBuffer();

	IndexBuffer* getIndexBuffer();

};

// Camera
class Camera
{
private:
	glm::vec3 pos;
	glm::vec2 rot;
	float fov = 45.0f;
	float rotSpeed = 32.0f;
	float walkSpeed = 32.0f;
public:
	void init(const glm::vec3& pos, const glm::vec2& rot);

	void update(float delta);

	glm::mat4 toViewMatrix();
	glm::mat4 toProjMatrix();

	glm::vec3 getPos();
	void setPos(const glm::vec3& pos);

	glm::vec2 getRot();
	void setRot(const glm::vec2& rot);

	float getFOV();
	void setFOV(float fov);

	float getRotSpeed();
	void setRotSpeed(float rotSpeed);
};

// Terrain
class Terrain
{
private:
	VertexBuffer vBuf;
	VertexBuffer tBuf;
	VertexBuffer nBuf;

	IndexBuffer iBuf;

	Texture2D blendMap;
	Texture2D channelBlack;
	Texture2D channelRed;
	Texture2D channelGreen;
	Texture2D channelBlue;

	std::vector<float> heights;

	uint32_t width = 1;
	uint32_t height = 1;

	float scale = 32.0f;

	float tiling = 1.0f;

public:

	void init(std::string path);

	//void render(ProgramWrapperTerrain& prog);

	void release();

	void setScale(float scale);

	float getScale();

	void setTiling(float tiling);

	float getTiling();

	Texture2D* getBlendMap();
	Texture2D* getChannelBlack();
	Texture2D* getChannelRed();
	Texture2D* getChannelGreen();
	Texture2D* getChannelBlue();

	VertexBuffer* getVertexBuffer();
	VertexBuffer* getTexCoordBuffer();
	VertexBuffer* getNormalBuffer();

	IndexBuffer* getIndexBuffer();

};

enum TerrainType
{
	TT_OCEAN = 0,
	TT_BEACH,
	TT_GRASS,
	TT_SNOW
};

// Terrain Data
struct TerrainData
{
	// Terrain Data
	float size = 512.0f;

	// Height Map Generation
	float seed = 0.0f;

	float hm_wave1 = 128.0f;
	float hm_weight1 = 0.5f;

	float hm_wave2 = 64.0f;
	float hm_weight2 = 0.35f;

	float hm_wave3 = 16.0f;
	float hm_weight3 = 0.15f;

	std::vector<float> elevation;
	std::vector<float> mask;
	std::vector<float> maskedElevation;
	std::vector<glm::vec2> moister;
	std::vector<glm::vec3> blend;
	std::vector<glm::vec3> biomes;
	std::vector<TerrainType> terrainType;

	SDL_Surface* elevationSurf = nullptr;
	SDL_Surface* maskSurf = nullptr;
	SDL_Surface* maskedElevationSurf = nullptr;
	SDL_Surface* moisterSurface = nullptr;
	SDL_Surface* blendSurf = nullptr;
	SDL_Surface* biomesSurf = nullptr;

	Texture2D elevationTex;
	Texture2D maskTex;
	Texture2D maskedElevationTex;
	Texture2D moisterTex;
	Texture2D blendMap;
	Texture2D biomesMap;

	void init();

	void release();

	float toMask(float x, float y, float radius, uint32_t r);

};

// Terrain Procedurally Generated Terrain
class TerrainProcedural
{
private:
	TerrainData data;

	Texture2D blendMap;

	Texture2D channelBlack;
	Texture2D channelRed;
	Texture2D channelGreen;
	Texture2D channelBlue;

	VertexBuffer vBuf;
	VertexBuffer tBuf;
	VertexBuffer nBuf;

	IndexBuffer iBuf;

	float scale = 32.0f;
	float tiling = 1.0f;
public:

	void init();

	//void render(ProgramWrapperTerrain& prog);

	void release();

	void setScale(float scale);

	float getScale();

	void setTiling(float tiling);

	float getTiling();

	TerrainType getTerrainType(float x, float z);
	float getHeight(float x, float z);

	glm::vec3 getRandomLocation(TerrainType type);

	TerrainData* getData();

	Texture2D* getBlendMap();
	Texture2D* getChannelBlack();
	Texture2D* getChannelRed();
	Texture2D* getChannelGreen();
	Texture2D* getChannelBlue();

	VertexBuffer* getVertexBuffer();
	VertexBuffer* getTexCoordBuffer();
	VertexBuffer* getNormalBuffer();

	IndexBuffer* getIndexBuffer();
};

// ProgramWrapperMain
class ProgramWrapperMain : public IProgramWrapper
{
private:
	Shader vertex;
	Shader fragment;

	Program program;
public:
	virtual void init();
	virtual void bind();
	virtual void unbind();
	virtual void release();

	virtual Program* getProgram();

	void bindAttribute();
	void unbindAttribute();

	void verticesPointer();
	void texCoordsPointer();
	void normalsPointer();

	void drawArrays(GLenum type, uint32_t count);
	void drawElements(GLenum type, uint32_t count);

	void setProjection(const glm::mat4& m);
	void setView(const glm::mat4& m);
	void setModel(const glm::mat4& m);

	void bindTex0(Texture2D& tex);
	void unbindTex0(Texture2D& tex);

	void drawMesh(Mesh& mesh, Texture2D& texture);
};

class ProgramWrapperTerrain : public IProgramWrapper
{
private:
	Shader vertex;
	Shader fragment;

	Program program;
public:

	virtual void init();
	virtual void bind();
	virtual void unbind();
	virtual void release();

	virtual Program* getProgram();

	void bindAttribute();
	void unbindAttribute();

	void verticesPointer();
	void texCoordsPointer();

	void drawArrays(GLenum type, int size);
	void drawElements(GLenum type, int size);

	void setProjection(const glm::mat4& proj);
	void setView(const glm::mat4& view);
	void setModel(const glm::mat4& model);

	void setTiling(float tiling);

	void bindBlendMap(Texture2D& blendMap);
	void bindChannelBlack(Texture2D& channelBlack);
	void bindChannelRed(Texture2D& channelRed);
	void bindChannelGreen(Texture2D& channelGreen);
	void bindChannelBlue(Texture2D& channelBlue);

	void unbindBlendMap(Texture2D& blendMap);
	void unbindChannelBlack(Texture2D& channelBlack);
	void unbindChannelRed(Texture2D& channelRed);
	void unbindChannelGreen(Texture2D& channelGreen);
	void unbindChannelBlue(Texture2D& channelBlue);

	void drawTerrain(Terrain& terrain);
	void drawTerrain(TerrainProcedural& terrain);
};

class ProgramWrapperBlur : public IProgramWrapper
{
private:
	Shader vertex;
	Shader fragment;

	Program program;
public:

	virtual void init();
	virtual void bind();
	virtual void unbind();
	virtual void release();

	virtual Program* getProgram();

	void bindAttribute();
	void unbindAttribute();

	void verticesPointer();
	void texCoordsPointer();

	void drawArrays(GLenum type, int size);
	void drawElements(GLenum type, int size);

	void setProjection(const glm::mat4& proj);
	void setView(const glm::mat4& view);
	void setModel(const glm::mat4& model);

	void setSize(float size);

	void bindInputTex(Texture2D& tex);
	void unbindInputTex(Texture2D& tex);

	void drawMesh(Mesh& mesh, Texture2D& tex);

	void drawBuffer(
		VertexBuffer& vBuf, 
		VertexBuffer& tBuf, 
		IndexBuffer& iBuf,
		Texture2D& tex);

};

class ProgramWrapperBillboard : public IProgramWrapper
{
private:
	Shader vertex;
	Shader fragment;
	Program program;
	VertexBuffer vBuf;
	VertexBuffer tBuf;
	IndexBuffer iBuf;
public:
	virtual void init();
	virtual void bind();
	virtual void unbind();
	virtual void release();
	virtual Program* getProgram();
	void bindAttribute();
	void unbindAttribute();
	void verticesPointer();
	void texCoordsPointer();
	void drawElements(GLenum type, int size);
	void setProjection(const glm::mat4& proj);
	void setView(const glm::mat4& view);
	void setModel(const glm::mat4& model);
	void setCameraPos(const glm::vec3& cameraPos);
	void bindBillboard(Texture2D& billboard);
	void unbindBillboard(Texture2D& billboard);
	void draw(Texture2D& tex);
};

struct Billboard
{
	glm::vec3 pos;
	glm::vec2 scale;
	uint32_t t = 0;
};

class BillboardManager
{
private:
	ProgramWrapperBillboard prog;

	std::vector<Billboard> billboards;

	Texture2D tree;
	Texture2D tree2;
	Texture2D tree3;
	Texture2D tree4;

public:

	void init();

	void addBillboard(
		glm::vec3 pos, 
		glm::vec2 scale, 
		uint32_t tex);

	void render(
		const glm::mat4& proj, 
		const glm::mat4& view,
		const glm::vec3& cameraPos);

	void release();

	uint32_t maxTreeSize = 4;
};

struct BillboardTest
{
	glm::vec3 pos;
	glm::vec2 scale;
};

class BillboardManagerTest
{
private:

	Shader vertex;
	Shader fragment;

	Program program;

	Texture2D tree;
	Texture2D tree2;
	Texture2D tree3;
	Texture2D tree4;

	std::vector<BillboardTest> tree_b;
	std::vector<BillboardTest> tree_b2;
	std::vector<BillboardTest> tree_b3;
	std::vector<BillboardTest> tree_b4;

	// Buffers
	VertexBuffer vBuf;
	VertexBuffer tBuf;
	IndexBuffer iBuf;

	// Instance Arrays
	VertexBuffer matrixBuffer;

	void render(Texture2D& tex, uint32_t size);

	void convertBillboardToMatrix(
		std::vector<glm::mat4>& ms, 
		std::vector<BillboardTest>& bs, 
		glm::vec3 cameraPos);
public:

	void init();

	void release();

	void addBillboard(
		glm::vec3 pos,
		glm::vec2 scale,
		uint32_t tex
	);

	void render(
		const glm::mat4& proj,
		const glm::mat4& view,
		const glm::vec3& cameraPos);

	uint32_t maxTreeSize = 4;
};

class GrassRendering
{
private:
	Shader vertex;
	Shader fragment;

	Program program;

	Mesh grass;

	Texture2D grass1;
	Texture2D grass2;
	Texture2D grass3;
	Texture2D grass4;
	Texture2D grass5;
	Texture2D grass6;
	Texture2D grass7;
	Texture2D grass8;
	Texture2D grass9;
	Texture2D grass10;

	VertexBuffer grass1_buf;
	VertexBuffer grass2_buf;
	VertexBuffer grass3_buf;
	VertexBuffer grass4_buf;
	VertexBuffer grass5_buf;
	VertexBuffer grass6_buf;
	VertexBuffer grass7_buf;
	VertexBuffer grass8_buf;
	VertexBuffer grass9_buf;
	VertexBuffer grass10_buf;

	std::vector<std::string> grass_path;


	void render(Texture2D& tex, VertexBuffer& buf);

	void loadGrassPath();

	std::string getGrassPath();

public:

	void add(int type, glm::vec3 pos);

	void upload();

	void init(TerrainProcedural& prog);
	
	void update(float delta);

	void render(Camera& camera);

	void release();

};

class MiniMap
{
private:
	Shader vertex;
	Shader fragment;

	Program program;

	VertexBuffer vBuf;
	VertexBuffer vBuf2;
	VertexBuffer tBuf;
	IndexBuffer iBuf;

	Texture2D player;

	glm::vec2 pos;
	glm::vec2 cameraPos;
	float zrot = 0.0f;

	bool enabled = false;

	void renderQuad(Texture2D& quad, const glm::mat4& model);
	void renderQuad2(Texture2D& quad, const glm::mat4& model);
public:

	void init();

	void update(Camera& camera, TerrainData* data);

	void render(TerrainData* data);

	void release();

	bool isEnabled();

	void setEnabled(bool enabled);

	void toggleEnabled();

};

class TestBillboard
{
private:
	Shader vertex;
	Shader fragment;

	Program program;

	VertexBuffer vbuf;
	VertexBuffer tbuf;

	IndexBuffer ibuf;

	glm::vec3 location;

	Texture2D tex;

	std::vector<std::string> ppath;

	void loadParticles();

	std::string getParticles();

public:

	void init(const glm::vec3& location);

	void render(Camera& camera);

	void release();
};

struct Particle
{
	glm::vec3 position;
	glm::vec3 velocity;
	float life;
};

class ParticleSystem
{
private:
	Shader vertex;
	Shader fragment;

	Program program;

	VertexBuffer vbuf;
	VertexBuffer tbuf;

	IndexBuffer ibuf;

	Mesh monkey;

	VertexBuffer matrix_buffer;

	Texture2D tex;

	std::vector<Particle> particles;

	float maxLife = 1.0f;

	glm::vec3 position;
	glm::vec3 velocity;

	glm::vec3 gravity;

public:

	void init(
		const glm::vec3& position,
		const glm::vec3& velocity,
		const glm::vec3& gravity,
		float maxLife);

	void update(float delta, Camera& camera, TerrainProcedural& terain);

	void render(Camera& camera);

	void release();

	void setPosition(const glm::vec3& position);

	glm::vec3 getPosition();

	void setMaxLife(float maxLife);

	float getMaxLife();

};