/*******************************************
  File:       CRenderingComponent.h
  Purpose:    Class for Rendering Component
  class name: CRenderingComponent
  ------------------------------------------
  @Author: Yo
  @Version: 0
 *******************************************/

#include "CRenderingComponent.h"
#include "ComponentManager.h"

using namespace Engine::Component;
using namespace Engine::Managers;

CRenderingComponent::CRenderingComponent() : Engine::Component::CComponent(), m_renderingComponentType(renderingComponentType::NO_TYPE),
																			  m_shaderProgramId(0)
{
	create();
}

CRenderingComponent::CRenderingComponent(const std::string& name, GLuint shaderid) : Engine::Component::CComponent(name), m_renderingComponentType(renderingComponentType::NO_TYPE),
																														  m_shaderProgramId(shaderid)
{
	create();
}

void CRenderingComponent::create()
{
	//Set the type as RENDERING
	setComponentType(Engine::Component::COMPONENT_TYPE::RENDERING);

	m_vao = std::unique_ptr<Engine::Graphics::CVAO>(new Engine::Graphics::CVAO());
	m_ubo = std::unique_ptr<Engine::Graphics::CUBO>(new Engine::Graphics::CUBO());
}

void CRenderingComponent::setShaderProgramId(GLuint sid)
{
	m_shaderProgramId = sid;
}

GLuint CRenderingComponent::getShaderProgramId()
{
	return m_shaderProgramId;
}

GLuint CRenderingComponent::getShaderName()
{
	return m_shaderProgramId;
}


std::shared_ptr<Engine::Graphics::CVAO>& CRenderingComponent::getVAO()
{
	return m_vao;
}

std::shared_ptr<Engine::Graphics::CUBO>& CRenderingComponent::getUBO()
{
	return m_ubo;
}

void CRenderingComponent::setRenderingComponentType(const std::string& typ)
{
	if (!typ.compare(TEXT_MESH))
		m_renderingComponentType = renderingComponentType::TEXT_MESH;
	else if (!typ.compare(TERRAIN_MESH))
		m_renderingComponentType = renderingComponentType::TERRAIN_MESH;
	else if (!typ.compare(BLENDER_MESH))
		m_renderingComponentType = renderingComponentType::BLENDER_MESH;
	else if (!typ.compare(MD2))
		m_renderingComponentType = renderingComponentType::MD2;
	else if (!typ.compare(CUBEMAP))
		m_renderingComponentType = renderingComponentType::CUBEMAP;
	else
		m_renderingComponentType = renderingComponentType::NO_TYPE;
}

void CRenderingComponent::setRenderingComponentType(renderingComponentType typ)
{
	m_renderingComponentType = typ;
}

renderingComponentType CRenderingComponent::getRenderingComponentType()
{
	return m_renderingComponentType;
}

CRenderingComponent::~CRenderingComponent()
{

}
