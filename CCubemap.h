/*****************************************
  File:       CCubeMap.h
  Purpose:    Class for a cube map
  class name: CCubeMap inherits from C3DObject
  ---------------------------------------
  @Author: Yo
  @Version: 0
 *****************************************/

#ifndef CCUBEMAP_H_
#define CCUBEMAP_H_

#include <iostream>

#include "CMesh.h"
#include "texturemanager.h"

namespace Engine
{
	namespace Graphics
	{
		class CCubemap : public Engine::Component::CRenderingComponent
		{
		public:
			//CCubemap(const std::string cubemap_name, unsigned int id);
			CCubemap(const std::string& file);
			~CCubemap();

			//void Render(CCamera *cam);
			GLuint getCubeMapTextureId();

		private:
			bool loadCubeMapData(const std::string& cubemap_name);
			void loadCubemapTextures();

			//Engine::Graphics::CMesh *m_Mesh;

			glm::mat4 m_M4model;
			glm::mat4 m_M4view;
			glm::mat4 m_M4projection;
			glm::mat4 m_M4mvp;

			std::string m_texNames[6];

			GLuint m_uiCubemapTexture;
		};
	}
}

#endif /* CCUBEMAP_H_ */
