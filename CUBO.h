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
			* Bind the uniform to the Uniform Buffer Object
			********************************************************************/
			void uniformBlockIndex(GLuint shader_id, const std::string& uniform);

		   /********************************************************************
		    * Add a name to the uniform block
			********************************************************************/
			void addUniformsToUbo(GLuint shader_id, GLuint nnames, const GLchar** uniform);

		   /********************************************************************
			* Create and initialize the Uniform Buffer Object
			********************************************************************/
			void createUniformBufferObject(std::size_t size, GLuint usage);

		   /********************************************************************
			* Update the data to the Buffer
			********************************************************************/
			void update(std::size_t size, const void* data);
			void update2(GLintptr offset, GLsizeiptr size, const GLvoid* data);

		   /********************************************************************
			* Get the Uniform Buffer Object handler
			********************************************************************/
			GLuint getUBOHandler();

		private:
			static const GLuint max_uniforms = 10;

			GLvoid *m_buffer;    //Buffer where is alloc the Uniform Block
			GLuint m_uboIndex;   //Ubo index
			GLint  m_uboSize;    //Size of buffer
			GLuint m_uboHandle;  //Ubo handler
			GLuint m_num_uniforms;

			const GLchar* m_names[max_uniforms];
			GLuint m_indexes[max_uniforms];
			GLint m_sizes[max_uniforms];
			GLint m_offsets[max_uniforms];
			GLint m_types[max_uniforms];

			void clear();
		};

	}
}

#endif
