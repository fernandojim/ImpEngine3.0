/*******************************************
  File:       CTimer.h
  Purpose:    Class for a Windows timer
  class name: CTimer
  ------------------------------------------
  @Author: Yo
  @Version: 0
 *******************************************/

#ifndef __CTIMER_H__
#define __CTIMER_H__

#include <string>
#include <SDL.h>

namespace Engine
{
	class CTimer
	{
	public:
		//Initializes variables
		CTimer();

		//The various clock actions
		void start();
		void stop();
		void pause();
		void unpause();

		//Gets ticks (millisecs) since start
		Uint32 getTicks();

		std::string sgetFPS();

		//Get elapsed seconds
		double getElapsedSeconds();

		//Checks the status of the timer
		bool isStarted();
		bool isPaused();
	
	private:
		//The clock time when the timer started
		Uint32 m_iStartTicks;

		//The ticks stored when the timer was paused
		Uint32 m_iPausedTicks;
	
		//Frames per second
		int m_iFps;
	
		//The timer status
		bool m_bPaused;
		bool m_bStarted;
	};
}

#endif // __CTIMER_H___
