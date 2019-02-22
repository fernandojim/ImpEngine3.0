#include "CTerrain.h"
#include "MaterialManager.h"
#include "CXMLParser.h"
#include "CRenderingSystem.h"
#include "CRenderingComponent.h"
#include "utils.h"

using namespace Engine::Graphics;
using namespace Engine::Managers;

#define PUNTOS_ALREDEDOR 6

//------------------------------------------
// Constructor by file
//------------------------------------------
CTerrain::CTerrain(const std::string& name, GLuint shaderid, const std::string& file) : CRenderingComponent(name, shaderid)
{
	GLuint x, z, k, vertex_size, index_size, subindex_size;
	float scale = 0.0;
	float rgb_max = 0.0;
	glm::vec3 vertex;
	glm::vec2 texel;
	glm::vec3 normal;
	GLuint index;
	std::unique_ptr<CXMLParser> parser(new CXMLParser);

	// Read the content of file
	if (parser->LoadXMLFile(file))
	{
		if (parser->getNodes("TERRAIN_MESH"))
		{
			//Get name and id
			m_cellWide = std::stof(parser->getAttributeValue("CELL_WIDE"));
			m_maxElevation = std::stof(parser->getAttributeValue("MAX_ELEVATION"));
			m_heightfieldTexture = parser->getAttributeValue("HEIGHTFIELD");
			m_materialsName = parser->getAttributeValue("MATERIALS");

			m_tilingFactorSand = 0.01;
			m_tilingFactorGrass = 0.01;
			m_tilingFactorDirt = 0.01;
			m_tilingFactorSnow = 0.01;

			/* Load the materials from file */
			Engine::Managers::MaterialManager::getSingleton().loadMaterialsFromMTL(m_materialsName);

			/* Store the materials loadad into terrain data */
			//m_MatSand = std::unique_ptr<CMaterial>(*::getMaterialManager().getMaterialByName(std::string("sand")));
			//m_MatGrass = std::unique_ptr<CMaterial>(*::getMaterialManager().getMaterialByName(std::string("grass")));
			//m_MatDirt = std::unique_ptr<CMaterial>(*::getMaterialManager().getMaterialByName(std::string("dirt")));
			//m_MatSnow = std::unique_ptr<CMaterial>(*::getMaterialManager().getMaterialByName(std::string("snow")));

			//Carga la imagen del mapa de alturas
			m_pHeightMap = new CBmp(m_heightfieldTexture.c_str());

			//Test if the BMP file heightmap was OK and it is an square
			if (m_pHeightMap && m_pHeightMap->getResult() && m_pHeightMap->isSquare())
			{
				//The info will be presented as a grid with m_widthCells x m_heightCells. It will be square
				// For example:
				//   ...
				//   ...
				//   ...
				//   heigh=3
				//   width=3
				m_numCells = m_pHeightMap->getWidth();

				//Calculate the total num of vertex, texels and index
				vertex_size = m_numCells * m_numCells;
				subindex_size = m_numCells * 2;
				index_size = subindex_size  * (m_numCells - 1);

				//Alloc the lists
				getVAO()->setNumVertex(vertex_size);
				getVAO()->setNumIndex(index_size);
				getVAO()->setOffset(subindex_size);
				getVAO()->setElements(subindex_size - 1);
				getVAO()->getvVextex3V3N2NP().reserve(vertex_size);
				getVAO()->getvIndex().reserve(index_size);

				//Gets the max value of RGB
				k = 0;
				rgb_max = 0.0;
				for (x = 0; x < m_numCells; x++)
				{
					for (z = 0; z < m_numCells; z++)
					{
						if (m_pHeightMap->getPixelData()[k * 3] > rgb_max)
							rgb_max = m_pHeightMap->getPixelData()[k * 3];

						k++;
					}
				}

				//escala la altura máxima del terreno a la altura introducida por el parámetro 'altura'
				scale = m_maxElevation / rgb_max;

				//Create a vertex grid for the text
				Engine::Graphics::Vextex3V3N2NP v;
				k = 0;
				for (x = 0; x < m_numCells; x++)
				{
					for (z = 0; z < m_numCells; z++)
					{
						//Create the vertex
						v.vertex.x = x * m_cellWide;
						v.vertex.y = m_pHeightMap->getPixelData()[k++ * 3] * scale;
						v.vertex.z = z * m_cellWide;

						//Create the texel
						v.texel.x = static_cast<float>(x);
						v.texel.y = static_cast<float>(z);

						getVAO()->addVextex3V3N2NP(v);
					}
				}

				/*
				It´s time to create the index to referenciate the vertexs
				1---3---4
				|\  |\  |
				^ \	^ \ ^
				|  \|   |
				0---2---5
				Type: GL_TRIANGLE_STRIP
				 */
				for (z = m_numCells - 1;z-- > 0;)
				{
					for (x = 0; x < m_numCells; x++)
					{
						index = z + (x * (m_numCells));
						getVAO()->addIndex(index);

						index = (z + 1) + (x * (m_numCells));
						getVAO()->addIndex(index);
					}
				}

				//After creating the
				for (GLuint cont = 0; cont < getVAO()->getNumVertex(); cont++)
				{
					v.normal.x = CalculaNormalVertice(cont).x;
					v.normal.y = CalculaNormalVertice(cont).y;
					v.normal.z = CalculaNormalVertice(cont).z;
					getVAO()->getvVextex3V3N2NP().at(cont).normal = v.normal;
				}

				//VERTEX ATTRIB OBJECT
				getVAO()->CreateVertexArrayObject();
				getVAO()->CreateArrayBuffer(getVAO()->getvVextex3V3N2NP().data(), getVAO()->getvVextex3V3N2NP().size() * sizeof(Engine::Graphics::Vextex3V3N2NP), GL_STATIC_DRAW);
				getVAO()->CreateVertexAttribPointer(shaderid, "Vertex", 3, sizeof(Engine::Graphics::Vextex3V3N2NP), 0);
				getVAO()->CreateVertexAttribPointer(shaderid, "Normal", 3, sizeof(Engine::Graphics::Vextex3V3N2NP), sizeof(Engine::Graphics::vector3));
				getVAO()->CreateVertexAttribPointer(shaderid, "Texel", 2, sizeof(Engine::Graphics::Vextex3V3N2NP), 2 * sizeof(Engine::Graphics::vector3));

				m_view = glm::lookAt(glm::vec3(100.0, 100.0, 100.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
				m_projection = glm::perspective(glm::radians(90.0f), (float)800/600, 0.1f, 8000.0f);
				m_model = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, 0.0));
				m_mvp = m_projection * m_view * m_model;
				glm::mat4 aux1 = glm::mat4(1.0);
				glm::mat3 aux2 = glm::mat3(1.0);

				//I don´t know if is better calculate the MVP in the CPU or GPU
				m_mvp = m_projection * m_view * m_model;

				//Configure the uniform to the Uniform Buffer Object
				size_t sz = sizeof(glm::mat4) * 2 + sizeof(glm::mat3);
				getUBO()->createUniformBlock(shaderid, "Matrices2", sz, 1);
				getUBO()->addData(0, glm::value_ptr(aux1), sizeof(aux1));
				getUBO()->addData(sizeof(aux1), glm::value_ptr(m_mvp), sizeof(glm::mat4));
				getUBO()->addData(sizeof(aux1) * 2, glm::value_ptr(aux2), sizeof(aux2));
			}
			else
				Message::MessageBOX("Terrain", "Heightfield bmp not valid: '" + getName() + "'");
		}
	}
}

CTerrain::~CTerrain()
{
	delete m_pHeightMap;
}

float CTerrain::GetAltura(float x, float z)
{
    glm::vec3 v1, v2, v3;
	glm::vec3 pos = glm::vec3(x, 0.0, z);
    float A,B,C,D;
    
    //Obtenemos los índices de los puntos del triángulo en el que está la pos x,z
    obtieneIndicesTriangulo(pos, v1, v2, v3);
    
    //Calculamos A, B, C, D de la ecuación Ax + By + Cz = D del plano
    //    A,B,C representa las magnitudes de la normal del triángulo calculado
    A = v1.y * (v2.z - v3.z) + v2.y * (v3.z - v1.z) + v3.y * (v1.z - v2.z);
    B = v1.z * (v2.x - v3.x) + v2.z * (v3.x - v1.x) + v3.z * (v1.x - v2.x);
    C = v1.x * (v2.y - v3.y) + v2.x * (v3.y - v1.y) + v3.x * (v1.y - v2.y);
    D = -(v1.x * (v2.y * v3.z - v3.y * v2.z) + v2.x * (v3.y * v1.z - v1.y * v3.z) + v3.x * (v1.y * v2.z - v2.y * v1.z));
    
    //Y ahora la coordenada Y de nuestro punto x,y dentro de ese plano calculado
    if (B == 0.0f) 
       B = 0.1f;
       
    return -(D + A * x + C * z) / B;
}

GLuint CTerrain::getIndice(GLuint x, GLuint z)
{
     //(x, z) indica la coordenada de vértice de la rejilla de vértices
     return (z + (x * m_numCells));
}
   
void CTerrain::setAltura(GLuint indice, float altura)
{
     float alt;
     
	 if (indice < 0 || indice > getVAO()->getNumVertex()) //Sale del rango
        return;
     else
     {
         alt = altura * 4 / 5;
         //getVAO()->getvVertex()[indice].y += alt;
         getVAO()->getvVextex3V3N2NP()[indice].vertex.y += alt;
         
         if (puntoInterior(indice))
         {
        	 //getVAO()->getvVertex()[indice - 1].y += altura * 1 / 5;
        	 getVAO()->getvVextex3V3N2NP()[indice - 1].vertex.y += altura * 1 / 5;
             
             //Nos aseguramos que no sobrepasar el número de vértices
			 if (indice < getVAO()->getNumVertex() - 1)
				 //getVAO()->getvVertex()[indice + 1].y += altura * 1 / 5;
			 	 getVAO()->getvVextex3V3N2NP()[indice + 1].vertex.y += altura * 1 / 5;
             
			 //getVAO()->getvVertex()[indice - m_numCells].y += altura * 1 / 5;
			 getVAO()->getvVextex3V3N2NP()[indice - m_numCells].vertex.y += altura * 1 / 5;
             
             //Nos aseguramos que no sobrepasar el número de vértices
			 if (indice < getVAO()->getNumVertex() - m_numCells - 1)
				 //getVAO()->getvVertex()[indice + m_numCells].y += altura * 1 / 5;
			 	 getVAO()->getvVextex3V3N2NP()[indice + m_numCells].vertex.y += altura * 1 / 5;
         }
     }
}

bool CTerrain::puntoInterior(GLuint ind)
{
	GLuint x, z; //Coordenadas x, z calculadas a partir del indice 'ind'
     
	if (ind < 0 || ind > getVAO()->getNumVertex())
		return false;
    else
    {
        //Calcula las coordenadas a partir del índice
        z = ind % m_numCells;
        x = ind / m_numCells;
         
        //Comprobamos que la coordenada x,z corresponde a un punto interior
        if (x > 0 && x < m_numCells && z > 0 && z < m_numCells)
           return true;
    }
    
	return false;
}

glm::vec3 CTerrain::calculaNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	glm::vec3 vector1;
	glm::vec3 vector2;

	/* Creamos los vectores según los puntos introducidos */
	vector1 = p2 - p1;
    vector2 = p3 - p1;

	/* El producto vectorial entre dos vectores es la normal al plano que forman */
	return glm::normalize(glm::cross(vector1, vector2));
}

void CTerrain::obtieneIndicesTriangulo(glm::vec3 pos, glm::vec3 &v1, glm::vec3 &v2, glm::vec3 &v3)
{
     //Obtenemos las coordenadas de vértices según la posición absoluta
	GLuint xdiv = static_cast<GLuint>(fabs(pos.x / m_cellWide));
	GLuint zdiv = static_cast<GLuint>(fabs(pos.z / m_cellWide));
     
    //Comprobamos que estamos dentro del terreno
    if (xdiv < 0 || xdiv > m_numCells - 1 || zdiv < 0 || zdiv > m_numCells - 1)
       return;
     
    //Calculamos la posicion relativa a la celda en la que estamos
    float pos_rel_x = pos.x - ( (float)xdiv * m_cellWide );
    float pos_rel_z = pos.z - ( (float)zdiv * m_cellWide );
     
    //Obtenemos el triangulo en el que está la posición
	GLint triangulo = 0;
	if ( pos_rel_z > pos_rel_x )
       triangulo = 1;
        
	//triangulo = 1 implica que del siguiente modelo de celda del terreno
	//   3 --- 2
	//   |   / |
	//   | /   |
	//   1 --- 0
	//   tomaremos el triángulo formado por los vértices 0-1-3.
	//   El triángulo 0 esta formado entonces por 2-0-3.
	//   ya que nuestro punto x,z cae dentro de dicho triángulo
	//   por haberlo sacado de los cálculos anteriores
	//   ind1, ind1 e ind3 se refiere a los índices en la lista de vértices del terreno 
	//Calculamos los índices de los vértices del triángulo en el que estamos
	GLint p1, p2, p3;
    if (triangulo == 1) 
    {
		p1 = ( m_numCells * xdiv ) + zdiv;
        p2 = ( m_numCells * ( xdiv + 1 ) ) + zdiv + 1;
        p3 = ( m_numCells * xdiv ) + zdiv + 1;
    }
    else
    {
        p1 = ( m_numCells * ( xdiv + 1 ) ) + zdiv;
        p2 = ( m_numCells * xdiv ) + zdiv;
        p3 = ( m_numCells * ( xdiv + 1 ) ) + zdiv + 1;
    }
     
    //Ya tenemos los tres índices de los puntos del triángulo en el que está la coordenada x,z
    //   Mediante la ecuación del plano, obtenemos la coordenada Y de nuestro punto x,z
	//v1 = glm::vec3(getVAO()->getvVertex()[p1].x, getVAO()->getvVertex()[p1].y, getVAO()->getvVertex()[p1].z);
	//v2 = glm::vec3(getVAO()->getvVertex()[p2].x, getVAO()->getvVertex()[p2].y, getVAO()->getvVertex()[p2].z);
	//v3 = glm::vec3(getVAO()->getvVertex()[p3].x, getVAO()->getvVertex()[p3].y, getVAO()->getvVertex()[p3].z);

	v1 = glm::vec3(getVAO()->getvVextex3V3N2NP()[p1].vertex.x, getVAO()->getvVextex3V3N2NP()[p1].vertex.y, getVAO()->getvVextex3V3N2NP()[p1].vertex.z);
	v2 = glm::vec3(getVAO()->getvVextex3V3N2NP()[p2].vertex.x, getVAO()->getvVextex3V3N2NP()[p2].vertex.y, getVAO()->getvVextex3V3N2NP()[p2].vertex.z);
	v3 = glm::vec3(getVAO()->getvVextex3V3N2NP()[p3].vertex.x, getVAO()->getvVextex3V3N2NP()[p3].vertex.y, getVAO()->getvVextex3V3N2NP()[p3].vertex.z);
}

glm::vec3 CTerrain::getNormalAtPos(glm::vec3 pos)
{
    glm::vec3 v1, v2, v3;
	     
	/* Se calculan los puntos del triángulo en el que cae la pos */
    obtieneIndicesTriangulo(pos, v1, v2, v3);
             
    /* Obtenemos su normal */
	return calculaNormal(v1, v2, v3);
}

float CTerrain::getFronteraX()
{
	return m_numCells * m_cellWide;
}

float CTerrain::getFronteraZ()
{
	return m_numCells * m_cellWide;
}

float CTerrain::CalculaAlturaMax()
{
	float alt = 0.0;

	for (GLuint i = 0; i < getVAO()->getNumVertex(); i++)
	{
		if (getVAO()->getvVertex()[i].y > alt)
		{
			//alt = getVAO()->getvVertex()[i].y;
			alt = getVAO()->getvVextex3V3N2NP()[i].vertex.y;
		}
	}

	return alt;
}

glm::vec3 CTerrain::CalculaNormalVertice(GLuint iVertice)
{
	GLuint indices[6]; // 6 vértices adyacentes como máximo
	glm::vec3 vertices[6];
	glm::vec3 vector[7];
	glm::vec3 vec[6];
	GLuint nIndices = 0;
	GLuint nVertices = 0;
	GLuint nVectores = 0;
	glm::vec3 v0;
		
	//Inicializa la lista de índices
	for (GLint i = 0; i < 6; i++)
		indices[i] = -1;
	
	//Obtiene el punto origen
	//v0.x = getVAO()->getvVertex()[iVertice].x;
	//v0.y = getVAO()->getvVertex()[iVertice].y;
	//v0.z = getVAO()->getvVertex()[iVertice].z;

	v0.x = getVAO()->getvVextex3V3N2NP()[iVertice].vertex.x;
	v0.y = getVAO()->getvVextex3V3N2NP()[iVertice].vertex.y;
	v0.z = getVAO()->getvVextex3V3N2NP()[iVertice].vertex.z;
	
	//Según el número de divisiones y su posición se obtienen los vértices adyacentes
	if ( iVertice == 0 )
	{
		indices[0] = 1;
		indices[1] = m_numCells + 1;
		indices[2] = m_numCells;
		nIndices = 3;
	}
	else if ( iVertice > 0 && iVertice < m_numCells - 1 )
	{
		indices[0] = iVertice + 1;
		indices[1] = iVertice + m_numCells + 1;
		indices[2] = iVertice + m_numCells;
		indices[3] = iVertice - 1;
		nIndices = 4;
	}
	else if ( iVertice == m_numCells - 1 )
	{
		indices[0] = iVertice + m_numCells;
		indices[1] = iVertice - 1;
		nIndices = 2;
	}
	else if ( (iVertice > 0) && (iVertice % m_numCells == 0) && (iVertice < m_numCells * (m_numCells - 1)) )
	{
		indices[0] = iVertice - m_numCells;
		indices[1] = iVertice + 1;
		indices[2] = iVertice + m_numCells + 1;
		indices[3] = iVertice + m_numCells;
		nIndices = 4;
	}
	else if ( (iVertice > m_numCells) && ((iVertice + 1) % m_numCells == 0) && (iVertice < (m_numCells*m_numCells - 1)) )
	{
		indices[0] = iVertice + m_numCells;
		indices[1] = iVertice - 1;
		indices[2] = iVertice - m_numCells - 1;
		indices[3] = iVertice - m_numCells;
		nIndices = 4;
	}
	else if ( iVertice == m_numCells * (m_numCells - 1) )
	{
		indices[0] = iVertice - m_numCells;
		indices[1] = iVertice + 1;
		nIndices = 2;
	}
	else if ( iVertice > m_numCells * (m_numCells - 1) && iVertice < m_numCells * m_numCells - 1 )
	{
		indices[0] = iVertice - 1;
		indices[1] = iVertice - m_numCells - 1;
		indices[2] = iVertice - m_numCells;
		indices[3] = iVertice + 1;
		nIndices = 4;
	}
	else if ( iVertice == m_numCells * m_numCells - 1 )
	{
		indices[0] = iVertice - 1;
		indices[1] = iVertice - m_numCells - 1;
		indices[2] = iVertice - m_numCells;
		nIndices = 3;
	}
	else
	{
		indices[0] = iVertice + 1;
		indices[1] = iVertice + m_numCells + 1;
		indices[2] = iVertice + m_numCells;
		indices[3] = iVertice - 1;
		indices[4] = iVertice - m_numCells - 1;
		indices[5] = iVertice - m_numCells;
		nIndices = 6;
	}

	//Obtiene las coordenadas de los vértices con los índices calculados
	// los guarda en la lista 'vertices'
	for (GLuint i = 0; i < nIndices; i++)
	{
		//Comprueba si está dentro de la rejilla del terreno
		if ( indices[i] >= 0 )
		{
			//vertices[nVertices].x = getVAO()->getvVertex()[indices[i]].x;
			//vertices[nVertices].y = getVAO()->getvVertex()[indices[i]].y;
			//vertices[nVertices].z = getVAO()->getvVertex()[indices[i]].z;

			vertices[nVertices].x = getVAO()->getvVextex3V3N2NP()[indices[i]].vertex.x;
			vertices[nVertices].y = getVAO()->getvVextex3V3N2NP()[indices[i]].vertex.y;
			vertices[nVertices].z = getVAO()->getvVextex3V3N2NP()[indices[i]].vertex.z;

			// Siguiente vértice
			nVertices++;
		}
	}

	if ( nVertices >= 2)
	{
		vector[0] = v0;
		for (GLuint i = 1; i <= nVertices; i++)
		{
			vector[i] = vertices[i-1];
		}

		nVectores = nVertices;
	}

	for (GLuint j = 0; j<nVectores; j++)
	{
		vec[j] = vector[j + 1] - vector[0];
	}

	GLuint l = 0;
	glm::vec3 dot[6];
	for (GLuint k = 0; k < nVectores - 1; k++)
	{
		dot[l] = glm::normalize(glm::cross(vec[k], vec[k + 1]));
		
		l++;
	}

	//En caso de ser un punto interior, tenemos 6 puntos cercanos, hay que 
	// tener en cuenta 6 caras para calcular la normal.
	if (nVectores == 6)
	{
		dot[l] = glm::normalize(glm::cross(vec[5], vec[0]));
	}

	glm::vec3 normal;
	for (GLuint m = 0; m < l; m++)
	{
		normal = normal + dot[m];
	}

	normal = glm::normalize(normal);

	return normal;
}

void CTerrain::setLights(const CLight& lights)
{
	//m_lights = lights;
}

void CTerrain::ReceiveEvent(void *buff)
{

}
