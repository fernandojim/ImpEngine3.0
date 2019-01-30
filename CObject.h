/*****************************************
  File:       CObject.h
  Purpose:    Base class for game objects
              and several entities
  class name: CObject
  ---------------------------------------
  @Author: Yo
  @Version: 0
 *****************************************/

#ifndef COBJECT_H_
#define COBJECT_H_

#include <iostream>

namespace Engine
{
	namespace Base
	{
		class CObject
		{
		public:
			CObject();
			CObject(const std::string& name);

			//Copy constructor. Deleted for now
			CObject(const CObject& object) = delete;

			//Copy assignation constructor. Deleted for now
			CObject& operator=(const CObject &cp) = delete;

			//get/set name property
			std::string getName();
			void setName(const std::string& name);

			//get id Property
			unsigned int getId();

			//For assertions
			//virtual void __ASSERT() = 0;

			//Virtual destructor
			virtual ~CObject() = 0;

		private:
			std::string m_name;
			static std::uint8_t m_id; //'id' will be increased with each new object
		};
	}
}

#endif /* COBJECT_H_ */
