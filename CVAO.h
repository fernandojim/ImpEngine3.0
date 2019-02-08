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
			* Creates an Array Buffer
			* That´s it a buffer for vertex, texels, etc... an its type of usage (GL_STATIC_DRAW...)
			********************************************************************************************/
			void CreateArrayBuffer(void *buffer, GLuint size, GLenum usage);

		   /********************************************************************************************
			* Creates an Attrib to Array Buffer
			* That depends on the shader layout location for that array (attrib_id)
			********************************************************************************************/
			void CreateAttribArrayBuffer(GLuint attrib_id, GLuint elements);

		   /********************************************************************************************
			* Get/set methods for the internal info
			********************************************************************************************/
			GLuint getHandler();
			GLuint getVaoHandler();
			GLuint getNumVertex();
			GLuint getNumIndex();
			GLuint getNumSubIndex();

			//Getters (Despite the rvalue functions)
			std::vector<glm::vec3>& getvVertex();
			std::vector<glm::vec3>& getvNormal();
			std::vector<glm::vec2>& getvTexel();
			std::vector<face>& getvFace();
			std::vector<GLuint>& getvIndex();

			//Setters for user types instead of glm::vecX
			vector3* getv3Vertex();
			vector3* getv3Normal();
			vector2* getv2Texel();
			GLuint*  getv1Index();

			void setNumVertex(GLuint n);
			void setNumTexel(GLuint n);
			void setNumIndex(GLuint n);
			void setNumSubIndex(GLuint n);

			//Adding methods
			void addVertex(const glm::vec3 v);
			void addNormal(const glm::vec3 n);
			void addTexel(const glm::vec2 t);
			void addIndex(GLuint i);

			GLuint getVBOHandesAt(GLuint buffer_id);

			void debug();

		private:
		   /********************************************************************************************
			* Creates the id for the Arrays
			********************************************************************************************/
			void CreateVertexArrays();

		   /********************************************************************************************
			* Buffers and their sizes to store in the graphics card to pass to the shaders
			********************************************************************************************/
			GLuint m_numVertex;
			GLuint m_numTexel;
			GLuint m_numIndex;
			GLuint m_numSubIndex; //Each itteration using GL_TRIANGLE_STRIP (for grids)

			//Std vectors
			std::vector<GLuint>    m_Index;
			std::vector<glm::vec3> m_Vertex;
			std::vector<glm::vec3> m_Normal;
			std::vector<glm::vec2> m_Texel;
			std::vector<face>      m_Faces;

			//User types instead of glm::vecX
			GLuint                *m_iuIndex;
			vector3				  *m_v3Vertex;
			vector3				  *m_v3Normal;
			vector2				  *m_v2Texel;

		   /********************************************************************************************
			* Handlers for both to manage the VAO and its buffers
			********************************************************************************************/
			GLuint m_handler;
			GLuint m_vaoHandle;
			static const int MaxBuffers = 6;
			GLuint m_vboHandles[MaxBuffers];
		};
	};
};

#endif
