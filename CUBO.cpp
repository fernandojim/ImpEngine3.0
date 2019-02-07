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
		m_uboSize(0),
		m_uboHandler(0),
		m_num_uniforms(0)
{
	clear();
}

CUBO::CUBO(const std::string& name) : Engine::Base::CObject(name),
		m_uboIndex(0),
        m_uboSize(0),
		m_uboHandler(0),
		m_num_uniforms(0)
{
	clear();
}

void CUBO::createUniformBlock(GLuint program_id, const std::string& uniform_block)
{
	//Creates an index to the given Uniform Buffer Object specified in 'uniform_block'
	m_uboIndex = glGetUniformBlockIndex(program_id, uniform_block.c_str());
	glUniformBlockBinding(program_id, m_uboIndex, 0);

	glGenBuffers(1, &m_uboHandler);
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboHandler);
	glBufferData(GL_UNIFORM_BUFFER, 192, nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_uboHandler, 0, 3 * sizeof(glm::mat4));

	/*if ( m_uboIndex == GL_INVALID_INDEX )
		Message::MessageBOX("Cannot create UBO: ", uniform_block.c_str());
	else
	{
		//Get the size of struct in the shader
		glGetActiveUniformBlockiv(program_id, m_uboIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &m_uboSize);

		//Create the buffer
		m_buffer = (GLubyte*)malloc(m_uboSize);

		if (m_buffer == nullptr)
		{
			Message::MessageBOX("Cannot malloc to: ", uniform_block.c_str());
		}
	}*/
}

void CUBO::addUniformNames(GLuint program_id, GLuint uniformCount, const GLchar** uniformNames)
{
	//Add the uniforms to the Uniforms BLock
	glGetUniformIndices(program_id, uniformCount, uniformNames, m_indices);
	glGetActiveUniformsiv(program_id, uniformCount, m_indices, GL_UNIFORM_OFFSET, m_offsets);
}

void CUBO::activeUniformiv(GLuint program_id, GLuint n, const GLvoid * data, size_t size)
{
	memcpy(m_buffer + m_offsets[n], data, size);
}

void CUBO::addData(GLuint offset, const GLvoid * data, size_t size)
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboHandler);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}



void CUBO::createUBO()
{
	glGenBuffers(1, &m_uboHandler);
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboHandler);
    glBufferData(GL_UNIFORM_BUFFER, 192, nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_uboHandler, 0, 3 * sizeof(glm::mat4));
}

void CUBO::bindUBO()
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboHandler);
}

GLubyte* CUBO::getBuffer()
{
	return m_buffer;
}


GLuint& CUBO::getUBOHandler()
{
	return m_uboHandler;
}

void CUBO::clear()
{
	//Clear the lists of uniforms into the UBO
	for (GLuint i = 0;i < max_uniforms;i++)
	{
		m_indices[i] = 0;
	    m_offsets[i] = 0;
	}
}

CUBO::~CUBO()
{
	glDeleteBuffers(1, &m_uboHandler);
}

