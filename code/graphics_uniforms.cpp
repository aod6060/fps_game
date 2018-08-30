#include "sys.h"


void Uniforms::init(Program* prog)
{
	this->prog = prog;
}

void Uniforms::release()
{
	this->prog = nullptr;
}

// Create Uniform
void Uniforms::create(std::string name)
{
	this->uniforms[name] = glGetUniformLocation(this->prog->getID(), name.c_str());
}

// Integer
void Uniforms::set1i(std::string name, int x)
{
	glUniform1i(this->uniforms[name], x);
}

void Uniforms::set2i(std::string name, int x, int y)
{
	glUniform2i(this->uniforms[name], x, y);
}

void Uniforms::set2i(std::string name, const glm::ivec2& v)
{
	this->set2i(name, v.x, v.y);
}

void Uniforms::set3i(std::string name, int x, int y, int z)
{
	glUniform3i(this->uniforms[name], x, y, z);
}

void Uniforms::set3i(std::string name, const glm::ivec3& v)
{
	this->set3i(name, v.x, v.y, v.z);
}

void Uniforms::set4i(std::string name, int x, int y, int z, int w)
{
	glUniform4i(this->uniforms[name], x, y, z, w);
}

void Uniforms::set4i(std::string name, const glm::ivec4& v)
{
	this->set4i(name, v.x, v.y, v.z, v.w);
}

// Float
void Uniforms::set1f(std::string name, float x)
{
	glUniform1f(this->uniforms[name], x);
}

void Uniforms::set2f(std::string name, float x, float y)
{
	glUniform2f(this->uniforms[name], x, y);
}

void Uniforms::set2f(std::string name, const glm::vec2& v)
{
	set2f(name, v.x, v.y);
}

void Uniforms::set3f(std::string name, float x, float y, float z)
{
	glUniform3f(this->uniforms[name], x, y, z);
}

void Uniforms::set3f(std::string name, const glm::vec3& v)
{
	set3f(name, v.x, v.y, v.z);
}

void Uniforms::set4f(std::string name, float x, float y, float z, float w)
{
	glUniform4f(this->uniforms[name], x, y, z, w);
}

void Uniforms::set4f(std::string name, const glm::vec4& v)
{
	set4f(name, v.x, v.y, v.z, v.w);
}

// Matrices
void Uniforms::setMat2(std::string name, const glm::mat2& m)
{
	glUniformMatrix2fv(this->uniforms[name], 1, GL_FALSE, &m[0][0]);
}

void Uniforms::setMat3(std::string name, const glm::mat3& m)
{
	glUniformMatrix3fv(this->uniforms[name], 1, GL_FALSE, &m[0][0]);
}

void Uniforms::setMat4(std::string name, const glm::mat4& m)
{
	glUniformMatrix4fv(this->uniforms[name], 1, GL_FALSE, &m[0][0]);
}
