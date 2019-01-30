/*********************************************
  File:       ComponentManager.h
  Purpose:    Class for ComponentManager
  class name:
  --------------------------------------------
  @Author: Yo
  @Version: 0
 *********************************************/

#ifndef COMPONENTMANAGER_H_
#define COMPONENTMANAGER_H_

#include <iostream>
#include <memory>

#include "CRenderingComponent.h"
#include "CTransformComponent.h"

namespace Engine
{
	namespace Managers
	{
		class ComponentManager
		{
		public:
			ComponentManager();
			~ComponentManager();

			void startup();
			void release();

			/********************************************************************************************
			 ** Creates rendering components;
			 ** By name, type of rendering component and its file as terrains, blender objs, MD2 models...
			 ********************************************************************************************/
			std::shared_ptr<Engine::Component::CRenderingComponent> createRenderingComponent(const std::string& name, const std::string& typ, GLuint shaderid, const std::string& file);

			/********************************************************************************************
			 ** Creates transform components;
			 ** Where its position, scale... are defined.
			 ********************************************************************************************/
			std::shared_ptr<Engine::Component::CTransformComponent> createTransformComponent(const std::string& name, glm::vec3 position, glm::vec3 scale);

			/********************************************************************************************
			 ** Get the ComponentManager unique instance
			 ********************************************************************************************/
			static ComponentManager& getComponentManagerInstance();

		private:
			void load(const std::string& file);

			//List of components
			std::vector<std::shared_ptr<Engine::Component::CComponent>> m_components;
		};
	}
}

#endif /* COMPONENTMANAGER_H_ */
