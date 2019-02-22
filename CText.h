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
#include "GameObjectManager.h"

namespace Engine
{
	namespace Graphics
	{
		class CText : public Engine::Component::CRenderingComponent
		{
		public:
			CText(const std::string& name, GLuint shaderid, const std::string& file);
			~CText();

			void setIdTexture(GLuint id);

			void ReceiveEvent(void *buff);

		private:
			bool loadValues(const std::string& file);

			static const GLuint n_chars = 36;  //Num of chars
			std::string m_text;                //Text to render
			GLuint      m_uiFontsTexture;      //Texture id where is painted the characters

			std::vector<GLuint>	m_charCodes;

			void text(const std::string& tx);
			void calculateTextureCoordinates();

		   /********************************************************************************************
			** Screen values of the cell to draw a character and its grid
			********************************************************************************************/
			GLfloat m_cellWidth;
			GLint m_gridHeight;
			GLint m_gridWidth;

			std::string m_textureFonts;

			GLint m_textureFontsGridX;
			GLint m_textureFontsGridY;
			GLint m_fontPixelsX;
			GLint m_fontPixelsY;

		   /********************************************************************************************
			** Texture coordinates for rendering the characters
			********************************************************************************************/
			std::vector<Engine::Graphics::vector2> m_charTextures;
		};
	}
}

#endif
