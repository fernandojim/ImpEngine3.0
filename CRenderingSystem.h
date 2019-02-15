/*******************************************
  File:       CRenderingSystem.h
  Purpose:    Class for Rendering System
  class name: CRenderingSystem
  ------------------------------------------
  @Author: Yo
  @Version: 0
 *******************************************/

#ifndef CRENDERINGSYSTEM_H_
#define CRENDERINGSYSTEM_H_

#include <iostream>
#include <memory>
#include <vector>
#include <map>

#include "glm\mat4x4.hpp"
#include "glm\mat3x3.hpp"

//#include "CText.h"
#include "CShader.h"
#include "CSystem.h"
#include "CRenderingComponent.h"

namespace Engine
{
	namespace System
	{
		//ASSETS to render
		//Structs to pass to Uniform Buffer Objects. Be carefully with alignments in its fields
		//Samplers cannot be passed throught Uniform Buffer Objects.
		struct MVP_matrix
		{
			glm::mat4 m_m4model;
			glm::mat4 m_m4view;
			glm::mat4 m_m4proyection;
		};

		struct Light
		{
			glm::vec4 lightPosition;
			glm::vec4 lightIntensity;
		};

		struct Fog
		{
			float maxDist;
			float minDist;
			glm::vec3 fogColor;
		};

		struct Material
		{
			float shininess;
			glm::vec4 ka;
			glm::vec4 kd;
			glm::vec4 ks;
		};

		struct Tiling //TERRAIN
		{
			float tilingFactorSand;
			float tilingFactorGrass;
			float tilingFactorDirt;
			float tilingFactorSnow;
		};

		struct _renderer
		{
			//Shader program ID
			GLuint programID;

			//Rendering component type
			Engine::Component::renderingComponentType typ;

			//Matrixes
			glm::mat4 m_projection;
			glm::mat4 m_model;
			glm::mat4 m_view;
			glm::mat4 m_mvp;

			//Graphics
			std::shared_ptr<Engine::Graphics::CVAO> vao; //Vertex Attrib Object
			std::shared_ptr<Engine::Graphics::CUBO> ubo; //Uniform Buffer Object
		};

		class CRenderingSystem : public Engine::System::CSystem
		{
		public:
			CRenderingSystem();
			~CRenderingSystem();

			void startup();
			void release();

			void Update(float dt);

			void Render();

			void clear();

			//Stores the rendering object into the Render system in order to proceed to draw it
			//The information needed is the VAO object and its uniform buffers object
			//The component list is orderer by shader id
			void RegisterRenderingComponent(const std::shared_ptr<Engine::Component::CRenderingComponent>& rendercomp);

			//Get the program´s id by its name
			GLuint getShaderIdByName(const std::string& sName);
			std::string getShaderNameById(GLuint id);

			void debug(GLvoid *p);

			static CRenderingSystem& getRenderingSystem();

		private:
			void loadShaders(const std::string& sShaders);

			//Num of shader programs
			GLuint m_uiShaders;

			//Shaders map with their program id
			std::map < GLuint, std::shared_ptr<Engine::Graphics::CShader> > m_Shaders;

			//Each time a new shader is loaded, its 'id' is increased and its id stored in the list
			static GLuint shader_id;
			static const std::uint8_t max = 10; //The size of the list (max num of shaders loaded)
			GLuint m_shaderListProgramIds[max];

			//Rendering component list to render
			std::vector<std::shared_ptr<Engine::Component::CRenderingComponent>> m_RenderingComponentList;

			//List of _renderer struct to render
			std::vector<_renderer> m_RenderingComponentList2;
		};
	}
}

#endif /* CRENDERINGSYSTEM_H_ */
