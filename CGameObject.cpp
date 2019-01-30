/*
 * CGameObject.cpp
 *
 *  Created on: 14 de nov. de 2017
 *      Author: fjimartinez
 */

#include "CGameObject.h"

Engine::Entity::CGameObject::CGameObject() : Engine::Base::CObject(), m_hierarchyLevel(GameObjectLayer::LAYER_0), m_gameObjectParent(nullptr)
{

}

Engine::Entity::CGameObject::CGameObject(const std::string& name, GameObjectLayer lvl) : CObject(name), m_hierarchyLevel(lvl), m_gameObjectParent(nullptr)
{

}

std::shared_ptr<Engine::Component::CComponent> Engine::Entity::CGameObject::getComponent(Engine::Component::COMPONENT_TYPE c_typ)
{
	return m_components[ c_typ ];
}

void Engine::Entity::CGameObject::AddComponent(const std::shared_ptr<Engine::Component::CComponent>& comp)
{
	m_components[ comp->getComponentType() ] = comp;
}

void Engine::Entity::CGameObject::setParent(const std::shared_ptr<CGameObject>& go)
{
	m_gameObjectParent = go;
}

std::shared_ptr<Engine::Entity::CGameObject> Engine::Entity::CGameObject::getParent()
{
	return m_gameObjectParent;
}

Engine::Entity::CGameObject::~CGameObject()
{
	clear();
}

void Engine::Entity::CGameObject::clear()
{
	m_components.clear();
}

/*void CGameObject::clear()
{
	//Id initialization
	//m_iId = 0;

	m_type = NO_TYPE;

	m_WorldTransformation = NULL;
	m_LocalTransformation = NULL;

	//Initialize vectors
	m_position = NULL;
	m_velocity = NULL;
	m_scale = NULL;
	m_angle = NULL;
	m_angles = NULL;

	m_bIsVisible = false;
	m_bIsValid = false;

	m_pParent = NULL;
	m_camera = NULL;
	m_world = NULL;

	m_bSmoothShading = false;

	m_handler = 0;
	m_vaoHandle = 0;

	m_hierarchyLevel = LEVEL_0;

	m_iNumVertex = 0;
	m_iNumIndex = 0;
	m_Index.clear();
	m_Vertex.clear();
	m_Normal.clear();
	m_Texel.clear();
	m_vboHandles.clear();

	m_bSmoothShading = false;

	//Create a new Vertex Array Object
	m_handler = 0;

	glGenVertexArrays(1, &m_vaoHandle);
	glBindVertexArray(m_vaoHandle);
}

GLuint CGameObject::CreateArrayBuffer(void *buffer, GLuint size, GLenum usage)
{
	// Generates an id for a buffer
	GLuint id = 0;

	glGenBuffers(1, &id);

	m_vboHandles.push_back(id);

	// Store the buffer into GPU
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, size, buffer, usage);

	return id;
}

void CGameObject::CreateAttribArrayBuffer(GLuint attrib_id, GLuint buffer_id, GLuint elements)
{
	// Asociates an attrib (from shader layout) to an array buffer
	glEnableVertexAttribArray(attrib_id);
	glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
	glVertexAttribPointer(attrib_id, elements, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
}

void CGameObject::BindBufferAttrib(GLuint attrib_id, GLuint buffer_id, GLuint elements)
{
	// Enable the shader attrib
	glEnableVertexAttribArray(attrib_id);

	// Bind the buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffer_id);

	// Associates the buffer
	glVertexAttribPointer(attrib_id, elements, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
}*/
