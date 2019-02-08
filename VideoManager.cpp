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
	  m_bdebug(false),
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

		//Turno on debugging
		if (m_bdebug)
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

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

			// Debugging
			aux = pfile->getAttributeValue("DEBUG");
			if (aux == "N")
			{
				m_bdebug = false;
				//m_debugLevel = NONE;
			}
			else if (aux == "Y")
			{
				m_bdebug = true;
				//m_debugLevel = LOW;
			}
			/*else if (aux == "M")
			{
				m_bdebug = true;
				m_debugLevel = MEDIUM;
			}
			else if (aux == "H")
			{
				m_bdebug = true;
				m_debugLevel = HIGH;
			}*/
			else
			{
				Message::MessageBOX("VideoManager", "Invalid 'DEBUG' value");

				return false;
			}
		}
	}

	return true;
}

void VideoManager::InitScene()
{
	glViewport(0, 0, m_width, m_height);

	// Enable the debug callback
	if (m_bdebug)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(openglCallbackFunction, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true);
	}

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

//Debugging callback
void APIENTRY VideoManager::openglCallbackFunction(
			  GLenum source,
			  GLenum type,
			  GLuint id,
			  GLenum severity,
			  GLsizei length,
			  const GLchar* message,
			  const void* userParam
			)
{
	std::string cad;

	cad = cad + "\n---------------------------------------------------------------------------------------------------";
	//Creates the message

	//severity
	cad = cad + "\nSeverity: ";
	switch (severity)
	{
		case GL_DEBUG_SEVERITY_HIGH:
			cad = cad + "High severity";
		break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			cad = cad + "Medium severity";
		break;
		case GL_DEBUG_SEVERITY_LOW:
			cad = cad + "Low severity";
		break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			cad = cad + "Informative";
		break;
		default:
			cad = cad + "Undefined";
		break;
	}

	//source
	cad = cad + "\nSource is: ";
	switch (source)
	{
		case GL_DEBUG_SOURCE_API:
			cad = cad + "OpenGL API calls";
		break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			cad = cad + "Windows API calls";
		break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			cad = cad + "Shader compiler";
		break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:
			cad = cad + "Third party";
		break;
		case GL_DEBUG_SOURCE_APPLICATION:
			cad = cad + "User";
		break;
		case GL_DEBUG_SOURCE_OTHER:
			cad = cad + "Other";
		break;
		default:
			cad = cad + "No source";
		break;
	}

	//type
	cad = cad + "\nType is: ";
	switch (type)
	{
		case GL_DEBUG_TYPE_ERROR:
			cad = cad + "From the API";
		break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			cad = cad + "Deprecated behaviour";
		break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			cad = cad + "Undefined behaviour";
		break;
		case GL_DEBUG_TYPE_PORTABILITY:
			cad = cad + "Not portable";
		break;
		case GL_DEBUG_TYPE_PERFORMANCE:
			cad = cad + "Performance issue";
		break;
		case GL_DEBUG_TYPE_MARKER:
			cad = cad + "Command stream annotation";
		break;
		case GL_DEBUG_TYPE_PUSH_GROUP:
			cad = cad + "Push group";
		break;
		case GL_DEBUG_TYPE_POP_GROUP:
			cad = cad + "Pop group";
		break;
		case GL_DEBUG_TYPE_OTHER:
			cad = cad + "Other";
		break;
		default:
			cad = cad + "No type";
		break;
	}

	std::cout << cad;
	std::cout << "\n OpenGL debug message: " << message;
	std::cout.flush();
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
