
/******************************************************
 * MANAGERS
 ******************************************************/
#include "GameManager.h"
#include "VideoManager.h"
#include "InputManager.h"

/******************************************************
 * SYSTEMS
 ******************************************************/
#include "CRenderingSystem.h"

using Engine::Managers::GameManager;

//
// -- The Instance to GameManager class
//
GameManager& GameManager::getGameManagerInstance()
{
	static GameManager ge;

	return ge;
}

GameManager::GameManager() : m_gameTimer(nullptr),
		 	 	 	 	 	 dt(0),
		                     m_exitgame(false)

{
}

void GameManager::startup()
{
	//Create the timer
	m_gameTimer = std::unique_ptr<CTimer>(new CTimer);

	//Start it
	m_gameTimer->start();
}

LRESULT GameManager::Run()
{
	while (!m_exitgame)
	{
		//Get events from the event queue and generates the appropiate messages to entities
		Engine::Managers::InputManager::getInputManagerInstance().ManageInputEvents();

		//update

		//Animate(dt)

		//draw
		Engine::System::CRenderingSystem::getRenderingSystem().Render();

		//swap buffers (SDL2)
		SDL_GL_SwapWindow(Engine::Managers::VideoManager::getVideoManagerInstance().getMainWindowHandler());
	}

	return -1;
}

void GameManager::addSystem(const std::shared_ptr<Engine::System::CSystem>& system)
{
	m_Systems.push_back(system);
}

std::shared_ptr<Engine::System::CSystem> GameManager::getSystem(Engine::System::SystemType systp)
{
	//Provisional. Solo devuelve el primero.
	return m_Systems.at(0);
}

std::string GameManager::getsFPS()
{
	return m_gameTimer.get()->sgetFPS();
}

void GameManager::getElapsedSeconds()
{
	dt = m_gameTimer.get()->getElapsedSeconds();
}

double GameManager::getDeltaTime()
{
	return dt;
}

void GameManager::exitGame()
{
	m_exitgame = true;
}

void GameManager::release()
{
	m_Systems.clear();
}

GameManager::~GameManager()
{
	release();
}
