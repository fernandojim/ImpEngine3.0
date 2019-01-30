// ImpEngine3.0.cpp : Defines the entry point for the console application.
//

//Managers
#include "VideoManager.h"
#include "InputManager.h"
#include "GameManager.h"
#include "GameObjectManager.h"
#include "ComponentManager.h"

//Systems
#include "CRenderingSystem.h"

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//Starts the managers
	Engine::Managers::VideoManager::getVideoManagerInstance().startup();
	Engine::Managers::InputManager::getInputManagerInstance().startup();
	Engine::Managers::GameManager::getGameManagerInstance().startup();
	Engine::System::CRenderingSystem::getRenderingSystem().startup();
	Engine::Managers::GameObjectManager::getGameObjectManagerInstance().startup();
	Engine::Managers::ComponentManager::getComponentManagerInstance().startup();

	//Game main loop
	Engine::Managers::GameManager::getGameManagerInstance().Run();

	//Release the systems
	Engine::System::CRenderingSystem::getRenderingSystem().release();

	//Release the Game managers and exit
	Engine::Managers::ComponentManager::getComponentManagerInstance().release();
	Engine::Managers::VideoManager::getVideoManagerInstance().release();
	Engine::Managers::InputManager::getInputManagerInstance().release();
	Engine::Managers::GameManager::getGameManagerInstance().release();
	Engine::Managers::GameObjectManager::getGameObjectManagerInstance().release();

	return 0;
}
