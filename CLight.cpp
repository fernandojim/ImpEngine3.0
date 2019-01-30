#include "CLight.h"

using namespace Engine::Graphics;

CLight::CLight() : CObject()
{
	m_light.lightIntensity = glm::vec3(0.0);
	m_light.lightPosition = glm::vec3(0.0);
	m_light.shadowMatrix = glm::mat4(1.0);
}

CLight::CLight(const std::string& name) : CObject(name)
{
	m_light.lightIntensity = glm::vec3(0.0);
	m_light.lightPosition = glm::vec3(0.0);
	m_light.shadowMatrix = glm::mat4(1.0);
}

CLight::CLight(const light& l)
{
	m_light.lightIntensity = l.lightIntensity;
	m_light.lightPosition = l.lightPosition;
	m_light.shadowMatrix = l.shadowMatrix;
}

CLight::~CLight()
{

}

void CLight::createShadowMatrix()
{
	glm::mat4 m_projectionMatrix = glm::ortho<float>(-10.0, 10.0, -10.0, 10.0, -10.0, 20.0);
	glm::mat4 m_viewMatrix = glm::lookAt(m_light.lightPosition, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 m_modelMatrix = glm::mat4(1.0);
	m_light.shadowMatrix = m_projectionMatrix * m_viewMatrix * m_modelMatrix;
}

void CLight::setLight(const light& l)
{
	m_light.lightIntensity = l.lightIntensity;
	m_light.lightPosition = l.lightPosition;
	m_light.shadowMatrix = l.shadowMatrix;
}

light CLight::getLight()
{
	return m_light;
}
