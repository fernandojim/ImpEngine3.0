/*********************************************
  File:       CGameEngine.h
  Purpose:    Class for the Game Engine
  class name: CGameEngine derived from CWOpenGL
  --------------------------------------------
  @Author: Yo
  @Version: 0
 *********************************************/

#ifndef _CGAMEENGINE_H_
#define _CGAMEENGINE_H_

#include <vector>
#include <memory>

#ifdef _OPENGL44_
 #include "gl_core_4_4.h"
#else
 #include "gl_core_3_3.h"
#endif

#include "CTimer.h"
#include "VideoManager.h"
#include "CSystem.h"

namespace Engine
{
	namespace Managers
	{
		class GameManager
		{
		public:
			GameManager();
			~GameManager();

			void startup();
			void release();

			LRESULT Run();

			void addSystem(const std::shared_ptr<Engine::System::CSystem>& system);
			std::shared_ptr<Engine::System::CSystem> getSystem(Engine::System::SystemType systp);

			//Real time methods
			std::string getsFPS();
			void getElapsedSeconds();
			double getDeltaTime();

			void exitGame();

			static GameManager& getGameManagerInstance();

		private:
			//Timer for real time gamming
			std::unique_ptr<Engine::CTimer> m_gameTimer;

			//Systems list as rendering, updating
			std::vector<std::shared_ptr<Engine::System::CSystem>> m_Systems;

			//Game realtime
			double dt;

			bool m_exitgame;
		};
	}
}

#endif 
