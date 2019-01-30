/*********************************************
  File:       CFbo.h
  Purpose:    Class for game Frame Buffer Objects
  class name: CFbo
  --------------------------------------------
  @Author: Yo
  @Version: 0
 *********************************************/

#ifndef _CFBO_H_
#define _CFBO_H_

#ifdef _OPENGL44_
 #include "gl_core_4_4.h"
#else
 #include "gl_core_3_3.h"
#endif

namespace Engine
{
	namespace Graphics
	{
		enum FrameBufferType {FRAMEBUFFER = 0, SHADOWMAP};

		class CFbo
		{
		public:
			CFbo(FrameBufferType m_type);
			CFbo(FrameBufferType m_type, GLuint w, GLuint h);
			virtual ~CFbo();

			//Frame buffer object
			void CreateFBO();

			//Shadow map buffer object
			void CreateSBO();

			void Bind();
			void Unbind();
			void clean();

		public:
			/* Handler to Frame Bufffer */
			GLuint m_uiFboHandler;

			/* Handler to buffers */
			GLuint m_uiColorHandler;
			GLuint m_uiDepthHandler;

			/* Handler to Texture */
			GLuint m_uiTextureHandler;

			/* Size */
			GLuint m_width;
			GLuint m_height;

			GLuint m_buffererror;

			FrameBufferType m_type;
		};
	}
}

#endif /* CFBO_H_ */
