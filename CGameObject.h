/*********************************************
  File:       CGameObject.h
  Purpose:    Class for game objects
  class name: CGameObject derived from CObject
  --------------------------------------------
  @Author: Yo
  @Version: 0
 *********************************************/

#ifndef _CGAMEOBJECT_H_
#define _CGAMEOBJECT_H_

#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <unordered_map>

#include "CComponent.h"
#include "CObject.h"

namespace Engine
{
	namespace Entity
	{
		//GameObjecLayer is the layer that a GameObject occupies in the GameObjects hierarchy
		enum class GameObjectLayer : std::uint8_t {LAYER_0, LAYER_1, LAYER_2, LAYER_3};

		class CGameObject : public Engine::Base::CObject
		{
		public:
			CGameObject();
			CGameObject(const std::string& name, GameObjectLayer lvl);

			std::shared_ptr<Engine::Component::CComponent> getComponent(Engine::Component::COMPONENT_TYPE c_typ);

			void AddComponent(const std::shared_ptr<Engine::Component::CComponent>& comp);

			void setParent(const std::shared_ptr<CGameObject>& go);

			std::shared_ptr<CGameObject> getParent();

			virtual ~CGameObject();

		private:
			void clear();

			/* Level in the hierarchy */
			GameObjectLayer m_hierarchyLevel;

			std::shared_ptr<CGameObject> m_gameObjectParent;

			std::map<Engine::Component::COMPONENT_TYPE, std::shared_ptr<Engine::Component::CComponent>> m_components;
		};
	}
}

#endif

