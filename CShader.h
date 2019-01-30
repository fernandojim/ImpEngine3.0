/*******************************************
  File:       CShader.h
  Purpose:    Class for create shaders
  class name: CShader
  ------------------------------------------
  @Author: Yo
  @Version: 0
 *******************************************/

#ifndef _CSHADER_H_
#define _CSHADER_H_

#include <iostream>
#include <fstream>
#include <string>
#ifdef _OPENGL44_
 #include "gl_core_4_4.h"
#else
 #include "gl_core_3_3.h"
#endif
#include "utils.h"

#include "CObject.h"

#define MAX_INFO_LOG 255
#define MAX_UNIFORMS 10

#define BUFFER_OFFSET(i) ((unsigned int*) NULL + (i))

namespace Engine
{
	namespace Graphics
	{
		enum ERROR_TYPE { NO_ERR = 0, FILE_ERROR, VS_COMPILE_ERROR, FS_COMPILE_ERROR, PROGRAM_LINK_ERROR};

		class CShader : public Engine::Base::CObject
		{
		public:
			CShader(const std::string& name, const std::string& vs, const std::string& fs);
			CShader(const std::string& name);
			~CShader() {}

			void CreaShader(const std::string& vs, const std::string& fs);

			//Get error
			ERROR_TYPE getError();

			//Program identifier -for use in drawing meshes-
			GLuint getProgramId();

			/*****************************/
			/* Shaders public attributes */
			/*****************************/
			// Uniforms location array
			//GLint   m_iUniformLocation[MAX_UNIFORMS];

			// Uniforms variables
			void    setUniformShader1f(const std::string& variable, GLfloat v1);
			void    setUniformShader1i(const std::string& variable, GLint v1);
			void    setUniformShader2fv(const std::string& variable, GLsizei count, const GLfloat *value);
			void    setUniformShader3fv(const std::string& variable, GLsizei count, const GLfloat *value);
			void    setUniformShaderMatrix3fv(const std::string& variable, GLsizei count, GLboolean traspose, const GLfloat *value);
			void    setUniformShaderMatrix4fv(const std::string& variable, GLsizei count, GLboolean traspose, const GLfloat *value);
			void 	setUniformShaderTexture2D(const std::string& variable, GLuint textureid);
			void 	setUniformShaderTextureCubeMap(const std::string& variable, GLuint textureid);

		private:
			/***********************************/
			/* To compile and link the shaders */
			/***********************************/
			ERROR_TYPE m_err;

			GLuint	m_uiProgram;

			const char *m_sVertSource;
			const char *m_sFragSource;

			GLuint m_uiVertexShader;
			GLuint m_uiFragmentShader;

			GLint  m_iCompileStatusVertexShader;
			GLint  m_iCompileStatusFragmentShader;
			GLint  m_iLinkStatusShaders;

			static const int maxInfoLog = 255;
			GLchar m_sLogCompileStatusVertexShader[maxInfoLog];
			GLchar m_sLogCompileStatusFragmentShader[maxInfoLog];
			GLchar m_sLogLinkStatusShaders[maxInfoLog];

			void clean();

			ERROR_TYPE CompilaShaders();
			ERROR_TYPE LinkaShaders();

			GLchar* GetBufferFromFile(const std::string& in_szFilename);
		};
	}
}

#endif
