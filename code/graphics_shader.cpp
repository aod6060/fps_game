#include "sys.h"


std::string Shader::getContent(std::string path)
{
	std::ifstream in(path);
	std::stringstream ss;
	std::string temp;
	while (std::getline(in, temp))
	{
		ss << temp << std::endl;
	}
	in.close();
	return ss.str();
}

void Shader::printShaderLog()
{
	int len = 0;
	char log[1024];
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
	if (len > 0)
	{
		glGetShaderInfoLog(id, len, 0, log);
		std::cout << log << std::endl;
	}
}

void Shader::create(GLenum type, std::string path)
{
	this->id = glCreateShader(type);
	std::string src = this->getContent(path);
	const char* csrc = src.c_str();
	glShaderSource(id, 1, &csrc, 0);
	glCompileShader(id);
	this->printShaderLog();
}

uint32_t Shader::getID()
{
	return id;
}

void Shader::release()
{
	glDeleteShader(this->id);
}
