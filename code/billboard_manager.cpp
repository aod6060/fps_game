#include "sys.h"


void BillboardManager::init()
{
	this->prog.init();
	this->tree.init("data/textures/test_tree.png");
	this->tree2.init("data/textures/test_tree2.png");
	this->tree3.init("data/textures/test_tree3.png");
	this->tree4.init("data/textures/test_tree4.png");
}

void BillboardManager::addBillboard(glm::vec3 pos, glm::vec2 scale, uint32_t tex)
{
	this->billboards.push_back({ pos, scale, tex });
}

float _toAngle(const glm::vec3& a, const glm::vec3& b)
{
	float la = glm::length(a);
	float lb = glm::length(b);

	float d = glm::dot(a, b);

	return glm::acos(d / (la * lb));

}

void BillboardManager::render(
	const glm::mat4& proj, 
	const glm::mat4& view,
	const glm::vec3& cameraPos)
{
	prog.bind();

	prog.setProjection(proj);
	prog.setView(view);
	prog.setCameraPos(cameraPos);

	
	for (uint32_t i = 0; i < this->billboards.size(); i++)
	{
		glm::vec3 objToCamera;
		glm::vec3 lookAt;
		glm::vec3 upAux;

		objToCamera.x = cameraPos.x - billboards[i].pos.x;
		objToCamera.y = 0;
		objToCamera.z = cameraPos.z - billboards[i].pos.z;

		lookAt.x = 0;
		lookAt.y = 0;
		lookAt.z = 1;

		objToCamera = glm::normalize(objToCamera);

		upAux = glm::cross(lookAt, objToCamera);

		float a = _toAngle(lookAt, objToCamera);

		prog.setModel(
			glm::translate(glm::mat4(1.0f), this->billboards[i].pos) *
			glm::rotate(glm::mat4(1.0f), a, glm::vec3(upAux.x, upAux.y, upAux.z)) * 
			glm::scale(glm::mat4(1.0f), glm::vec3(billboards[i].scale, 0.0f)));

		switch (billboards[i].t)
		{
		case 0:
			prog.draw(this->tree);
			break;
		case 1:
			prog.draw(this->tree2);
			break;
		case 2:
			prog.draw(this->tree3);
			break;
		case 3:
			prog.draw(this->tree4);
			break;
		}
	}

	prog.unbind();
}

void BillboardManager::release()
{
	this->tree.release();
	this->prog.release();
}
