/*****************************************
  File:       CComponent.h
  Purpose:    Base class for game components
  class name: CCompoment
  ---------------------------------------
  @Author: Yo
  @Version: 0
 *****************************************/

#ifndef CCOMPONENT_H_
#define CCOMPONENT_H_

#include <iostream>
#include <memory>

#include "CObject.h"

//Forward declaration in order to avoid circular dependency between CGameObject and CComponent
class CGameObject;

namespace Engine
{
	namespace Component
	{
		enum class COMPONENT_TYPE {NO_TYPE = 0, RENDERING, TRANSFORM, COLLIDE};

		//Return the type of Component as COMPONENT_TYPE traduced from a string
		COMPONENT_TYPE componentType(const std::string& typ);

		class CComponent : public Engine::Base::CObject
		{
		public:
			CComponent();
			CComponent(const std::string &name);
			virtual ~CComponent() = 0;

			//Copy constructor
			//CComponent(const CComponent& component);

			//Copy assignation constructor
			//CComponent& operator=(const CComponent &co);

			//virtual void RegisterComponent();

		   /*********************************************************************
			** Get the GameObject who owns this component
			*********************************************************************/
			std::shared_ptr<CGameObject> getGameObject();

		   /*********************************************************************
			** Attach this component to a GameObject by const reference
			*********************************************************************/
			void AttachToGameObject(const std::shared_ptr<CGameObject>& go);

		   /*********************************************************************
			** Set the type of this component (RENDERER, TRANSFORM, ... )
			*********************************************************************/
			void setComponentType(COMPONENT_TYPE c_ty);

		   /*********************************************************************
			** Get the type of this component (RENDERER, TRANSFORM, ... )
			*********************************************************************/
			COMPONENT_TYPE getComponentType();

		private:
			COMPONENT_TYPE m_componentType;

			//Pointer to the gameobject who owns the component
			std::shared_ptr< CGameObject > m_gameObject;
		};
	};
};

#endif /* CCOMPONENT_H_ */
