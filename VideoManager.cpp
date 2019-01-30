/*******************************************
  File:       VideoManager.cpp
  Purpose:    Manager Class for a the video
  class name: VideoManager
  ------------------------------------------
  @Author: Yo
  @Version: 0
 *******************************************/

#include "VideoManager.h"
#include "utils.h"
#include "CXMLParser.h"

using namespace Engine::Managers;

//
// -- The Instance to VideoManager class
//
VideoManager& VideoManager::getVideoManagerInstance()
{
	static VideoManager vm;

	return vm;
}

VideoManager::VideoManager()
	: m_mainwindow(nullptr),
	  m_maincontext(nullptr),
	  m_isStarted(false),
	  m_width(0),
	  m_height(0),
	  m_bits(0),
	  m_aspect(0),
	  m_bfullscreen(false),
	  m_bvsync(false),
	  m_szName("")
{

}

void VideoManager::startup()
{
	m_aspect = 0.0;

	Uint32 flags = SDL_WINDOW_OPENGL;

	//Load the parameters
	if (!Load(engine_file))
	{
		SDL_Quit();
	}
	else
	{
		//Set full screen if fsscreen parameter is true
		if (m_bfullscreen)
			flags = flags | SDL_WINDOW_FULLSCREEN;

		//Initialize SDL's Video subsystem
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		{
			Message::MessageBOX("VideoManager", "Unable to initialize SDL");
			SDL_Quit();
		}

		//Request the OpenGL version
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OPENGL_VERSION_MAYOR);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OPENGL_VERSION_MINOR);

		//Turn on double buffering
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, m_bits);

		//Create our window centered at given position
		m_mainwindow = SDL_CreateWindow(m_szName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			m_width, m_height, flags);

		//Die if creation failed
		if (!m_mainwindow)
		{
			Message::MessageBOX("VideoManager", "Fail tor create the main window");
			SDL_Quit();
		}

		/*if (SDLNet_Init() < 0)
		{
			//std::cerr << "SDLNet_Init: " << SDLNet_GetError() << std::endl;
			std::cout << "\nNo se puede inicializar SDLNet" << std::endl;
		}*/

		/* Hidde cursor mouse and set it to window center */
		SDL_SetRelativeMouseMode(SDL_TRUE);
		SDL_WarpMouseInWindow(m_mainwindow, 320, 240);

		/* Create our opengl context and attach it to our window */
		m_maincontext = SDL_GL_CreateContext(m_mainwindow);

		/* Load OpenGL functions */
		if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
		{
			Message::MessageBOX("VideoManager", "Unable to load ogl functions");
			SDL_Quit();
		}

		//Initialize the scene
		InitScene();

		//ViewHardwareInfo();

		/* This makes our buffer swap syncronized with the monitor's vertical refresh. Parameter: */
		/* 0 -> without sync refresh */
		/* 1 -> Sync refresh */
		SDL_GL_SetSwapInterval((int)m_bvsync);

		m_isStarted = true;
	}
}

//
// -- Load the opengl window parameters
//
bool VideoManager::Load(const std::string& szFile)
{
	//Create the script file
	std::unique_ptr<CXMLParser> pfile(new CXMLParser);

	//Try to open the file and read its content
	if (!pfile->LoadXMLFile(szFile))
	{
		Message::MessageBOX("Engine file error", szFile);

		return false;
	}
	else
	{
		//Find the ENGINE node
		if (pfile->getNodes("ENGINE"))
		{
			// Gets the name ob object
			m_szName = pfile->getAttributeValue("name");

			std::string aux;

			// Gets the FULLSCREEN value
			aux = pfile->getAttributeValue("FULLSCREEN");
			if (aux == "N")
				m_bfullscreen = false;
			else if (aux == "Y")
				m_bfullscreen = true;
			else
			{
				Message::MessageBOX("VideoManager", "Invalid 'FULLSCREEN' value");

				return false;
			}

			// VERTICAL SYNC
			aux = pfile->getAttributeValue("VSYNC");
			if (aux == "N")
				m_bvsync = false;
			else if (aux == "Y")
				m_bvsync = true;
			else
			{
				Message::MessageBOX("VideoManager", "Invalid 'VSYNC' value");

				return false;
			}

			// WINDOW WIDTH
			m_width = atoi(pfile->getAttributeValue("WINDOW_HEIGHT").c_str());
			if (m_width < 320 || m_width > 1920)
			{
				Message::MessageBOX("VideoManager", "Invalid Window 'width' value");

				return false;
			}

			// WINDOW HEIGHT
			m_height = atoi(pfile->getAttributeValue("WINDOW_WIDTH").c_str());
			if (m_height < 200 || m_height > 1200)
			{
				Message::MessageBOX("VideoManager", "Invalid Window 'height' value");

				return false;
			}

			// WINDOW DEPTH BITS
			m_bits = atoi(pfile->getAttributeValue("WINDOW_COLOR_DEPTH").c_str());
			if (m_bits != 8 && m_bits != 16 && m_bits != 24 && m_bits != 32)
			{
				Message::MessageBOX("VideoManager", "Invalid Window bits depth color value");

				return false;
			}
		}
	}

	return true;
}

void VideoManager::InitScene()
{
	glViewport(0, 0, m_width, m_height);

	//Init OpenGL
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glClearDepth(GL_TRUE);
}

void VideoManager::Resize()
{
	if (m_width > m_height)
	{
		m_aspect = m_width;
	}
	else
	{
		m_aspect = m_height;
	}
	
	if (m_maincontext)
	{
		glViewport(0, 0, m_width, m_height);
	}
}

void VideoManager::ViewHardwareInfo()
{
	char cad[300];
	const GLubyte *vendor = glGetString(GL_VENDOR);
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *version = glGetString(GL_VERSION);
	const GLubyte *language = glGetString(GL_SHADING_LANGUAGE_VERSION);

	sprintf(cad, "%s\n%s\n%s\n%s", vendor, renderer, version, language);
	Message::MessageBOX("OpenGL info", cad);
}

bool VideoManager::isVideoManagerStarted()
{
	return m_isStarted;
}

SDL_Window* VideoManager::getMainWindowHandler()
{
	return m_mainwindow;
}

void VideoManager::release()
{
	/* Delete our opengl context, destroy our window, and shutdown SDL */
	SDL_GL_DeleteContext(m_maincontext);
	SDL_DestroyWindow(m_mainwindow);
	SDL_Quit();
}

VideoManager::~VideoManager()
{
	release();
}
