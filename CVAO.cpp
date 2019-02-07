/*
 * CVao.cpp
 */

#include "CVAO.h"

using namespace Engine::Graphics;

inline unsigned int* BUFFER_OFFSET(int a)
{
	return ((unsigned int*) 0u + (a));
};

CVAO::CVAO() : CObject()
{
	Clear();

	CreateVertexArrays();
}

CVAO::CVAO(const std::string& name) : CObject(name)
{
	Clear();

	CreateVertexArrays();
}

void CVAO::Clear()
{
	m_numVertex = 0;
	m_numIndex = 0;
	m_Index.clear();
	m_Vertex.clear();
	m_Normal.clear();
	m_Texel.clear();
	m_Faces.clear();
}

void CVAO::CreateVertexArrays()
{
	m_handler = 0;

	/* Creates an arrays object */
	glGenVertexArrays(1, &m_vaoHandle);
	glBindVertexArray(m_vaoHandle);
}

void CVAO::CreateArrayBuffer(void *buffer, GLuint size, GLenum usage)
{
	/* Generates an id for buffer and get its id into 'vbo' */
	glGenBuffers(1, &m_vboHandles[m_handler]);

	/* Depending on nElements creates the buffer */
	glBindBuffer(GL_ARRAY_BUFFER, m_vboHandles[m_handler]);
	glBufferData(GL_ARRAY_BUFFER, size, buffer, usage);
}

void CVAO::CreateAttribArrayBuffer(GLuint attrib_id, GLuint elements)
{
	/* Asociates an attrib id to a array buffer */
	glEnableVertexAttribArray(attrib_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboHandles[m_handler]);
	glVertexAttribPointer(attrib_id, elements, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	
	//Prepate to next array buffer
	m_handler++;
}

GLuint CVAO::getHandler()
{
	return m_handler;
}

GLuint CVAO::getVaoHandler()
{
	return m_vaoHandle;
}

GLuint CVAO::getNumVertex()
{
	return m_numVertex;
}

GLuint CVAO::getNumIndex()
{
	return m_numIndex;
}

std::vector<glm::vec3>& CVAO::getvVertex()
{
	return m_Vertex;
}

std::vector<glm::vec3>& CVAO::getvNormal()
{
	return m_Normal;
}

std::vector<glm::vec2>& CVAO::getvTexel()
{
	return m_Texel;
}

std::vector<face>& CVAO::getvFace()
{
	return m_Faces;
}

std::vector<GLuint>& CVAO::getvIndex()
{
	return m_Index;
}

void CVAO::setNumVertex(GLuint n)
{
	m_numVertex = n;
}

void CVAO::setNumTexel(GLuint n)
{
	m_numTexel = n;
}

void CVAO::setNumIndex(GLuint n)
{
	m_numIndex = n;
}

void CVAO::addVertex(const glm::vec3 v)
{
	m_Vertex.push_back(v);
}

void CVAO::addNormal(const glm::vec3 n)
{
	m_Normal.push_back(n);
}

void CVAO::addTexel(const glm::vec2 t)
{
	m_Texel.push_back(t);
}

void CVAO::addIndex(GLuint i)
{
	m_Index.push_back(i);
}

vector3* CVAO::getv3Vertex()
{
	return m_v3Vertex;
}

vector3* CVAO::getv3Normal()
{
	return m_v3Normal;
}

vector2* CVAO::getv2Texel()
{
	return m_v2Texel;
}

GLuint*  CVAO::getv1Index()
{
	return m_iuIndex;
}

void CVAO::addv3Vertex(const vector3 v)
{

}

void CVAO::addv3Normal(const vector3 n)
{

}

void CVAO::addv2Texel(const vector2 t)
{

}

GLuint CVAO::getVBOHandesAt(GLuint buffer_id)
{
	return m_vboHandles[buffer_id];
}

CVAO::~CVAO()
{
	Clear();

	for (GLuint i = 0;i<m_handler;i++)
		glDeleteBuffers(1, &m_vboHandles[i]);
}

void CVAO::debug()
{
	std::cout << "\n";
	for (auto i : m_Texel)
	{
		std::cout << "(" << i.x << "," << i.y << ")";
	}
	std::cout.flush();
}
