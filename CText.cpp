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
	GLint x,y,size,grid;
	GLuint ind;
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

			//Total num of quads
			grid = m_gridHeight * m_gridWidth;

			//Num of vertex
			size = grid * 4;

			//It is created a simple quad in order to render one texture with the corresponding letter
			//Creates the vertex, texel and index vectors
			getVAO()->getvVertex().reserve(size);
			getVAO()->getvTexel().reserve(size);
			getVAO()->getvIndex().reserve(size);

			getVAO()->setNumVertex(size);
			getVAO()->setNumIndex(size);

			ind = 0;
			for (y = m_gridHeight - 1; y >= 0; y--)
			{
				for (x = 0; x < m_gridWidth; x++)
				{
					/* Vertex´s
					0 => x*cell, y * cell, 0             2 -->-- 3
					1 => (x+1)*cell, y * cell, 0           \
					2 => x*cell, (y + 1) * cell, 0           \
					3 => (x+1)*cell, (y + 1) * cell, 0   0 -->-- 1
					*/
					getVAO()->addVertex(glm::vec3(x * m_cellWidth, y * m_cellWidth, 0.0));
					getVAO()->addIndex(ind++);

					getVAO()->addVertex(glm::vec3((x + 1) * m_cellWidth, y * m_cellWidth, 0.0));
					getVAO()->addIndex(ind++);

					getVAO()->addVertex(glm::vec3(x * m_cellWidth, (y + 1) * m_cellWidth, 0.0));
					getVAO()->addIndex(ind++);

					getVAO()->addVertex(glm::vec3((x + 1) * m_cellWidth, (y + 1) * m_cellWidth, 0.0));
					getVAO()->addIndex(ind++);

					//Texels
					getVAO()->addTexel(glm::vec2(0.0, 0.0));
					getVAO()->addTexel(glm::vec2(1.0, 0.0));
					getVAO()->addTexel(glm::vec2(0.0, 1.0));
					getVAO()->addTexel(glm::vec2(1.0, 1.0));
				}
			}

			//UNIFORM BUFFER OBJECT
			//-------------------------------------------------------------------------------------------------
			//Creates the ortographical projection. The view is fixed
			//glm::mat4 aux = glm::lookAt(glm::vec3(0.0, 0.0, 90.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
			//m_view = std::make_shared<glm::mat4>(glm::mat4(aux));
			m_view = std::make_shared<glm::mat4>(glm::mat4(glm::lookAt(glm::vec3(0.0, 0.0, 90.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0))));
			m_projection = std::make_shared<glm::mat4>(glm::mat4(glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 100.0f, -100.0f)));
			m_model = std::make_shared<glm::mat4>(glm::mat4(glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, 0.0))));
			//m_mvp = m_projection * m_view * m_model;

			//Configure the uniform to the Uniform Buffer Object
			GLuint n_uniforms = 3;
			const GLchar* names[n_uniforms] = {"view", "projection", "model"};

			GLuint indices[n_uniforms];
			GLint size[n_uniforms];
			GLint offset[n_uniforms];
			GLint type[n_uniforms];

			glGetUniformIndices(shaderid, n_uniforms, names, indices);
			glGetActiveUniformsiv(shaderid, n_uniforms, indices, GL_UNIFORM_OFFSET, offset);
			glGetActiveUniformsiv(shaderid, n_uniforms, indices, GL_UNIFORM_SIZE, size);
			glGetActiveUniformsiv(shaderid, n_uniforms, indices, GL_UNIFORM_TYPE, type);

			memcpy(getUBO()->getBuffer() + offset[m_view], &m_view, size[m_view] )

			std::cout << "\nMata";


			//-------------------------------------------------------------------------------------------------

			//VERTEX ATTRIB OBJECT
			//Create the vertex and texel array buffers
			getVAO()->CreateArrayBuffer(getVAO()->getvVertex().data(), getVAO()->getvVertex().size() * sizeof(glm::vec3), GL_STATIC_DRAW);
			getVAO()->CreateAttribArrayBuffer(3, 3);
			getVAO()->CreateArrayBuffer(getVAO()->getvTexel().data(), getVAO()->getvVertex().size() * sizeof(glm::vec2), GL_STATIC_DRAW);
			getVAO()->CreateAttribArrayBuffer(4, 2);

			//Load the text texture
			m_uiTextura[0] = Engine::Managers::TextureManager::getSingleton().loadTexture(m_textureA);

			std::cout << "\nCREADO TEXT";
			std::cout.flush();
		}
	}
}

void CText::RenderText(const std::string& text)
{
	GLuint chr;
	GLuint ret;
	GLushort ix, iy;
	int despx = 0;
	int despy = 0;
	
	m_text = text;

	//Renderiza si el tamaño del texto es mayor que 0
	for (GLuint i = 0; i < m_text.length(); i++)
	{
		//Obtenemos el código del carácter a dibujar
		chr = m_text.at(i);

		if (m_text.at(i) == '<')
		{
			despy += 16;
			despx = 0;
			chr = m_text.at(++i);
		}
		
		ix = 0;
		iy = 0;

		// Del código del carácter se obtiene el índice para el desplazamiento en la textura
		if (chr >= 48 && chr <= 53)		// 0..5
		{
			ix = chr - 22;
		}
		else if (chr >= 54 && chr <= 57)   // 6..9
		{
			ix = chr - 22 - 32;
			iy = 1;
		}
		else if ((chr >= 65 && chr <= 90) || (chr >= 97 && chr <= 122))	// A..Z o a..z
		{
			if (chr >= 97)
				ix = chr - 97; //Minúsculas
			else
				ix = chr - 65; //Mayúsculas
		}
		else if (chr == 45)	   //guión
		{
			ix = 1;
			iy = 2;
		}
		else if (chr == 46)				// punto
		{
			ix = 0;
			iy = 2;
		}
		else if (chr == 32)				    // Espacio en blanco
		{
			ix = 32;
			iy = 7;
		}

		//Para cada caracter, elegimos las coordenadas de textura de su letra
		getVAO()->getvTexel()[0].x = ix / 32.0f;
		getVAO()->getvTexel()[0].y = (8.0f - iy - 1.0f) / 8.0f;
		getVAO()->getvTexel()[1].x = (ix + 1) / 32.0f;
		getVAO()->getvTexel()[1].y = (8.0f - iy - 1.0f) / 8.0f;
		getVAO()->getvTexel()[2].x = ix / 32.0f;
		getVAO()->getvTexel()[2].y = (8.0f - iy) / 8.0f;
		getVAO()->getvTexel()[3].x = (ix + 1.0f) / 32.0f;
		getVAO()->getvTexel()[3].y = (8.0f - iy) / 8.0f;

		//Mapeamos el buffer de texels para acceder a la textura que representa la letra elegida
		glBindBuffer(GL_ARRAY_BUFFER, getVAO()->getVBOHandesAt(1));

		glm::vec2 *data = static_cast<glm::vec2*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
		if(data != nullptr)
		{
			std::copy(getVAO()->getvTexel().begin(), getVAO()->getvTexel().end(), data);
		}

		//Finaliza la modificación del buffer
		glUnmapBuffer(GL_ARRAY_BUFFER);

		//Crea la matrix Model-View-Projection
		*m_model = glm::translate(glm::mat4(1.0), glm::vec3(0.0 + (m_cellWidth * despx) + 1, 0.0 - despy, 0.0));
		*m_mvp = *m_projection * (*m_view) * (*m_model);

		//Active the texture
		glActiveTexture(GL_TEXTURE0 + m_uiTextura[0]);
		glBindTexture(GL_TEXTURE_2D, m_uiTextura[0]);

		//Get the uniform to textureFonts
		ret = glGetUniformLocation(getShaderProgramId(), "m_textureA");
		glUniform1i(ret, m_uiTextura[0]);

		//Transformation Matrix
		ret = glGetUniformLocation(getShaderProgramId(), "mvp");
		glUniformMatrix4fv(ret, 1, GL_FALSE, glm::value_ptr(*m_mvp));

		glBindVertexArray(getVAO()->getVaoHandler());
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);

		despx++;
	} //End-for (GLuint i = 0; i < m_iNumFonts; i++)
}

void CText::setIdTexture(GLuint n, GLuint id)
{
	m_uiTextura[n] = id;
}

std::shared_ptr<glm::mat4>& CText::getProjectionMatrix()
{
	return m_projection;
}

std::shared_ptr<glm::mat4>& CText::getModelMatrix()
{
	return m_model;
}

std::shared_ptr<glm::mat4>& CText::getViewMatrix()
{
	return m_view;
}

std::shared_ptr<glm::mat4>& CText::getMVPMatrix()
{
	return m_mvp;
}

CText::~CText()
{

}
