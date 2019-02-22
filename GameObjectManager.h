/*********************************************
  File:       GameObjectManager.h
  Purpose:    Class for the GameObjectManager
  class name: GameObjectManager
  --------------------------------------------
  @Author: Yo
  @Version: 0
 *********************************************/

#ifndef GAMEOBJECTMANAGER_H_
#define GAMEOBJECTMANAGER_H_

#include <iostream>
#include <memory>
#include <vector>
#include <map>

#include "CObject.h"
#include "CGameObject.h"

namespace Engine
{
	namespace Managers
	{
		enum class EVENT_TYPE {EVENT_NULL = 0, EVENT_RENDER_TEXT, EVENT_MOVE_GAMEOBJECT};

		struct Event
		{
			std::string sender;
			std::string receiver;
			EVENT_TYPE eventTyp;
			void *buffer;
		};

		class GameObjectManager
		{
		public:
			GameObjectManager();
			~GameObjectManager();

			void startup();
			void release();

		   /********************************************************************************************
			** addGameObject
			** Add a new GameObject to the GameObject list
			********************************************************************************************/
			void addGameObject(const std::shared_ptr<Engine::Entity::CGameObject>& go);

		   /********************************************************************************************
			** deleteGameObject
			** Delete the given GameObject from the list of GameObjects
			********************************************************************************************/
			void deleteGameObject(const std::shared_ptr<Engine::Entity::CGameObject>& go);

		   /********************************************************************************************
			** AttachComponentToGameObject
			** Method to attach a component to its GameObject
			********************************************************************************************/
			void AttachComponent(const std::shared_ptr<Engine::Component::CComponent>& component, const std::string& go_name);

		   /********************************************************************************************
			** getGameObjectByName
			** Return a GameObject obtained by its name
			********************************************************************************************/
			std::shared_ptr<Engine::Entity::CGameObject> getGameObject(const std::string& name);

		   /********************************************************************************************
			** SendMessage
			** Method to send a message to an entity
			********************************************************************************************/
			bool SendEvent(const std::string& sender, const std::string& receiver, EVENT_TYPE typ, void* info);

			void debug();

			static GameObjectManager& getGameObjectManagerInstance();


		private:

			//Load objects from file
			void load(const std::string& file);

			//List of GameObjects
			std::map<std::string, std::shared_ptr<Engine::Entity::CGameObject>> m_gameObjects;
		};
	}
}

#endif /* GAMEOBJECTMANAGER_H_ */
