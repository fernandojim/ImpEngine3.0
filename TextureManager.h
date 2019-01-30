/*********************************************
  File:       TextureManager.h
  Purpose:    Class for TextureManager
  class name: TextureManager
  --------------------------------------------
  @Author: Yo
  @Version: 0
 *********************************************/

#ifndef TEXTUREMANAGER_H_
#define TEXTUREMANAGER_H_

#include <iostream>
#include <map>
#include <memory>
#include <cassert>

#include "CTexture.h"

namespace Engine
{
	namespace Managers
	{
		class TextureManager
		{
		public:
			TextureManager();
			TextureManager(const std::string& file);
			~TextureManager();

			GLuint loadTexture(const std::string& sTexturePath);
			//GLuint depthBufferTexture(GLuint &iFrameBuffer);
			std::shared_ptr<Engine::Graphics::CTexture> getTextureByID(GLuint id);
			bool deleteTexture(GLuint id);

			static TextureManager& getSingleton();
		private:
			//Textures map CTexture class and opengl-handler
			std::map < GLuint, std::shared_ptr<Engine::Graphics::CTexture> > m_Textures;

			//Clear the list
			void clear();
		};
	}
}

#endif /* TEXTUREMANAGER_H_ */
