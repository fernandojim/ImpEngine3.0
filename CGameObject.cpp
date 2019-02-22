/*
 * CGameObject.cpp
 *
 *  Created on: 14 de nov. de 2017
 *      Author: fjimartinez
 */

#include "CGameObject.h"

Engine::Entity::CGameObject::CGameObject() : Engine::Base::CObject(), m_hierarchyLevel(GameObjectLayer::LAYER_0), m_gameObjectParent(nullptr)
{

}

Engine::Entity::CGameObject::CGameObject(const std::string& name, GameObjectLayer lvl) : CObject(name), m_hierarchyLevel(lvl), m_gameObjectParent(nullptr)
{

}

std::shared_ptr<Engine::Component::CComponent> Engine::Entity::CGameObject::getComponent(Engine::Component::COMPONENT_TYPE c_typ)
{
	return m_components[ c_typ ];
}

void Engine::Entity::CGameObject::AddComponent(const std::shared_ptr<Engine::Component::CComponent>& comp)
{
	m_components[ comp->getComponentType() ] = comp;
}

void Engine::Entity::CGameObject::setParent(const std::shared_ptr<CGameObject>& go)
{
	m_gameObjectParent = go;
}

std::shared_ptr<Engine::Entity::CGameObject> Engine::Entity::CGameObject::getParent()
{
	return m_gameObjectParent;
}

Engine::Entity::CGameObject::~CGameObject()
{
	clear();
}

void Engine::Entity::CGameObject::clear()
{
	m_components.clear();
}
