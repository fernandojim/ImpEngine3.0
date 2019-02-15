/*******************************************
  File:       CVao.h
  Purpose:    Class for Vertex Attrib Object
              Information will be stored in
              the graphics hardware and used
              by the shaders.

  class name: CVao
  ------------------------------------------
  @Author: Yo
  @Version: 0
 *******************************************/

#ifndef CVAO_H_
#define CVAO_H_

#ifdef _OPENGL44_
 #include "gl_core_4_4.h"
#else
 #include "gl_core_3_3.h"
#endif

#include <iostream>
#include <vector>

#include "glm\vec2.hpp"
#include "glm\vec3.hpp"
#include "CObject.h"

namespace Engine
{
	namespace Graphics
	{
		struct vector2
		{
			GLfloat x;
			GLfloat y;
		};

		struct vector3
		{
			GLfloat x;
			GLfloat y;
			GLfloat z;
		};

		/*
		 * 3 float to vertex
		 * 3 float to normal
		 * 2 float to texel
		 * Total: 32 bytes
		 */
		struct Vextex3V3N2NP
		{
			vector3 vertex;
			vector3 normal;
			vector2 texel;
		};

		struct face
		{
			GLuint nverts;
			GLuint vertex[4];
			GLuint texel[4];
			GLuint normal[4];
		};

		class CVAO : public Engine::Base::CObject
		{
		public:
			CVAO();
			CVAO(const std::string& name);
			~CVAO();

		   /********************************************************************************************
			* Clear the internal info
			********************************************************************************************/
			void Clear();

		   /********************************************************************************************
			* Creates the id for the Arrays
			********************************************************************************************/
			void CreateVertexArrays();

		   /********************************************************************************************
			* Creates the Array Buffer
			********************************************************************************************/
			void CreateArrayBuffer(GLvoid *buffer, GLuint size, GLenum usage);

			void CreateVertexAttribPointer(GLuint program_id, const std::string& attrib_name, GLuint elements, size_t sz, GLuint offset);

		   /********************************************************************************************
			* Creates an Attrib to given Array Buffer
			* That depends on the shader layout location for that array (attrib_id)
			********************************************************************************************/
			void CreateAttribArrayBuffer(GLuint attrib_id, GLuint elements, void *buffer, GLuint size, GLenum usage);

		   /********************************************************************************************
			* Get/set methods for the internal info
			********************************************************************************************/
			GLuint getVaoHandler(); //VAO handler
			GLuint getNumVertex();  //Num vertex
			GLuint getNumIndex();   //Num index
			GLuint getOffset();     //Offsets to render
			GLuint getElements();   //Elements to render

			void setNumVertex(GLuint n);
			void setNumIndex(GLuint n);
			void setOffset(GLuint n);
			void setElements(GLuint n);

			//Getters (Despite the rvalue functions)
			std::vector<glm::vec3>& getvVertex();
			std::vector<glm::vec3>& getvNormal();
			std::vector<glm::vec2>& getvTexel();
			std::vector<face>& getvFace();
			std::vector<GLuint>& getvIndex();

			//Get the Vextex3V3N2NP array
			std::vector<Vextex3V3N2NP>& getvVextex3V3N2NP();

			//Adding methods
			void addVertex(const glm::vec3 v);
			void addNormal(const glm::vec3 n);
			void addTexel(const glm::vec2 t);
			void addIndex(GLuint i);

			//Add an element to the Vextex3V3N2NP array
			void addVextex3V3N2NP(const Vextex3V3N2NP& v);

			GLuint getVBOHandesAt(GLuint buffer_id);

			void debug();

		private:
		   /********************************************************************************************
			* Buffers and their sizes to store in the graphics card to pass to the shaders
			********************************************************************************************/
			GLuint m_numVertex;
			GLuint m_numIndex;

			//Itterations using GL_TRIANGLE_STRIP (for grids)
			GLuint m_elements;    //Elements per draw command
			GLuint m_offset;      //offset to apply

			//Std vectors
			std::vector<GLuint>    m_Index;
			std::vector<glm::vec3> m_Vertex;
			std::vector<glm::vec3> m_Normal;
			std::vector<glm::vec2> m_Texel;
			std::vector<face>      m_Faces;

			//Vertex with vertex, normal and texel information
			std::vector<Vextex3V3N2NP> m_Vextex3V3N2NP;

		   /********************************************************************************************
			* Handlers for both to manage the VAO and its buffers
			********************************************************************************************/
			GLuint m_arrayhandler;
			GLuint m_vaoHandle;
			static const int MaxBuffers = 6;
			GLuint m_vboHandles[MaxBuffers];
		};
	};
};

#endif
