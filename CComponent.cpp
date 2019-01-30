/*
 * CComponent.cpp
 *
 *  Created on: 14 de nov. de 2017
 *      Author: fjimartinez
 */

#include "CComponent.h"

using namespace Engine::Component;

CComponent::CComponent() : CObject(), m_componentType(COMPONENT_TYPE::NO_TYPE), m_gameObject(0)
{

}

CComponent::CComponent(const std::string &name) : CObject(name), m_componentType(COMPONENT_TYPE::NO_TYPE), m_gameObject(nullptr)
{

}

//Copy assignation constructor
/*CComponent& CComponent::operator=(const CComponent &co)
{
	CObject::operator=(co);

	m_gameObject = co.m_gameObject;

	return *this;
}

//Copy constructor
CComponent::CComponent(const CComponent& component) : CObject(component), m_componentType(COMPONENT_TYPE::NO_TYPE)
{
	m_gameObject = component.m_gameObject;
}*/

COMPONENT_TYPE Engine::Component::componentType(const std::string& typ)
{
	if (!typ.compare("RENDERING"))
		return COMPONENT_TYPE::RENDERING;
	else if (!typ.compare("TRANSFORM"))
		return COMPONENT_TYPE::TRANSFORM;
	else if (!typ.compare("COLLIDE"))
		return COMPONENT_TYPE::COLLIDE;
	else
		return COMPONENT_TYPE::NO_TYPE;
}

void CComponent::setComponentType(COMPONENT_TYPE c_ty)
{
	m_componentType = c_ty;
}

COMPONENT_TYPE CComponent::getComponentType()
{
	return m_componentType;
}

std::shared_ptr<CGameObject> CComponent::getGameObject()
{
	return m_gameObject;
}

void CComponent::AttachToGameObject(const std::shared_ptr<CGameObject>& go)
{
	m_gameObject = go;
}

CComponent::~CComponent()
{
}
