/*
 * CRenderingSystem.cpp
 *
 *  Created on: 20 de nov. de 2017
 *      Author: fjimartinez
 */

#include <algorithm>

#include "GameManager.h"
#include "CRenderingSystem.h"
#include "CXMLParser.h"
#include "CText.h"

#include "glm\glm.hpp"
#include "glm\gtc\type_ptr.hpp"

using namespace Engine::System;

GLuint CRenderingSystem::shader_id = 0;

const std::string ShadersFile("objetos\\shaders.xml");

CRenderingSystem& CRenderingSystem::getRenderingSystem()
{
	static CRenderingSystem rs;

	return rs;
}

CRenderingSystem::CRenderingSystem() : CSystem(Engine::System::SystemType::RENDERER), m_uiShaders(0)
{

}

void CRenderingSystem::startup()
{
	clear();

	//Load and create the shaders
	loadShaders(ShadersFile);
}

void CRenderingSystem::clear()
{
	//clean the lists
	m_uiShaders = 0;
	m_Shaders.clear();
	m_RenderingComponentList2.clear();
}

void CRenderingSystem::loadShaders(const std::string& sShaders)
{
	std::shared_ptr<Engine::Graphics::CShader> shader;
	std::string shader_name;
	std::string vert;
	std::string frag;

	std::shared_ptr<CXMLParser> pfile = std::make_shared<CXMLParser>();

	if (pfile->LoadXMLFile(ShadersFile))
	{
		//Read and create the shaders
		while (pfile->getNodes("SHADER"))
		{
			//Read the values
			std::string name = pfile->getAttributeValue("name");
			std::string vshader = pfile->getAttributeValue("VSHADER");
			std::string fshader = pfile->getAttributeValue("FSHADER");

			//Creates the shader
			shader = std::make_shared<Engine::Graphics::CShader>(name, vshader.c_str(), fshader.c_str());

			//Add shader to list
			m_Shaders.insert(std::pair < GLuint, std::shared_ptr<Engine::Graphics::CShader> >(shader->getProgramId(), shader));

			//Get its program id and store in the list of program ids
			m_shaderListProgramIds[CRenderingSystem::shader_id++] = shader->getProgramId();

			m_uiShaders++;

			//Next node
			pfile->Next();
		}
	}
}

GLuint CRenderingSystem::getShaderIdByName(const std::string& sName)
{
	//Creates the iterator to search
	for (auto i : m_Shaders)
	{
		if (!sName.compare(i.second->getName()))
			return i.second->getProgramId();
	}

	return 0;
}

std::string CRenderingSystem::getShaderNameById(GLuint id)
{
	//Search the name of the shader by given id
	auto i = m_Shaders.find(id);

	if (i->second)
	{
		return i->second->getName();
	}

	return nullptr;
}

/************************************************
	The 'RegisterRenderingComponent' method copy
	 the necessary
	 info as the transform matrices and the Buffers
	 into an structure used for render the graphical
	 components
 *************************************************/
void CRenderingSystem::RegisterRenderingComponent(const std::shared_ptr<Engine::Component::CRenderingComponent>& rendercomp)
{
	//Adds the object to the rendering list
	m_RenderingComponentList.push_back(rendercomp);

	//Creates the renderer structure
	_renderer r;

	if ( rendercomp->getRenderingComponentType() == Engine::Component::renderingComponentType::TEXT_MESH )
	{
		r.m_projection = std::make_shared<glm::mat4>((std::static_pointer_cast<Engine::Graphics::CText>(rendercomp))->getProjectionMatrix());
		r.m_model = std::make_shared<glm::mat4>((std::static_pointer_cast<Engine::Graphics::CText>(rendercomp))->getModelMatrix());
		r.m_view = std::make_shared<glm::mat4>((std::static_pointer_cast<Engine::Graphics::CText>(rendercomp))->getViewMatrix());
		r.m_mvp = (std::static_pointer_cast<Engine::Graphics::CText>(rendercomp))->getMVPMatrix();
		r.programID = rendercomp->getShaderProgramId();
		r.typ = rendercomp->getRenderingComponentType();
		r.vao = rendercomp->getVAO();
		r.ubo = rendercomp->getUBO();
	}
	else
	{
		r.programID = rendercomp->getShaderProgramId();
		r.typ = rendercomp->getRenderingComponentType();
		r.vao = rendercomp->getVAO();
		r.ubo = rendercomp->getUBO();
	}

	//Add the render struct to the list
	m_RenderingComponentList2.push_back(r);

	//Sort the rendering component list from shader id
	std::sort(m_RenderingComponentList2.begin(), m_RenderingComponentList2.end(), [](const _renderer& first, const _renderer& second)
	{
		if (first.programID < second.programID)
			return true;
		else
			return false;
	});


	//Sort the rendering component list from shader id
	/*std::sort(m_RenderingComponentList.begin(), m_RenderingComponentList.end(), [](const std::shared_ptr<Engine::Component::CRenderingComponent> first, const std::shared_ptr<Engine::Component::CRenderingComponent> second)
	{
		if (first->getShaderProgramId() < second->getShaderProgramId())
			return true;
		else
			return false;
	});*/
}

/************************************************
	The 'Render' method renderize all the components
	since its type
 *************************************************/
void CRenderingSystem::Render()
{
	/* For each object in the list do
	 *    glUseProgram(object)
	 *      glBindBuffers()
	 *      glUniforms()
	 *
	 *      ...
	 *
	 *      glDrawArrays(object)
	 *  end for
	 *
	 */

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   /********************************************************
    * Render text
    ********************************************************/
	//Run the list of shaders id´s and use each one
	//for (std::uint8_t i = 0; i < m_uiShaders ; i++)
	for (auto i : m_RenderingComponentList2)
	{
		//glUseProgram(m_shaderListProgramIds[i]);
		glUseProgram(i.programID);

		if (i.typ == Engine::Component::renderingComponentType::TEXT_MESH)
		{
			//Activate the uniforms or uniform block
			//i.ubo->bindUBO();

			//Get the texture
			GLint ret = glGetUniformLocation(i.programID, "texturaA");
			glUniform1i(ret, 1);
			glActiveTexture(GL_TEXTURE0 + 1);
			glBindTexture(GL_TEXTURE_2D, 1);

			//Matrix de transformación
			GLint base = glGetUniformLocation(i.programID, "mvp");
			glUniformMatrix4fv(base, 1, GL_FALSE, glm::value_ptr(i.m_mvp));

			glBindVertexArray(i.vao->getHandler());
			GLint offset,size = 8;
			for (GLuint c = 0 ; c < 3 ; c++)
			{
				offset = size * c;
				debug(&(i.vao->getvIndex())[offset]);
				glDrawElements(GL_TRIANGLE_STRIP, size, GL_UNSIGNED_INT, &(i.vao->getvIndex())[offset]);
			}
			glBindVertexArray(0);
		}

		break;

		/*if (rc->getRenderingComponentType() == Engine::Component::renderingComponentType::TERRAIN_MESH)
		{
			glBindVertexArray(i.vao->getHandler());
			GLuint offset;
			GLuint size = sqrt(i.vao->getNumVertex()); //size is square root of numvertex
			GLuint size2 = size * 2;
			for (GLuint j = 0 ; j < size2 - 1 ; j++)
			{
				offset = size2 * j;
				glDrawElements(GL_TRIANGLE_STRIP, size2, GL_UNSIGNED_INT, &i.vao->getv1Index()[offset]);
			}
		}*/

	}
}

void CRenderingSystem::Update(float dt)
{

}

void CRenderingSystem::release()
{
	m_uiShaders = 0;
	m_Shaders.clear();
	m_RenderingComponentList.clear();
	m_RenderingComponentList2.clear();
}

void CRenderingSystem::debug(GLvoid *p)
{
	for (auto i : m_RenderingComponentList)
		std::cout << "\n NAME='" +
		             i->getName() +
					 "'" +
					 " SHADER='"+ getShaderNameById(i->getShaderProgramId()) + "'";

	std::vector<GLuint> vuc(static_cast<GLuint*>(p), static_cast<GLuint*>(p) + 8);

	for (auto x : vuc)
		std::cout << "," << x;

	std::cout.flush();
}

CRenderingSystem::~CRenderingSystem()
{

}
