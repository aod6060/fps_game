#include "sys.h"


void Camera::init(const glm::vec3& pos, const glm::vec2& rot)
{
	this->setPos(pos);
	this->setRot(rot);

	this->rotSpeed = 128.0f;
}

void Camera::update(float delta)
{
	glm::vec2 mc;

	input_mousePosition(mc);

	rot.y += this->rotSpeed * mc.x * ((delta - 0.001f < 0) ? 0.001f : delta);
	rot.x += this->rotSpeed * mc.y * ((delta - 0.001f < 0) ? 0.001f : delta);

	//std::cout << rot.x << ", " << rot.y << std::endl;

	if (rot.x < -90.0f)
	{
		rot.x = -90.0f;
	}

	if (rot.x > 90.0f)
	{
		rot.x = 90.0f;
	}

	if (rot.y <= -360.0f)
	{
		rot.y += 360.0f;
	}

	if (rot.y >= 360.0f)
	{
		rot.y -= 360.0f;
	}

	float yrad = glm::radians(rot.y);

	float sp = this->walkSpeed;

	if (input_isKeyPress(SDL_SCANCODE_E))
	{
		sp *= 3.0f;
	}

	if (input_isKeyPress(SDL_SCANCODE_W))
	{
		pos.x += sp * glm::sin(yrad) * delta;
		pos.z -= sp * glm::cos(yrad) * delta;
	}

	if (input_isKeyPress(SDL_SCANCODE_S))
	{
		pos.x -= sp * glm::sin(yrad) * delta;
		pos.z += sp * glm::cos(yrad) * delta;
	}

	if (input_isKeyPress(SDL_SCANCODE_A))
	{
		pos.x -= sp * glm::cos(yrad) * delta;
		pos.z -= sp * glm::sin(yrad) * delta;
	}

	if (input_isKeyPress(SDL_SCANCODE_D))
	{
		pos.x += sp * glm::cos(yrad) * delta;
		pos.z += sp * glm::sin(yrad) * delta;
	}

	if (input_isKeyPress(SDL_SCANCODE_LSHIFT))
	{
		pos.y -= sp * delta;
	}

	if (input_isKeyPress(SDL_SCANCODE_SPACE))
	{
		pos.y += sp * delta;
	}
}

glm::mat4 Camera::toViewMatrix()
{
	return
		glm::rotate(glm::mat4(1.0f), glm::radians(rot.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(rot.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::translate(glm::mat4(1.0f), -this->pos);
}

glm::mat4 Camera::toProjMatrix()
{
	return glm::perspective(
		glm::radians(this->fov),
		(float)app_getWidth() / (float)app_getHeight(),
		0.1f,
		1024.0f
	);
}

glm::vec3 Camera::getPos()
{
	return this->pos;
}

void Camera::setPos(const glm::vec3& pos)
{
	this->pos = pos;
}

glm::vec2 Camera::getRot()
{
	return this->rot;
}

void Camera::setRot(const glm::vec2& rot)
{
	this->rot = rot;
}

float Camera::getFOV()
{
	return this->fov;
}

void Camera::setFOV(float fov)
{
	this->fov = fov;
}

float Camera::getRotSpeed()
{
	return this->rotSpeed;
}

void Camera::setRotSpeed(float rotSpeed)
{
	this->rotSpeed = rotSpeed;
}