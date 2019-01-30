/********************************************
  File:       CRenderingComponent.h
  Purpose:    Class for Rendering components
  class name: CRenderingComponent
  -------------------------------------------
  @Author: Yo
  @Version: 0
 ********************************************/

#ifndef CRENDERINGCOMPONENT_H_
#define CRENDERINGCOMPONENT_H_

#include <iostream>
#include <memory>

#include "CComponent.h"
#include "CVAO.h"
#include "CUBO.h"

namespace Engine
{
	namespace Component
	{
		const std::string TEXT_MESH("TEXT_MESH");
		const std::string TERRAIN_MESH("TERRAIN_MESH");
		const std::string BLENDER_MESH("BLENDER_MESH");
		const std::string MD2("MD2");
		const std::string CUBEMAP("CUBEMAP");

		enum class renderingComponentType : std::int8_t {NO_TYPE, TEXT_MESH, TERRAIN_MESH, BLENDER_MESH, MD2, CUBEMAP};

		class CRenderingComponent : public Engine::Component::CComponent
		{
		public:
			CRenderingComponent();
			CRenderingComponent(const std::string& name, GLuint shaderid);
			virtual ~CRenderingComponent();
			void create();

			GLuint getShaderName();
			void setShaderProgramId(GLuint sid);
			GLuint getShaderProgramId();

			void setRenderingComponentType(const std::string& typ);
			void setRenderingComponentType(renderingComponentType typ);
			renderingComponentType getRenderingComponentType();

			std::shared_ptr<Engine::Graphics::CVAO>& getVAO();
			std::shared_ptr<Engine::Graphics::CUBO>& getUBO();

		private:
			//The type of rendering component
			renderingComponentType m_renderingComponentType;

			//The shader id for rendering
			GLuint m_shaderProgramId;

			//Vertex Attrib Object
			std::shared_ptr<Engine::Graphics::CVAO> m_vao;

			//Uniforms Buffer Object
			std::shared_ptr<Engine::Graphics::CUBO> m_ubo;
		};
	};
};

#endif /* CRENDERINGCOMPONENT_H_ */
