/*******************************************
  File:       CTexture.h
  Purpose:    Class for a Texture Object

  class name: CTexture derived from CObject
  ------------------------------------------
  @Author: Yo
  @Version: 0
 *******************************************/

#ifndef _CTEXTURE_H_
#define _CTEXTURE_H_

#include <iostream>
#include <fstream>
#include <cassert>

#include "CObject.h"
#include "CBmp.h"

#ifdef _OPENGL44_
 #include "gl_core_4_4.h"
#else
 #include "gl_core_3_3.h"
#endif

namespace Engine
{
	namespace Graphics
	{
		enum class loadTextureResult : std::int8_t {TEXTURE_OK, FILE_ERROR, TEXTURE_NOT_VALID};

		class CTexture : public Engine::Base::CObject
		{
		public:
			CTexture(const std::string& sFilename);	//Constructor
			~CTexture();						    //Destructor

			loadTextureResult getResult();

			unsigned int getTextureId();

		private:
			//Load the texture into OpenGL
			void bindTexture();

			//Generic info for texture image
			long         m_lHeight;
			long         m_lWidth;
			BYTE        *m_Pixeldata;

			// RGB or BGR
			GLenum       m_rgbAlignment; //GL_RGB or GL_BGR
			GLuint       m_textureId;    //Texture ID for OpenGL

			//Result loading texture
			loadTextureResult m_result;
		};
	}
}

#endif
