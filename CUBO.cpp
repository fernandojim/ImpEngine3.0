/*
 * CUbo.cpp
 *
 *  Created on: 18 de may. de 2018
 *      Author: fjimartinez
 */

#include "CUBO.h"

using namespace Engine::Graphics;

CUBO::CUBO() : Engine::Base::CObject(),
		m_uboIndex(0),
		m_uboSize(0),
		m_uboHandle(0),
		m_num_uniforms(0)
{
	clear();
}

CUBO::CUBO(const std::string& name) : Engine::Base::CObject(name),
		m_uboIndex(0),
        m_uboSize(0),
		m_uboHandle(0),
		m_num_uniforms(0)
{
	clear();
}

void CUBO::uniformBlockIndex(GLuint shader_id, const std::string& uniform)
{
	m_uboIndex = glGetUniformBlockIndex(shader_id, uniform.c_str());

	//Get the size of struct in the shader
	glGetActiveUniformBlockiv(shader_id, m_uboIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &m_uboSize);

	//Create the buffer
	m_buffer = malloc(m_uboSize);

	if (m_buffer == nullptr)
	{
		std::cout << "\n Error al crear el buffer";
	}
}

GLvoid* CUBO::getBuffer()
{
	return m_buffer;
}

void CUBO::addUniformsToUbo(GLuint shader_id, GLuint nnames, const GLchar** uniform)
{
	//Add the uniforms to the Uniforms BLock
	glGetUniformIndices(shader_id, nnames, uniform, m_indexes);
	glGetActiveUniformsiv(shader_id, nnames, m_indexes, GL_UNIFORM_OFFSET, m_offsets);
	glGetActiveUniformsiv(shader_id, nnames, m_indexes, GL_UNIFORM_SIZE, m_sizes);
	glGetActiveUniformsiv(shader_id, nnames, m_indexes, GL_UNIFORM_TYPE, m_types);
}

void CUBO::createUniformBufferObject(std::size_t size, GLuint usage)
{
	//Create the Uniform Buffer Object
	glGenBuffers(1, &m_uboHandle);
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboHandle);
	glBufferData(GL_UNIFORM_BUFFER, size, nullptr, usage);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_uboHandle, 0, size);
}

void CUBO::update(std::size_t size, const void * data)
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboHandle);
	glBufferSubData(GL_UNIFORM_BUFFER, size, size, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void CUBO::update2(GLintptr offset, GLsizeiptr size, const GLvoid* data)
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboHandle);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

GLuint CUBO::getUBOHandler()
{
	return m_uboHandle;
}

void CUBO::clear()
{
	//Clear the lists of uniforms into the UBO
	for (GLuint i = 0;i < max_uniforms;i++)
	{
		m_indexes[i] = 0;
	    m_sizes[i] = 0;
		m_offsets[i] = 0;
		m_types[i] = 0;
	}
}

CUBO::~CUBO()
{
	glDeleteBuffers(1, &m_uboHandle);
}

