/*
 * CSystem.cpp
 *
 *  Created on: 12 de ene. de 2018
 *      Author: fjimartinez
 */

#include "CSystem.h"

using namespace Engine::System;

CSystem::CSystem() : m_systemType(Engine::System::SystemType::NO_TYPE)
{

}

CSystem::CSystem(const SystemType& tp) : m_systemType(tp)
{

}

SystemType Engine::System::systemType(unsigned int typ)
{
	switch (typ)
	{
		case 1:
			return Engine::System::SystemType::RENDERER;
		break;
		case 2:
			return Engine::System::SystemType::TRANSFORM;
		break;
		default:
			return Engine::System::SystemType::NO_TYPE;
		break;
	}
}

CSystem::~CSystem() {}

