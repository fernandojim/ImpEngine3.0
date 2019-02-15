/********************************************
  File:       CUbo.h
  Purpose:    Class for Uniform Buffer Object
              Information will be stored in
              the graphics hardware
  class name: CUbo
  -------------------------------------------
  @Author: Yo
  @Version: 0
 ********************************************/

#ifndef CUBO_H_
#define CUBO_H_

#ifdef _OPENGL44_
 #include "gl_core_4_4.h"
#else
 #include "gl_core_3_3.h"
#endif

#include <iostream>
#include <string>

#include "glm\vec2.hpp"
#include "glm\vec3.hpp"
#include "CObject.h"

namespace Engine
{
	namespace Graphics
	{
		class CUBO : public Engine::Base::CObject
		{
		public:
			CUBO();
			CUBO(const std::string& name);
			~CUBO();

		   /********************************************************************
			* Creates the Uniform Block
			* program_id : the shader program who use the UBO
			* uniform_block : name of the block in the shader
			* size : size of the block in bytes
			* binding : number of block the program shader will use
			********************************************************************/
			void createUniformBlock(GLuint program_id, const std::string& uniform_block, size_t size, GLuint binding);

		   /********************************************************************
			* Create and alloc the variables to uniform block
			********************************************************************/
			void addData(GLuint offset, const GLvoid * data, size_t size);

			GLuint getUboIndex();

		private:
			GLuint m_uboIndex;        //Ubo index
			GLuint m_uboHandler;      //Ubo handler
			GLuint m_uboBindingPoint; //Ubo binding point
		};

	}
}

#endif
