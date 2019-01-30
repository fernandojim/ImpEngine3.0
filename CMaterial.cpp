////////////////////////////////////////////////////////////////////////////////
// cmaterial.h
// Author       : Fernando Jiménez Martínez
//
// Copyright    : (C) 2015 by Fernando Jiménez
// Email        : fernandojim@gmail.com
////////////////////////////////////////////////////////////////////////////////

/*********************************************************************************
*                                                                                *
*   This program is free software; you can redistribute it and/or modify         *
*   it under the terms of the GNU Lesser General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or            *
*   (at your option) any later version.                                          *
*                                                                                *
**********************************************************************************/

#include "CFiledef.h"
#include "CMaterial.h"
#include "TextureManager.h"

/*const int SZ_NEWMTL  = sizeof("newmtl")-1;
const int SZ_NS      = sizeof("Ns")-1;
const int SZ_KA      = sizeof("Ka")-1;
const int SZ_KD      = sizeof("Kd")-1;
const int SZ_KS      = sizeof("Ks")-1;
const int SZ_NI      = sizeof("Ni")-1;
const int SZ_D       = sizeof("d")-1;
const int SZ_ILLUM   = sizeof("illum")-1;
const int SZ_MAPKD   = sizeof("map_Kd")-1;
const int SZ_MAPKS   = sizeof("map_Ks")-1;
const int SZ_MAPKA   = sizeof("map_Ka")-1;
const int SZ_MAPBUMP = sizeof("map_Bump")-1;*/

using namespace Engine::Graphics;
using namespace Engine::Managers;

CMaterial::CMaterial(const std::string& file) : Engine::Base::CObject()
{
	std::string aux[4];
	std::string line;

	//Creates the file to read info
	std::unique_ptr<CFileDef> filedef = std::unique_ptr<CFileDef>(new CFileDef("objetos\\"+file));

	// if exist
	if (filedef->getIsOpen())
	{
		while (!filedef->m_Filedef.eof())
		{
			/* get line from file and stores into buffer */
			std::getline(filedef->m_Filedef, line);

			/* ignores '#' '\n' '{' or '\0' characters */
			if (line.size() != 0 && line.at(0) != '#' && line.at(0) != '\n' && line.at(0) != '\0')
			{
				//Get the field newmtl
				if (!line.compare(0, SZ_NEWMTL, "newmtl"))
				{
					std::string nm;
					std::stringstream(line) >> aux[0] >> aux[1];
					std::stringstream(aux[1]) >> nm;
					setName(nm);
				}
				else if (!line.compare(0, SZ_NS, "Ns"))
				{
					std::stringstream(line) >> aux[0] >> aux[1];
					m_fNs = std::stof(aux[1]);
				}
				else if (!line.compare(0, SZ_KA, "Ka"))
				{
					std::stringstream(line) >> aux[0] >> aux[1] >> aux[2] >> aux[3];
					m_vKa.x = std::stof(aux[1]);
					m_vKa.y = std::stof(aux[2]);
					m_vKa.z = std::stof(aux[3]);
				}
				else if (!line.compare(0, SZ_KD, "Kd"))
				{
					std::stringstream(line) >> aux[0] >> aux[1] >> aux[2] >> aux[3];
					m_vKd.x = std::stof(aux[1]);
					m_vKd.y = std::stof(aux[2]);
					m_vKd.z = std::stof(aux[3]);
				}
				else if (!line.compare(0, SZ_KS, "Ks"))
				{
					std::stringstream(line) >> aux[0] >> aux[1] >> aux[2] >> aux[3];
					m_vKs.x = std::stof(aux[1]);
					m_vKs.y = std::stof(aux[2]);
					m_vKs.z = std::stof(aux[3]);
				}
				else if (!line.compare(0, SZ_NI, "Ni"))
				{
					std::stringstream(line) >> aux[0] >> aux[1];
					m_fNi = std::stof(aux[1]);
				}
				else if (!line.compare(0, SZ_D, "d"))
				{
					std::stringstream(line) >> aux[0] >> aux[1];
					m_fd = std::stof(aux[1]);
				}
				else if (!line.compare(0, SZ_ILLUM, "illum"))
				{
					std::stringstream(line) >> aux[0] >> aux[1];
					m_iIllum = std::stoi(aux[1]);
				}
				else if (!line.compare(0, SZ_MAPBUMP, "map_Bump"))
				{
					m_sMap_Bump = line.substr(9, line.length());
					m_iMap_Bump_texture = Engine::Managers::TextureManager::getSingleton().loadTexture(m_sMap_Bump);
				}
				else if (!line.compare(0, SZ_MAPKA, "map_Ka"))
				{
					m_sMap_ka = line.substr(7, line.length());
					m_iMap_ka_texture = Engine::Managers::TextureManager::getSingleton().loadTexture(m_sMap_ka);
				}
				else if (!line.compare(0, SZ_MAPKD, "map_Kd"))
				{
					m_sMap_kd = line.substr(7, line.length());
					m_iMap_kd_texture = Engine::Managers::TextureManager::getSingleton().loadTexture(m_sMap_kd);
				}
				else if (!line.compare(0, SZ_MAPKS, "map_Ks"))
				{
					m_sMap_ks = line.substr(7, line.length());
					m_iMap_ks_texture = Engine::Managers::TextureManager::getSingleton().loadTexture(m_sMap_ks);
				}
			}
		}
	}
}

void CMaterial::clear()
{
	m_fNs = 0.0;
	m_vKa.x = m_vKa.y = m_vKa.z = 0.0;
	m_vKd.x = m_vKd.y = m_vKd.z = 0.0;
	m_vKs.x = m_vKs.y = m_vKs.z = 0.0;
	m_fNi = 0.0;
	m_fd = 0.0;
	m_iIllum = 0.0;
	m_iMap_ka_texture = 0;
	m_iMap_ks_texture = 0;
	m_iMap_kd_texture = 0;
	m_iMap_Bump_texture = 0;
}


CMaterial::~CMaterial()
{

}
