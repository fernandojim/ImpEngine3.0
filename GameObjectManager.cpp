/*
 * GameObjectManager.cpp
 *
 *  Created on: 21 de mar. de 2018
 *      Author: fjimartinez
 */

#include "GameObjectManager.h"
#include "ComponentManager.h"
#include "CXMLParser.h"
#include "CGameObject.h"
#include "utils.h"
#include "CRenderingSystem.h"

using namespace Engine::Managers;

//Forward declaration in order to avoid circular dependency between CComponent and CGameObject
class CComponent;

const std::string GameObjectManagerFile("objetos\\gameobjects.xml");

//
// -- The Instance to GameObjectManager class
//
GameObjectManager& GameObjectManager::getGameObjectManagerInstance()
{
	static GameObjectManager gom;

	return gom;
}

GameObjectManager::GameObjectManager()
{
}

void GameObjectManager::startup()
{
	load(GameObjectManagerFile);
}

void GameObjectManager::load(const std::string& file)
{
	std::shared_ptr<CXMLParser> pfile = std::make_shared<CXMLParser>();

	if (pfile->LoadXMLFile(file))
	{
		//Read and creates the gameobjects
		while (pfile->getNodes("GAMEOBJECT"))
		{
			//Read Name, Parent and Layer of the GameObject
			std::string name = pfile->getAttributeValue("name");          //Name and id are unique for each gameobject
			std::string parent = pfile->getAttributeValue("parent");      //parent gameobject
			int layer = atoi(pfile->getAttributeValue("layer").c_str());  //layer in the gameobjects hierarchy

			//Creates the gameobject
			std::shared_ptr<Engine::Entity::CGameObject> gameobject = std::make_shared<Engine::Entity::CGameObject>(name, static_cast<Engine::Entity::GameObjectLayer>(layer));

			//Set the gameobject parent
			gameobject->setParent(getGameObjectByName(parent));

			//Finally, adds the gameobject to the gameobject list
			addGameObject(gameobject);

			//Next node
			pfile->Next();
		}
	}
	else
	{
		Message::MessageBOX("Engine file error", "GameObjects file not found:\n" + file );
		exit(0);
	}
}

void GameObjectManager::addGameObject(const std::shared_ptr<Engine::Entity::CGameObject>& go)
{
	m_gameObjects.push_back(go);
}

void GameObjectManager::deleteGameObject(const std::shared_ptr<Engine::Entity::CGameObject>& go)
{
	int i = 0;
	for (auto j : m_gameObjects)
	{
		if (j == go)
			m_gameObjects.erase(m_gameObjects.begin() + i);
		i++;
	}
}

void GameObjectManager::AttachComponentToGameObject(const std::shared_ptr<Engine::Component::CComponent>& render_component, const std::string& go_name)
{
	//Search the gameobject to attach with

	std::shared_ptr<Engine::Entity::CGameObject> obj = getGameObjectByName(go_name);
	//std::shared_ptr<CGameObject> obj = getGameObjectByName(go_name);


	obj->AddComponent(render_component);

	//Creates the pointer to Gameobject for the component
	//render_component->AttachToGameObject(obj);
	//render_component->AttachToGameObject(getGameObjectByName(go_name));
}

std::shared_ptr<Engine::Entity::CGameObject> GameObjectManager::getGameObjectByName(const std::string& name)
{
	//Search the object in the list
	for (auto &i : m_gameObjects)
	{
		if (!i->getName().compare(name))
			return i;
	}

	//If there is no object with that 'name' the object won´t have parent (i.e. main object)
	return nullptr;

}

void GameObjectManager::release()
{
	m_gameObjects.clear();
}

void GameObjectManager::debug()
{
	for (auto i : m_gameObjects)
	{
		std::cout << "\n NAME='" << i->getName() << "'";

		std::shared_ptr<Engine::Component::CComponent> c1 = i->getComponent(Engine::Component::COMPONENT_TYPE::TRANSFORM);
		std::shared_ptr<Engine::Component::CComponent> c2 = i->getComponent(Engine::Component::COMPONENT_TYPE::RENDERING);

		if (c1 != nullptr)
			std::cout << " TRANSFORM COMPONENT NAME='" + c1->getName() + "'";

		if (c2 != nullptr)
			std::cout << " RENDERING COMPONENT NAME='" + c2->getName() + "'";
	}
}

GameObjectManager::~GameObjectManager()
{

}

