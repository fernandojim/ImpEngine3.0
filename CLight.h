/*******************************************
  File:       CLight.h
  Purpose:    Class for lights
  class name: CLight
  ------------------------------------------
  @Author: Yo
  @Version: 0
 *******************************************/

#ifndef _CLIGHT_H_
#define _CLIGHT_H_

#define GLM_FORCE_RADIANS

#include <ImpEngine3.0/src/CObject.h>
#include "glm\vec3.hpp"
#include "glm\mat4x4.hpp"
#include "glm\gtc\matrix_transform.hpp"

namespace Engine
{
	namespace Graphics
	{
		struct light
		{
			glm::vec3 lightPosition;
			glm::vec3 lightIntensity;
			glm::mat4 shadowMatrix;
		};

		class CLight : public Engine::Base::CObject
		{
		public:
			light m_light;

		public:
			CLight();
			CLight(const std::string& name);
			CLight(const light& l);
			~CLight();

			void createShadowMatrix();

			void setLight(const light& l);
			light getLight();
		};
	}
}

#endif
