/*
 * InputManager.cpp
 *
 *  Created on: 24 de may. de 2018
 *      Author: fjimartinez
 */

#include "InputManager.h"
#include "VideoManager.h"
#include "GameManager.h"

using namespace Engine::Managers;

//
// -- The Instance to VideoManager class
//
InputManager& InputManager::getInputManagerInstance()
{
	static InputManager im;

	return im;
}

InputManager::InputManager() : m_yaw(0),
							   m_pitch(0),
							   m_mouseSensitivity(0.0)
{
}

void InputManager::startup()
{
	m_mouseSensitivity = 0.2f;

	//Test if the VideoManager is started
	if (Engine::Managers::VideoManager::getVideoManagerInstance().isVideoManagerStarted())
	{
		//Initialize the array of keys
		memset(&m_arrbKeys, 0, sizeof(bool) * SDL_NUM_SCANCODES);
	}
}

void InputManager::ManageInputEvents()
{
	SDL_Event event;

	//Get the event if exist
	if (SDL_PollEvent(&event))
	{
		//Manage the events
		switch (event.type)
		{
			case SDL_KEYDOWN:
				//Enables the scancode of key pressed
				m_arrbKeys[event.key.keysym.scancode] = true;
			break;

			case SDL_KEYUP:
				//Disables the scancode of key pressed
				m_arrbKeys[event.key.keysym.scancode] = false;
			break;

			case SDL_MOUSEMOTION:
				int x,y;
				SDL_GetRelativeMouseState(&x, &y);
				m_yaw += x * m_mouseSensitivity;
				m_pitch -= y * m_mouseSensitivity;
			break;
		}

		CheckInput();
	}
	else
	{
		Engine::Managers::GameManager::getGameManagerInstance().getElapsedSeconds();
	}
}

void InputManager::CheckInput(/*double deltaTime*/)
{
	//static double buttonDelta = 0.0;

	//buttonDelta -= deltaTime;

	//if (buttonDelta < 0.0)
	//	buttonDelta = 0.0;

	//Get the pressed keys
	if (m_arrbKeys[SDL_SCANCODE_W])
		OnKeyDown(SDLK_w);

	if (m_arrbKeys[SDL_SCANCODE_S])
		OnKeyDown(SDLK_s);

	if (m_arrbKeys[SDL_SCANCODE_A])
		OnKeyDown(SDLK_a);

	if (m_arrbKeys[SDL_SCANCODE_D])
		OnKeyDown(SDLK_d);

	if (m_arrbKeys[SDL_SCANCODE_T])
		OnKeyDown(SDLK_t);

	if (m_arrbKeys[SDL_SCANCODE_G])
		OnKeyDown(SDLK_g);

	if (m_arrbKeys[SDL_SCANCODE_F])
		OnKeyDown(SDLK_f);

	if (m_arrbKeys[SDL_SCANCODE_H])
		OnKeyDown(SDLK_h);

	//Exit
	if (m_arrbKeys[SDL_SCANCODE_ESCAPE])
		OnKeyDown(SDLK_ESCAPE);
}

void InputManager::OnKeyDown(SDL_Keycode key)
{
	// Adelante
	if (key == SDLK_w)
	{
		//if (glm::length(m_gameCamera->velocity) < 25.0)
		//	m_gameCamera->velocity += glm::vec3(0.0, 0.0, 2.0);
	}

	// Atrás
	if (key == SDLK_s)
	{
		//if (glm::length(m_gameCamera->velocity) < 25.0)
		//	m_gameCamera->velocity += glm::vec3(0.0, 0.0, -2.0);
	}

	// Izquierda
	if (key == SDLK_a)
	{
		//if (glm::length(m_gameCamera->velocity) < 25.0)
		//	m_gameCamera->velocity += glm::vec3(-2.0, 0.0, 0.0);
	}

	// Derecha
	if (key == SDLK_d)
	{
		//if (glm::length(m_gameCamera->velocity) < 25.0)
		//	m_gameCamera->velocity += glm::vec3(2.0, 0.0, 0.0);
	}

	if (key == SDLK_t)
	{
		//*::getGameObjectManager().m_GameObjectsMD2.at(0).m_speed = 16.0;
	}

	if (key == SDLK_g)
	{

	}

	if (key == SDLK_f)
	{
		//*::getGameObjectManager().m_GameObjectsMD2.at(0).m_angle += 0.02;
	}

	if (key == SDLK_h)
	{
		//*::getGameObjectManager().m_GameObjectsMD2.at(0).m_angle -= 0.02;
	}

	// Tecla ESC
	if (key == SDLK_ESCAPE)
	{
		Engine::Managers::GameManager::getGameManagerInstance().exitGame();
	}
}

float InputManager::getYaw()
{
	return m_yaw;
}

float InputManager::getPitch()
{
	return m_pitch;
}

void InputManager::release()
{

}

InputManager::~InputManager()
{
	release();
}
