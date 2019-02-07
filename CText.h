/*********************************************
  File:       CText.h
  Purpose:    Class for Fonts in OpenGL
  class name: CText
  --------------------------------------------
  @Author: Yo
  @Version: 0
 *********************************************/

#ifndef _CTEXT_H_
#define _CTEXT_H_

#include <iostream>
#include <memory>

#define GLM_FORCE_RADIANS

#include "glm\vec2.hpp"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp" //Header for glm::perspective, etc...
#include "glm\gtc\type_ptr.hpp"
#include "CRenderingComponent.h"

namespace Engine
{
	namespace Graphics
	{
		class CText : public Engine::Component::CRenderingComponent
		{
		public:
			CText(const std::string& name, GLuint shaderid, const std::string& file);
			~CText();

			void setIdTexture(GLuint n, GLuint id);

			glm::mat4& getProjectionMatrix();
			glm::mat4& getModelMatrix();
			glm::mat4& getViewMatrix();
			glm::mat4& getMVPMatrix();

		private:
			static const GLuint n_chars = 36;  //Num of chars
			std::string m_text;                //Text to render
			GLuint      m_uiTextura[n_chars];  //Texture id´s A to Z and 0 to 9 texture chars

		   /********************************************************************************************
			** Screen values of the cell to draw a character and its grid
			********************************************************************************************/
			GLfloat m_cellWidth;
			GLint m_gridHeight;
			GLint m_gridWidth;

			std::string m_textureA;
			std::string m_textureB;

		   /********************************************************************************************
			** Transform matrixes (only for text rendering)
			********************************************************************************************/
			glm::mat4 m_projection;
			glm::mat4 m_model;
			glm::mat4 m_view;
			glm::mat4 m_mvp;
		};
	}
}

#endif
