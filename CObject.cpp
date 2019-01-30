/*
 * CObject.cpp
 *
 *  Created on: 14 de nov. de 2017
 *      Author: fjimartinez
 */

#include "CObject.h"

using namespace Engine::Base;

std::uint8_t CObject::m_id = 0;

//Copy constructor
/*CObject::CObject(const CObject& object)
{
	this->m_name = object.m_name;
	this->m_id++;
}*/

//Copy assignation constructor
/*CObject& CObject::operator=(const CObject &cp)
{
	this->m_name = cp.m_name;
	this->m_id++;

	return *this;
}*/

CObject::CObject() : m_name("")
{
	m_id++;
}

CObject::CObject(const std::string& name) : m_name(name)
{
	m_id++;
}

//get/set name property
std::string CObject::getName()
{
	return m_name;
}

void CObject::setName(const std::string& name)
{
	m_name = name;
}

//get/set id Property
unsigned int CObject::getId()
{
	return m_id;
}

CObject::~CObject() {}
