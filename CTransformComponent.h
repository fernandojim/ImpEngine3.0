/*********************************************
  File:       CTransform.h
  Purpose:    Class for game objects Transform
  class name: CTransform
  --------------------------------------------
  @Author: Yo
  @Version: 0
 *********************************************/

#ifndef CTRANSFORMCOMPONENT_H_
#define CTRANSFORMCOMPONENT_H_

#define GLM_FORCE_RADIANS

#include <ImpEngine3.0/src/CComponent.h>
#include "glm\vec3.hpp"
#include "glm\mat4x4.hpp"
#include "glm\gtc\quaternion.hpp"

namespace Engine
{
	namespace Component
	{
		glm::vec3 stringToVec3(const std::string& cad);

		class CTransformComponent : public CComponent
		{
		public:
			CTransformComponent();
			CTransformComponent(const std::string& name);
			~CTransformComponent();

			void Clear();

		public:
			glm::mat4 m_WorldTransformation;
			glm::mat4 m_LocalTransformation;
			glm::vec3 m_position;
			glm::vec3 m_scale;
			glm::vec3 m_facing;
			glm::vec3 m_angles;
			glm::quat m_qrot;
			glm::quat m_qrotTo;
			glm::vec3 m_acceleration;
			glm::vec3 m_velocity;
			float     m_speed;
			float     m_angle;
		};
	};
};

#endif /* CTRANSFORMCOMPONENT_H_ */
