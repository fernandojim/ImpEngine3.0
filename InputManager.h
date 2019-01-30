/*******************************************
  File:       InputManager.h
  Purpose:    Manager Class for a the Input
  class name: VideoManager
  ------------------------------------------
  @Author: Yo
  @Version: 0
 *******************************************/

#ifndef INPUTMANAGER_H_
#define INPUTMANAGER_H_

#include <SDL.h>

namespace Engine
{
	namespace Managers
	{
		class InputManager
		{
		public:
			InputManager();
			~InputManager();

			void startup();
			void release();

			void ManageInputEvents();
			void OnKeyDown(SDL_Keycode key);

			//the VideoManager instance
			static InputManager& getInputManagerInstance();

		private:
			//Mouse position
			float m_yaw;
			float m_pitch;
			float m_mouseSensitivity;

			//Array with the keys scancodes
			bool m_arrbKeys[SDL_NUM_SCANCODES];

			//Values calculated from mouse events
			float getYaw();
			float getPitch();

			void CheckInput(/*double deltaTime*/);

			//Methods for events
			/*virtual bool OnCreate() { return true; }
			virtual bool OnClose() { return true; }
			virtual void OnSize() { }
			virtual void OnMouseDownL(float x, float y) { }
			virtual void OnMouseDownR(float x, float y) { }
			virtual void OnMouseUpL() { }
			virtual void OnMouseUpR() { }
			virtual void OnMouseMove(int x, int y, int centerX, int centerY) {}
			virtual void OnMouseMove(int deltaX, int deltaY) {}
			virtual void OnMouseDragL(int x, int y, int dx, int dy) { }
			virtual void OnMouseDragR(int x, int y, int dx, int dy) { }
			virtual void OnKeyUp(SDL_Keycode key) {}
			virtual void OnKeyDown(SDL_Keycode key) {}
			virtual void OnChar(char c) { }*/

		};
	}
}

#endif /* INPUTMANAGER_H_ */
