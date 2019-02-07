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
			********************************************************************/
			void createUniformBlock(GLuint program_id, const std::string& uniform_block);

		   /********************************************************************
		    * Add names to the uniform block
			********************************************************************/
			void addUniformNames(GLuint program_id, GLuint uniformCount, const GLchar** uniformNames);

		   /********************************************************************
			* Create and alloc the variables to uniforms
			********************************************************************/
			void activeUniformiv(GLuint program_id, GLuint n, const GLvoid * data, size_t size);
			void addData(GLuint offset, const GLvoid * data, size_t size);

		   /********************************************************************
			* Bind the UBO to pass to the shader program
			********************************************************************/
			void bindUBO();

		   /********************************************************************
			* Bind the UBO to pass to the shader program
			********************************************************************/
			void createUBO();

		   /********************************************************************
			* Get the Uniform Buffer Object handler
			********************************************************************/
			GLuint& getUBOHandler();

		   /********************************************************************
		    * Get the Uniform Buffer Object handler
			********************************************************************/
			GLubyte* getBuffer();

		private:
			static const GLuint max_uniforms = 10;

			GLubyte *m_buffer;    //Buffer where is alloc the Uniform Block
			GLuint m_uboIndex;   //Ubo index
			GLint  m_uboSize;    //Size of buffer
			GLuint m_uboHandler;  //Ubo handler
			GLuint m_num_uniforms;

			const GLchar* m_names[max_uniforms];
			GLuint m_indices[max_uniforms];
			GLint m_offsets[max_uniforms];

			void clear();
		};

	}
}

#endif
