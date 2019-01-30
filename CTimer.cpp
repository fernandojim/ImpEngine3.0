/////////////////////////
//
//  --- ctimer.cpp ---
//
/////////////////////////

#include "CTimer.h"
#include <iostream>
#include <sstream>

using namespace Engine;

CTimer::CTimer() 
{ 
	//Initialize the variables 
	m_iStartTicks = 0; 
	m_iPausedTicks = 0; 
	m_bPaused = false; 
	m_bStarted = false;
	m_iFps = 0;
}

void CTimer::start() 
{ 
	//Start the timer 
	m_bStarted = true; 
	
	//Unpause the timer 
	m_bPaused = false; 
	
	//Get the current clock time 
	m_iStartTicks = SDL_GetTicks(); 
	m_iPausedTicks = 0; 
}

void CTimer::stop() 
{ 
	//Stop the timer 
	m_bStarted = false; 
	
	//Unpause the timer 
	m_bPaused = false; 
	
	//Clear tick variables 
	m_iStartTicks = 0; 
	m_iPausedTicks = 0; 
}

void CTimer::pause() 
{ 
	//If the timer is running and isn't already paused 
	if( m_bStarted && !m_bPaused ) 
	{ 
		//Pause the timer 
		m_bPaused = true; 
		
		//Calculate the paused ticks 
		m_iPausedTicks = SDL_GetTicks() - m_iStartTicks; 
		m_iStartTicks = 0; 
	} 
}

void CTimer::unpause() 
{ 
	//If the timer is running and paused 
	if( m_bStarted && m_bPaused ) 
	{ 
		//Unpause the timer 
		m_bPaused = false; 
		
		//Reset the starting ticks 
		m_iStartTicks = SDL_GetTicks() - m_iPausedTicks; 
		
		//Reset the paused ticks 
		m_iPausedTicks = 0; 
	} 
}

Uint32 CTimer::getTicks() 
{ 
	//The actual timer time 
	Uint32 time = 0; 
	
	//If the timer is running 
	if( m_bStarted ) 
	{ 
		//If the timer is paused 
		if( m_bPaused ) 
		{ 
			//Return the number of ticks when the timer was paused 
			time = m_iPausedTicks; 
		} 
		else 
		{ 
			//Return the current time minus the start time 
			time = SDL_GetTicks() - m_iStartTicks; 
		} 
	} 
	
	return time; 
}

std::string CTimer::sgetFPS()
{
	std::stringstream ss;

	ss << m_iFps;

	return ss.str();
}

double CTimer::getElapsedSeconds()
{
	static Uint32 starttime = 0;
	static Uint32 starttimeFPS = 0;
	Uint32 currenttime;
	Uint32 currenttimeFPS;

	// Get the current ticks
	currenttime = currenttimeFPS = SDL_GetTicks();

	// Get the seconds pass between calls to getElapsedSeconds
	double secs = ((double)currenttime - (double)starttime) / 1000.0;

	if (currenttimeFPS - starttimeFPS > 1000.0)
	{
		m_iFps = (int)(1.0 / secs);
		starttimeFPS = currenttimeFPS;
	}

	// Reset the timer
	starttime = currenttime;

	// Get Frames Per Second

	return secs;
}

bool CTimer::isStarted() 
{ 
	//Timer is running and paused or unpaused 
	return m_bStarted; 
} 

bool CTimer::isPaused() 
{ 
	//Timer is running and paused 
	return m_bPaused && m_bStarted; 
}
