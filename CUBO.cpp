/*
 * CUbo.cpp
 *
 *  Created on: 18 de may. de 2018
 *      Author: fjimartinez
 */

#include "utils.h"

#include "CUBO.h"
#include "glm\glm.hpp"
#include "glm\gtc\type_ptr.hpp"

using namespace Engine::Graphics;

CUBO::CUBO() : Engine::Base::CObject(),
		m_uboIndex(0),
		m_uboHandler(0),
		m_uboBindingPoint(0)
{
	//clear();
}

CUBO::CUBO(const std::string& name) : Engine::Base::CObject(name),
		m_uboIndex(0),
		m_uboHandler(0),
		m_uboBindingPoint(0)
{
	//clear();
}

void CUBO::createUniformBlock(GLuint program_id, const std::string& uniform_block, size_t size, GLuint binding)
{
	//Creates an index to the given Uniform Buffer Object specified in 'uniform_block'
	m_uboIndex = glGetUniformBlockIndex(program_id, uniform_block.c_str());
	glUniformBlockBinding(program_id, m_uboIndex, binding);

	//Generates a handled to the buffer and bind it
	glGenBuffers(1, &m_uboHandler);
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboHandler);
	glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, binding, m_uboHandler, 0, size);
}

void CUBO::addData(GLuint offset, const GLvoid * data, size_t size)
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboHandler);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

GLuint CUBO::getUboIndex()
{
	return m_uboIndex;
}

CUBO::~CUBO()
{
	glDeleteBuffers(1, &m_uboHandler);
}

