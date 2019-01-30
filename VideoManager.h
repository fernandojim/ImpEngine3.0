/*******************************************
  File:       VideoManager.h
  Purpose:    Manager Class for a the video
  class name: VideoManager
  ------------------------------------------
  @Author: Yo
  @Version: 0
 *******************************************/

#ifndef _VIDEOMANAGER_H_
#define _VIDEOMANAGER_H_

#include <iostream>
#include <string>
#include <memory>
#include <SDL.h>
//#include <SDL_net.h>

#include "CXMLParser.h"

#ifdef _OPENGL44_
 #include "gl_core_4_4.h"
 #define OPENGL_VERSION_MAYOR 4
 #define OPENGL_VERSION_MINOR 4
#else
 #include "gl_core_3_3.h"
 #define OPENGL_VERSION_MAYOR 3
 #define OPENGL_VERSION_MINOR 3
#endif

const std::string engine_file("objetos\\engine.xml");

namespace Engine
{
	namespace Managers
	{
		class VideoManager
		{
		public:
			VideoManager();
			~VideoManager();

			void startup();
			void release();

			SDL_Window* getMainWindowHandler();
			void ViewHardwareInfo();

			bool isVideoManagerStarted();

			//the VideoManager instance
			static VideoManager& getVideoManagerInstance();

		private:
			//Load the window info
			bool Load(const std::string& szFile);

			//Inits OpenGL background
			void InitScene();

			//Operations to resize the OpenGL window
			void Resize();

			//Our window handle
			SDL_Window   *m_mainwindow;

			//Our opengl context handle
			SDL_GLContext m_maincontext;

			//If the Manager is started OK
			bool m_isStarted;

			//Window measures/attribs
			int m_width;
			int m_height;
			int m_bits;
			int m_aspect;
			bool m_bfullscreen;
			bool m_bvsync;
			std::string m_szName;
		};
	}
}

#endif
