#include "CVAO.h"

using namespace Engine::Graphics;

CVAO::CVAO() : CObject()
{
	Clear();
}

CVAO::CVAO(const std::string& name) : CObject(name)
{
	Clear();
}

void CVAO::Clear()
{
	m_numVertex = 0;
	m_numIndex = 0;
	m_offset = 0;
	m_elements = 0;

	m_Index.clear();
	m_Vertex.clear();
	m_Normal.clear();
	m_Texel.clear();
	m_Faces.clear();
	m_Vextex3V3N2NP.clear();
}

void CVAO::CreateVertexArrays()
{
	m_arrayhandler = 0;

	/* Creates an arrays object */
	glGenVertexArrays(1, &m_vaoHandle);
	glBindVertexArray(m_vaoHandle);
}

void CVAO::CreateArrayBuffer(GLvoid *buffer, GLuint size, GLenum usage)
{
	// Generates an id for buffer and get its id into 'vbo'
	glGenBuffers(1, &m_vboHandles[m_arrayhandler]);

	// Creates the buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vboHandles[m_arrayhandler]);
	glBufferData(GL_ARRAY_BUFFER, size, buffer, usage);
}

void CVAO::CreateVertexAttribPointer(GLuint program_id, const std::string& attrib_name, GLuint elements, size_t sz, GLuint offset)
{
	GLuint attrib_loc = glGetAttribLocation(program_id, attrib_name.c_str());
	glEnableVertexAttribArray(attrib_loc);
	glVertexAttribPointer(attrib_loc, elements, GL_FLOAT, GL_FALSE, sz, (const GLvoid*)offset);
	glEnableVertexAttribArray(0);
}

void CVAO::CreateAttribArrayBuffer(GLuint attrib_id, GLuint elements, void *buffer, GLuint size, GLenum usage)
{
	/* Generates an id for buffer and get its id into 'vbo' */
	glGenBuffers(1, &m_vboHandles[m_arrayhandler]);

	/* Depending on nElements creates the buffer */
	glBindBuffer(GL_ARRAY_BUFFER, m_vboHandles[m_arrayhandler]);
	glBufferData(GL_ARRAY_BUFFER, size, buffer, usage);

	glEnableVertexAttribArray(attrib_id);
	glVertexAttribPointer(attrib_id, elements, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_arrayhandler++;
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

GLuint CVAO::getOffset()
{
	return m_offset;
}


GLuint CVAO::getElements()
{
	return m_elements;
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

std::vector<Vextex3V3N2NP>& CVAO::getvVextex3V3N2NP()
{
	return m_Vextex3V3N2NP;
}

void CVAO::setNumVertex(GLuint n)
{
	m_numVertex = n;
}

void CVAO::setNumIndex(GLuint n)
{
	m_numIndex = n;
}

void CVAO::setOffset(GLuint n)
{
	m_offset = n;
}

void CVAO::setElements(GLuint n)
{
	m_elements = n;
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

void CVAO::addVextex3V3N2NP(const Vextex3V3N2NP& v)
{
	m_Vextex3V3N2NP.push_back(v);
}

GLuint CVAO::getVBOHandesAt(GLuint buffer_id)
{
	return m_vboHandles[buffer_id];
}

CVAO::~CVAO()
{
	Clear();

	for (GLuint i = 0;i<m_arrayhandler;i++)
		glDeleteBuffers(1, &m_vboHandles[i]);
}

void CVAO::debug()
{
	std::cout << "\nVAO(" << m_vaoHandle << ")-->BUFF=" << m_vboHandles[0] << "," << m_vboHandles[1] << "," << m_vboHandles[2];

	std::cout.flush();
}
