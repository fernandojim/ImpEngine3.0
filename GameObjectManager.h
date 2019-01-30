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

#include "CGameObject.h"

namespace Engine
{
	namespace Managers
	{
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
			void AttachComponentToGameObject(const std::shared_ptr<Engine::Component::CComponent>& render_component, const std::string& go_name);

		   /********************************************************************************************
			** getGameObjectByName
			** Return a GameObject obtained by its name
			********************************************************************************************/
			std::shared_ptr<Engine::Entity::CGameObject> getGameObjectByName(const std::string& name);

			void debug();

			static GameObjectManager& getGameObjectManagerInstance();

		private:
			//Load objects from file
			void load(const std::string& file);

			//List of GameObjects
			std::vector<std::shared_ptr<Engine::Entity::CGameObject>> m_gameObjects;

		};
	}
}

#endif /* GAMEOBJECTMANAGER_H_ */
