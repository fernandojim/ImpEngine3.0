#include "CText.h"
#include "CTexture.h"
#include "TextureManager.h"
#include "CXMLParser.h"
#include "CRenderingSystem.h"

using namespace Engine::Graphics;

CText::CText(const std::string& name, GLuint shaderid, const std::string& file) : CRenderingComponent(name, shaderid),
		m_cellWidth(0.0),
		m_gridHeight(0),
		m_gridWidth(0),
		m_textureA("")
{
	GLint x,y,size,index;
	std::unique_ptr<CXMLParser> parser(new CXMLParser);

	// Read the content of file
	if (parser->LoadXMLFile(file))
	{
		if (parser->getNodes("FONTS"))
		{
			//Gets the geometric values for the cell
			m_cellWidth = std::atoi(parser->getAttributeValue("CELL_WIDTH").c_str());
			m_gridHeight = std::atoi(parser->getAttributeValue("GRID_HEIGHT").c_str());
			m_gridWidth = std::atoi(parser->getAttributeValue("GRID_WIDTH").c_str());
			m_textureA = parser->getAttributeValue("A");
			m_textureB = parser->getAttributeValue("B");

			//Create the Vertex Array Object
			for (GLuint i = 0;i<n_chars;i++)
				m_uiTextura[i] = 0;

			//Total num of vertex and texels and reserve it
			size = (m_gridHeight + 1) * (m_gridWidth + 1);
			getVAO()->setNumVertex(size);
			getVAO()->setNumTexel(size);
			getVAO()->getvVertex().reserve(size);
			getVAO()->getvTexel().reserve(size);

			//Total num of index
			index = ((m_gridWidth + 1) * 2 ) * m_gridHeight;
			getVAO()->setNumIndex(index );
			getVAO()->getvIndex().reserve(index);

			for (x = 0; x <= m_gridHeight; x++)
			{
				for (y = 0; y <= m_gridWidth; y++)
				{
					getVAO()->addVertex(glm::vec3(x * m_cellWidth, y * m_cellWidth, 0.0));
					getVAO()->addTexel(glm::vec2(static_cast<float>(x), static_cast<float>(y)));
				}
			}

			/*It´s time to create the index to referenciate the vertexs
			1   3
			|\  |
			^ \	^ ...
			|  \|
			0   2
			*/
			for (y = m_gridHeight - 1;y >= 0;y--)
			{
				for (x = 0; x < m_gridWidth; x++)
				{
					index = y + (x * (m_gridHeight + 1));
					getVAO()->addIndex(index);

					index = (y + 1) + (x * (m_gridHeight + 1));
					getVAO()->addIndex(index);

					index = y + ((x + 1) * (m_gridHeight + 1));
					getVAO()->addIndex(index);

					index = (y + 1) + ((x + 1) * (m_gridHeight + 1));
					getVAO()->addIndex(index);
				}
			}

			//VERTEX ATTRIB OBJECT
			//Create the vertex and texel array buffers
			getVAO()->CreateArrayBuffer(getVAO()->getvVertex().data(), getVAO()->getvVertex().size() * sizeof(glm::vec3), GL_STATIC_DRAW);
			getVAO()->CreateAttribArrayBuffer(3, 3);
			getVAO()->CreateArrayBuffer(getVAO()->getvTexel().data(), getVAO()->getvTexel().size() * sizeof(glm::vec2), GL_STATIC_DRAW);
			getVAO()->CreateAttribArrayBuffer(4, 2);

			//UNIFORM BUFFER OBJECT
			//-------------------------------------------------------------------------------------------------
			//Creates the ortographical projection. The view is fixed
			//glm::mat4 aux = glm::lookAt(glm::vec3(0.0, 0.0, 90.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
			//m_view = std::make_shared<glm::mat4>(glm::mat4(aux));
			m_view = glm::lookAt(glm::vec3(0.0, 0.0, 90.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
			m_projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 100.0f, -100.0f);
			m_model = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, 0.0));
			m_mvp = m_projection * m_view * m_model;

			//Configure the uniform to the Uniform Buffer Object
			//-------------------------------------------------------------------------------------------------
			/*GLuint n_uniforms = 3;
			const GLchar* names[n_uniforms] = {"view", "projection", "model"};

			getUBO()->createUniformBlock(shaderid, "Matrices");
			//getUBO()->addUniformNames(shaderid, n_uniforms, names);
			//getUBO()->activeUniformiv(shaderid, 0, glm::value_ptr(m_view), sizeof(m_view));
			//getUBO()->activeUniformiv(shaderid, 1, glm::value_ptr(m_projection), sizeof(m_projection));
			//getUBO()->activeUniformiv(shaderid, 2, glm::value_ptr(m_model), sizeof(m_model));
			//getUBO()->createUBO();

			getUBO()->addData(0, glm::value_ptr(m_view), sizeof(glm::mat4));
			getUBO()->addData(sizeof(glm::mat4), glm::value_ptr(m_projection), sizeof(glm::mat4));
			getUBO()->addData(2 * sizeof(glm::mat4), glm::value_ptr(m_model), sizeof(glm::mat4));*/

			//-------------------------------------------------------------------------------------------------

			//Load the text texture
			m_uiTextura[0] = Engine::Managers::TextureManager::getSingleton().loadTexture(m_textureA);
			m_uiTextura[1] = Engine::Managers::TextureManager::getSingleton().loadTexture(m_textureB);
		}
	}
}

void CText::setIdTexture(GLuint n, GLuint id)
{
	m_uiTextura[n] = id;
}

glm::mat4& CText::getProjectionMatrix()
{
	return m_projection;
}

glm::mat4& CText::getModelMatrix()
{
	return m_model;
}

glm::mat4& CText::getViewMatrix()
{
	return m_view;
}

glm::mat4& CText::getMVPMatrix()
{
	return m_mvp;
}

CText::~CText()
{

}
