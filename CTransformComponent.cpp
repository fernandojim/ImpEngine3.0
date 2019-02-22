/*
 * CTransform.cpp
 *
 *  Created on: 17 de nov. de 2017
 *      Author: fjimartinez
 */

#include <sstream>

#include "CTransformComponent.h"

using namespace Engine::Component;

CTransformComponent::CTransformComponent() : CComponent()
{
	Clear();
}

CTransformComponent::CTransformComponent(const std::string& name) : CComponent(name)
{
	Clear();
}

void CTransformComponent::Clear()
{
	m_WorldTransformation = glm::mat4(1.0);
	m_LocalTransformation = glm::mat4(1.0);
	m_position = glm::vec3(0.0, 0.0, 0.0);
	m_scale = glm::vec3(1.0, 1.0, 1.0);
	m_facing  = glm::vec3(0.0, 0.0, 0.0);
	m_angles = glm::vec3(0.0, 0.0, 0.0);
	//m_qrot = glm::vec3(0.0, 0.0, 0.0);
	//m_qrotTo = glm::vec3(0.0, 0.0, 0.0);
	m_acceleration = glm::vec3(0.0, 0.0, 0.0);
	m_velocity = glm::vec3(0.0, 0.0, 0.0);
	m_speed = 0.0;
	m_angle = 0.0;
}

glm::vec3 Engine::Component::stringToVec3(const std::string& cad)
{
	std::istringstream c(cad);
	std::string token;
	glm::vec3 ret;

	//Run the 'cad' and tokenize by the character ','
	std::getline(c, token, ',');
	ret.x = std::atof(token.c_str());

	std::getline(c, token, ',');
	ret.y = std::atof(token.c_str());

	std::getline(c, token, ',');
	ret.z = std::atof(token.c_str());

	return ret;
}

void CTransformComponent::ReceiveEvent(void *buff)
{

}

CTransformComponent::~CTransformComponent()
{

}
