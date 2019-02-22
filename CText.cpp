#include <cstring>

#include "CText.h"
#include "CTexture.h"
#include "TextureManager.h"
#include "CXMLParser.h"
#include "CRenderingSystem.h"
#include "GameObjectManager.h"

using namespace Engine::Graphics;

CText::CText(const std::string& name, GLuint shaderid, const std::string& file) : CRenderingComponent(name, shaderid),
		m_cellWidth(0.0),
		m_gridHeight(0),
		m_gridWidth(0),
		m_textureFonts(""),
		m_textureFontsGridX(0),
		m_textureFontsGridY(0),
		m_fontPixelsX(0),
		m_fontPixelsY(0)
{
	GLint x,y,size,index;
	std::unique_ptr<CXMLParser> parser(new CXMLParser);

	m_charCodes.clear();
	m_charTextures.clear();

	// Read the content of file
	if (loadValues(file))
	{
		//Calcule total num of vertex and texels and reserve it
		//Size is the number of vertex
		size = 4 * m_gridHeight * m_gridWidth;
		getVAO()->setNumVertex(size);
		getVAO()->getvVextex3V3N2NP().reserve(size);

		//Set offsets - info for render loop
		//Each quad has 4 elements
		getVAO()->setOffset(4);

		//Number total of quads
		getVAO()->setElements(m_gridHeight * m_gridWidth);

		//Total num of index, in this case they are (m_gridHeight * m_gridWidth) quads
		index = 4 * m_gridHeight * m_gridWidth;
		getVAO()->setNumIndex(index);
		getVAO()->getvIndex().reserve(index);

		//Create each quad starting from the upper left corner.
		Engine::Graphics::Vextex3V3N2NP v;
		for (y = m_gridHeight;y-- > 0;)
		{
			for (x = 0; x <= m_gridWidth-1; x++)
			{
				//Vertex: 0
				v.vertex.x = x * m_cellWidth;
				v.vertex.y = y * m_cellWidth;
				v.vertex.z = 0.0;
				v.texel.x = 0.0;
				v.texel.y = 0.0;
				getVAO()->addVextex3V3N2NP(v);

				//Vertex: 1
				v.vertex.x = x * m_cellWidth;
				v.vertex.y = (y + 1) * m_cellWidth;
				v.vertex.z = 0.0;
				v.texel.x = 0.0;
				v.texel.y = 0.0;
				getVAO()->addVextex3V3N2NP(v);

				//Vertex: 2
				v.vertex.x = (x + 1) * m_cellWidth;
				v.vertex.y = y * m_cellWidth;
				v.vertex.z = 0.0;
				v.texel.x = 0.0;
				v.texel.y = 0.0;
				getVAO()->addVextex3V3N2NP(v);

				//Vertex: 3
				v.vertex.x = (x + 1) * m_cellWidth;
				v.vertex.y = (y + 1) * m_cellWidth;
				v.vertex.z = 0.0;
				v.texel.x = 0.0;
				v.texel.y = 0.0;
				getVAO()->addVextex3V3N2NP(v);
			}
		}

		/*It´s time to create the index to referenciate the vertexs
		 * For that, we need to create index to create independant quads
		 * where each character will be renderized.
		1---3---5     1---3  3---5
		|\  |\  |     |\  |  |\  |
		^ \	^ \ ^ =>  ^ \ ^  ^ \ ^
		|  \|   |     |  \|  |  \|
		0---2---4     0---2  2---4
		Type: GL_TRIANGLE_STRIP
		*/
		index = 0;
		for (y = 0; y < m_gridHeight; y++)
		{
			for (x = 0; x < m_gridWidth; x++)
			{
				//index = y + (x * m_gridHeight);
				getVAO()->addIndex(index);
				getVAO()->addIndex(index + 1);
				getVAO()->addIndex(index + 2);
				getVAO()->addIndex(index + 3);

				index = index + 2;
			}

			index = index + 2;
		}

		//VERTEX ATTRIB OBJECT
		getVAO()->CreateVertexArrayObject();
		getVAO()->CreateArrayBuffer(getVAO()->getvVextex3V3N2NP().data(), getVAO()->getvVextex3V3N2NP().size() * sizeof(Engine::Graphics::Vextex3V3N2NP), GL_STATIC_DRAW);
		getVAO()->CreateVertexAttribPointer(shaderid, "Vertex", 3, sizeof(Engine::Graphics::Vextex3V3N2NP), 0);
		getVAO()->CreateVertexAttribPointer(shaderid, "Texel", 2, sizeof(Engine::Graphics::Vextex3V3N2NP), 2 * sizeof(Engine::Graphics::vector3));

		//UNIFORM BUFFER OBJECT
		//-------------------------------------------------------------------------------------------------
		//Creates the ortographical projection. The view is fixed
		m_view = glm::lookAt(glm::vec3(0.0, 0.0, 90.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
		m_projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 100.0f, -100.0f);
		m_model = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, 0.0));

		//I don´t know if is better calculate the MVP in the CPU or GPU
		//m_mvp = m_projection * m_view * m_model;

		//Configure the uniform to the Uniform Buffer Object
		getUBO()->createUniformBlock(shaderid, "Matrices", 3 * sizeof(glm::mat4), 0);
		getUBO()->addData(0, glm::value_ptr(m_view), sizeof(glm::mat4));
		getUBO()->addData(sizeof(glm::mat4), glm::value_ptr(m_projection), sizeof(glm::mat4));
		getUBO()->addData(2 * sizeof(glm::mat4), glm::value_ptr(m_model), sizeof(glm::mat4));

		//-------------------------------------------------------------------------------------------------
		//Load the text texture
		m_uiFontsTexture = Engine::Managers::TextureManager::getSingleton().loadTexture(m_textureFonts);
	}

	//text((const char*)Engine::Managers::GameObjectManager::getGameObjectManagerInstance().messages[0].buffer);
}

void CText::text(const std::string& tx)
{
	GLuint i = 0;
	GLuint offset = 2;
	GLuint offset_texture = 0;
	GLuint texture_index = 0;
	GLubyte c = 0;
	GLuint ele = getVAO()->getElements();

	//Traverses the list of string since the number of elements to display
	while (i < ele && i < tx.length())
	{
		c = tx.at(i);

		if (c >= 65 && c <= 90) //Characters A to Z
		{
			texture_index = c - 65; //Thus, the code will start at 0 code ('A')
		}
		else if (c >= 48 && c <= 57) //Characters 0 to 9
		{
			texture_index = c - 22;
		}
		else if (c == 20) //Space
		{
			texture_index = m_charTextures.size(); //last texture index
		}
		else // future assignations
		{
			texture_index = m_charTextures.size();
		}

		//Read the texture coordinates for each character and store it in the vertex buffers
		offset = i * 4;
		offset_texture = texture_index * 4;
		getVAO()->getvVextex3V3N2NP().at(offset).texel.x = m_charTextures.at(offset_texture).x;
		getVAO()->getvVextex3V3N2NP().at(offset).texel.y = m_charTextures.at(offset_texture).y;

		getVAO()->getvVextex3V3N2NP().at(offset+1).texel.x = m_charTextures.at(offset_texture+1).x;
		getVAO()->getvVextex3V3N2NP().at(offset+1).texel.y = m_charTextures.at(offset_texture+1).y;

		getVAO()->getvVextex3V3N2NP().at(offset+2).texel.x = m_charTextures.at(offset_texture+2).x;
		getVAO()->getvVextex3V3N2NP().at(offset+2).texel.y = m_charTextures.at(offset_texture+2).y;

		getVAO()->getvVextex3V3N2NP().at(offset+3).texel.x = m_charTextures.at(offset_texture+3).x;
		getVAO()->getvVextex3V3N2NP().at(offset+3).texel.y = m_charTextures.at(offset_texture+3).y;

		i++;
	}

	//Bind the buffer with id=0 (Vextex3V3N2NP buffer) and map the buffer to modify its values
	getVAO()->BindBuffer(0);
	getVAO()->MapBuffer();
	glUnmapBuffer(GL_ARRAY_BUFFER);
}

bool CText::loadValues(const std::string& file)
{
	bool res = false;
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
			m_textureFonts = parser->getAttributeValue("TEXTURE_FONTS");
			m_textureFontsGridX = std::atoi(parser->getAttributeValue("TEXTURE_FONTS_GRID_X").c_str());
			m_textureFontsGridY = std::atoi(parser->getAttributeValue("TEXTURE_FONTS_GRID_Y").c_str());
			m_fontPixelsX = std::atoi(parser->getAttributeValue("FONTS_PIXELS_X").c_str());
			m_fontPixelsY = std::atoi(parser->getAttributeValue("FONTS_PIXELS_Y").c_str());
			m_charCodes.push_back(std::atoi(parser->getAttributeValue("POS_A").c_str()));
			m_charCodes.push_back(std::atoi(parser->getAttributeValue("POS_B").c_str()));
			m_charCodes.push_back(std::atoi(parser->getAttributeValue("POS_C").c_str()));
			m_charCodes.push_back(std::atoi(parser->getAttributeValue("POS_D").c_str()));
			m_charCodes.push_back(std::atoi(parser->getAttributeValue("POS_E").c_str()));
			m_charCodes.push_back(std::atoi(parser->getAttributeValue("POS_F").c_str()));
			m_charCodes.push_back(std::atoi(parser->getAttributeValue("POS_G").c_str()));
			m_charCodes.push_back(std::atoi(parser->getAttributeValue("POS_H").c_str()));
			m_charCodes.push_back(std::atoi(parser->getAttributeValue("POS_I").c_str()));
			m_charCodes.push_back(std::atoi(parser->getAttributeValue("POS_J").c_str()));
			m_charCodes.push_back(std::atoi(parser->getAttributeValue("POS_K").c_str()));
			m_charCodes.push_back(std::atoi(parser->getAttributeValue("POS_L").c_str()));
			m_charCodes.push_back(std::atoi(parser->getAttributeValue("POS_M").c_str()));
			m_charCodes.push_back(std::atoi(parser->getAttributeValue("POS_N").c_str()));
			m_charCodes.push_back(std::atoi(parser->getAttributeValue("POS_O").c_str()));
			m_charCodes.push_back(std::atoi(parser->getAttributeValue("POS_P").c_str()));
			m_charCodes.push_back(std::atoi(parser->getAttributeValue("POS_Q").c_str()));
			m_charCodes.push_back(std::atoi(parser->getAttributeValue("POS_R").c_str()));
			m_charCodes.push_back(std::atoi(parser->getAttributeValue("POS_S").c_str()));
			m_charCodes.push_back(std::atoi(parser->getAttributeValue("POS_T").c_str()));
			m_charCodes.push_back(std::atoi(parser->getAttributeValue("POS_U").c_str()));
			m_charCodes.push_back(std::atoi(parser->getAttributeValue("POS_V").c_str()));
			m_charCodes.push_back(std::atoi(parser->getAttributeValue("POS_W").c_str()));
			m_charCodes.push_back(std::atoi(parser->getAttributeValue("POS_X").c_str()));
			m_charCodes.push_back(std::atoi(parser->getAttributeValue("POS_Y").c_str()));
			m_charCodes.push_back(std::atoi(parser->getAttributeValue("POS_Z").c_str()));
			m_charCodes.push_back(std::atoi(parser->getAttributeValue("POS_0").c_str()));
			m_charCodes.push_back(std::atoi(parser->getAttributeValue("POS_1").c_str()));
			m_charCodes.push_back(std::atoi(parser->getAttributeValue("POS_2").c_str()));
			m_charCodes.push_back(std::atoi(parser->getAttributeValue("POS_3").c_str()));
			m_charCodes.push_back(std::atoi(parser->getAttributeValue("POS_4").c_str()));
			m_charCodes.push_back(std::atoi(parser->getAttributeValue("POS_5").c_str()));
			m_charCodes.push_back(std::atoi(parser->getAttributeValue("POS_6").c_str()));
			m_charCodes.push_back(std::atoi(parser->getAttributeValue("POS_7").c_str()));
			m_charCodes.push_back(std::atoi(parser->getAttributeValue("POS_8").c_str()));
			m_charCodes.push_back(std::atoi(parser->getAttributeValue("POS_9").c_str()));

			calculateTextureCoordinates();

			res = true;
		}
	}

	return res;
}

void CText::calculateTextureCoordinates()
{
	//Run the list and calculate the texture coordinates
	// depending on the char code and the dimensions of the texture
	Engine::Graphics::vector2 texel;
	GLint x,y;

	GLfloat texture_wide_x = 1.0 / m_textureFontsGridX;
	GLfloat texture_wide_y = 1.0 / m_textureFontsGridY;

	//First character => 'A' starts in the top left position
	x = 0;
	y = 0;
	for (auto c : m_charCodes)
	{
		/*Calculate left-bottom coordinates
		 *
		 *  1  3
		 *  |\ |
		 *  | \|
		 *  0  2
		 */
		//coordinate 0
		texel.x = texture_wide_x * x;
		texel.y = 1.0 - (texture_wide_y * (y + 1));
		m_charTextures.push_back(texel);

		//Coordinate 1
		texel.x = texture_wide_x * x;
		texel.y = 1.0 - (texture_wide_y * y);
		m_charTextures.push_back(texel);

		//Coordinate 2
		texel.x = (texture_wide_x * x) + texture_wide_x;
		texel.y = 1.0 - (texture_wide_y * (y + 1));
		m_charTextures.push_back(texel);

		//Coordinate 3
		texel.x = (texture_wide_x * x) + texture_wide_x;
		texel.y = 1.0 - (texture_wide_y * y);
		m_charTextures.push_back(texel);

		if (x < m_textureFontsGridX - 1)
			x++;
		else
		{
			x = 0;
			y++;
		}
	}
}

void CText::setIdTexture(GLuint id)
{
	m_uiFontsTexture = id;
}

void CText::ReceiveEvent(void *buff)
{
	text((const std::string&)buff);
}

CText::~CText()
{

}
