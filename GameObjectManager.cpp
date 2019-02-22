/*
 * GameObjectManager.cpp
 *
 *  Created on: 21 de mar. de 2018
 *      Author: fjimartinez
 */

#include <algorithm>

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
	release();

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
			gameobject->setParent(getGameObject(parent));

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
	m_gameObjects.insert(std::pair<std::string, std::shared_ptr<Engine::Entity::CGameObject>>(go->getName(), go));
}

void GameObjectManager::deleteGameObject(const std::shared_ptr<Engine::Entity::CGameObject>& go)
{
	/*int i = 0;
	for (auto j : m_gameObjects)
	{
		if (j == go)
			m_gameObjects.erase(m_gameObjects.begin() + i);
		i++;
	}*/
}

void GameObjectManager::AttachComponent(const std::shared_ptr<Engine::Component::CComponent>& component, const std::string& go_name)
{
	//Search the gameobject to attach with
	std::shared_ptr<Engine::Entity::CGameObject> obj = getGameObject(go_name);

	//Add the component if it exists
	if (obj != nullptr)
		obj->AddComponent(component);
}

std::shared_ptr<Engine::Entity::CGameObject> GameObjectManager::getGameObject(const std::string& name)
{
	return m_gameObjects[name];
}

void GameObjectManager::release()
{
	m_gameObjects.clear();
}

bool GameObjectManager::SendEvent(const std::string& sender, const std::string& receiver, EVENT_TYPE typ, void* info)
{
	//Create the event to send to the entity and give its values
	Event ev;

	ev.sender = sender;
	ev.receiver = receiver;
	ev.eventTyp = typ;
	ev.buffer = info;

	//Gets the GameObject who the message was send to. It is find by its name
	std::shared_ptr<Engine::Entity::CGameObject> go = getGameObject(receiver);

	if (go != nullptr)
	{
		//Depending on type of event send the event to a component
		switch (typ)
		{
			//Send a text to the text gameobject
			case Engine::Managers::EVENT_TYPE::EVENT_RENDER_TEXT:
				go->getComponent(Engine::Component::COMPONENT_TYPE::RENDERING)->ReceiveEvent(ev.buffer);
			break;
		}

		return true;
	}

	return false;
}

void GameObjectManager::debug()
{
	for (auto i : m_gameObjects)
	{
		std::cout << "\n NAME='" << i.second->getName() << "'";

		std::shared_ptr<Engine::Component::CComponent> c1 = i.second->getComponent(Engine::Component::COMPONENT_TYPE::TRANSFORM);
		std::shared_ptr<Engine::Component::CComponent> c2 = i.second->getComponent(Engine::Component::COMPONENT_TYPE::RENDERING);

		if (c1 != nullptr)
			std::cout << " TRANSFORM COMPONENT NAME='" + c1->getName() + "'";

		if (c2 != nullptr)
			std::cout << " RENDERING COMPONENT NAME='" + c2->getName() + "'";
	}
}

GameObjectManager::~GameObjectManager()
{

}

