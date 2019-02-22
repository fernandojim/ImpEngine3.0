/*********************************************
  File:       CSystem.h
  Purpose:    Base Class for the System
  class name: CSystem
  --------------------------------------------
  @Author: Yo
  @Version: 0
 *********************************************/

#ifndef CSYSTEM_H_
#define CSYSTEM_H_

#include <iostream>

namespace Engine
{
	namespace System
	{
		enum class SystemType : std::int8_t {NO_TYPE, RENDERER, TRANSFORM};

		//Return the type of Graphical Component as GraphicalComponentType traduced from a string
		SystemType systemType(unsigned int typ);

		class CSystem
		{
		public:
			CSystem();
			CSystem(const SystemType& tp);

			virtual void Animate() = 0;
			virtual void Update(float dt) = 0;
			virtual void startup() = 0;
			virtual void release() = 0;

			virtual ~CSystem() = 0;

		private:
			SystemType m_systemType;
		};
	}
}

#endif /* CSYSTEM_H_ */
