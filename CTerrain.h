/*********************************************
  File:       CTerrain.h
  Purpose:    Class for Terrain object
  class name: CTerrain derived from C3DObject
  --------------------------------------------
  @Author: Yo
  @Version: 0
 *********************************************/

#ifndef _CTERRAIN_H_
#define _CTERRAIN_H_

#include <string>

#define GLM_FORCE_RADIANS

#include "glm\vec3.hpp"
#include "glm\vec2.hpp"
#include "glm\gtc\type_ptr.hpp"	//Header for glm::value_ptr

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp" //Header for glm::perspective, etc...

#include "CBmp.h"
#include "CGameObject.h"
#include "CMaterial.h"
#include "CRenderingComponent.h"

#include "nvertex.h"

using namespace NVERTEX;

class CLight;

const unsigned int MaxTextures = 5;

namespace Engine
{
	namespace Graphics
	{
		class CTerrain : public Engine::Component::CRenderingComponent
		{
		public:
			CTerrain(const std::string& name, GLuint shaderid, const std::string& file);
			~CTerrain();

			GLuint m_numCells;     //Num of cells x and z axis (square)
			float  m_cellWide;     //Wide of square cell
			float  m_maxElevation; //Maximum elevation (y axis)
			float  m_tilingFactor;
			std::string m_heightfieldTexture;

			//Bmp con el mapa DTED2 para las alturas
			CBmp	  *m_pHeightMap;

			bool addTexture(GLuint id);

			std::unique_ptr<CMaterial> 	m_MatSand;
			std::unique_ptr<CMaterial> 	m_MatGrass;
			std::unique_ptr<CMaterial> 	m_MatDirt;
			std::unique_ptr<CMaterial> 	m_MatSnow;

			//Tiling texture factors
			GLfloat		m_tilingFactorSand;
			GLfloat		m_tilingFactorGrass;
			GLfloat		m_tilingFactorDirt;
			GLfloat		m_tilingFactorSnow;

			//Materials - one material per texture
			std::string m_materialsName;

			void    setLights(const CLight& lights);

			void    obtieneIndicesTriangulo(glm::vec3 pos, glm::vec3 &v1, glm::vec3 &v2, glm::vec3 &v3);
			float   GetAltura(float x, float z);    //Dando las coordenadas (x, z), obtiene la coordenada y
			GLuint  getIndice(GLuint x, GLuint z);          //Obtenemos el indice del vértice dando el vértice (x,z)
			void    setAltura(GLuint indice, float altura);           //Damos altura al vértice referenciado por 'indice'
			bool    puntoInterior(GLuint ind);
			glm::vec3 calculaNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
			glm::vec3 getNormalAtPos(glm::vec3 pos);
			float   getFronteraX();
			float   getFronteraZ();
			float   CalculaAlturaMax();
			glm::vec3 CalculaNormalVertice(GLuint iVertice);

			void Animate(double dt);
			void Update();
		};
	}
}

#endif
