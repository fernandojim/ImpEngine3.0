/*
 * MaterialManager.cpp
 *
 *  Created on: 21 de nov. de 2017
 *      Author: fjimartinez
 */

#include "MaterialManager.h"
#include "TextureManager.h"

using namespace Engine::Managers;

/*
    ----------------------------
	Methods for CMaterialManager
	----------------------------
*/

MaterialManager& MaterialManager::getSingleton()
{
	static MaterialManager mm;

	return mm;
}

MaterialManager::MaterialManager()
{
	m_uiMaterials = 0;

	m_Materials.clear();
}

MaterialManager::~MaterialManager()
{
	std::map < std::string, std::shared_ptr<Engine::Graphics::CMaterial> >::iterator iter = m_Materials.begin();

	for (; iter != m_Materials.end(); ++iter)
	{
		//Gets the object in the i-position
		std::shared_ptr<Engine::Graphics::CMaterial> pMaterial = iter->second;

		assert(pMaterial);

		//Free our resources
		if (pMaterial)
		{
			//delete pMaterial;
			pMaterial = NULL;
		}
	}

	m_Materials.clear();

	m_uiMaterials = 0;
}

void MaterialManager::loadMaterialsFromMTL(const std::string& sMTLName)
{
	std::shared_ptr<Engine::Graphics::CMaterial> pMaterial;
	std::string sid;
	std::string mat_name;
	std::string aux[4];
	std::string line;
	bool found = false;

	//Load Materials from file mtl
	std::unique_ptr<CFileDef> filedef = std::unique_ptr<CFileDef>(new CFileDef(sMTLName));

	//Test if this material exists in the library of materials
	//pMaterial = getMaterialByName(sMTLName);

	//Load material
	if (filedef->getIsOpen() && filedef->m_Filedef.good())
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
					std::stringstream(line) >> aux[0] >> aux[1];
					std::stringstream(aux[1]) >> mat_name;

					//Test if the material already exist
					for (auto itt : m_Materials)
					{
						std::string name = itt.first;

						if (!name.compare(mat_name))
						{
							found = true;
							break;
						}
					}

					//The material does not exists. Then, it is created and stored
					if ( !found )
					{
						pMaterial = std::make_shared<Engine::Graphics::CMaterial>(mat_name);
						pMaterial->setName(mat_name);

						while (!filedef->m_Filedef.eof())
						{
							/* get line from file and stores into buffer */
							std::getline(filedef->m_Filedef, line);

							//Test if it is the end of a material
							if (line.size() == 0 || line.at(0) == '#' || line.at(0) == '\n' || line.at(0) == '\0' || line.at(0) == ' ')
								break;

							if (!line.compare(0, SZ_NS, "Ns"))
							{
								std::stringstream(line) >> aux[0] >> aux[1];
								pMaterial->m_fNs = std::stof(aux[1]);
							}
							else if (!line.compare(0, SZ_KA, "Ka"))
							{
								std::stringstream(line) >> aux[0] >> aux[1] >> aux[2] >> aux[3];
								pMaterial->m_vKa.x = std::stof(aux[1]);
								pMaterial->m_vKa.y = std::stof(aux[2]);
								pMaterial->m_vKa.z = std::stof(aux[3]);
							}
							else if (!line.compare(0, SZ_KD, "Kd"))
							{
								std::stringstream(line) >> aux[0] >> aux[1] >> aux[2] >> aux[3];
								pMaterial->m_vKd.x = std::stof(aux[1]);
								pMaterial->m_vKd.y = std::stof(aux[2]);
								pMaterial->m_vKd.z = std::stof(aux[3]);
							}
							else if (!line.compare(0, SZ_KS, "Ks"))
							{
								std::stringstream(line) >> aux[0] >> aux[1] >> aux[2] >> aux[3];
								pMaterial->m_vKs.x = std::stof(aux[1]);
								pMaterial->m_vKs.y = std::stof(aux[2]);
								pMaterial->m_vKs.z = std::stof(aux[3]);
							}
							else if (!line.compare(0, SZ_NI, "Ni"))
							{
								std::stringstream(line) >> aux[0] >> aux[1];
								pMaterial->m_fNi = std::stof(aux[1]);
							}
							else if (!line.compare(0, SZ_D, "d"))
							{
								std::stringstream(line) >> aux[0] >> aux[1];
								pMaterial->m_fd = std::stof(aux[1]);
							}
							else if (!line.compare(0, SZ_ILLUM, "illum"))
							{
								std::stringstream(line) >> aux[0] >> aux[1];
								pMaterial->m_iIllum = std::stoi(aux[1]);
							}
							else if (!line.compare(0, SZ_MAPBUMP, "map_Bump"))
							{
								pMaterial->m_sMap_Bump = line.substr(9, line.length());
								pMaterial->m_iMap_Bump_texture = Engine::Managers::TextureManager::getSingleton().loadTexture(pMaterial->m_sMap_Bump);
							}
							else if (!line.compare(0, SZ_MAPKA, "map_Ka"))
							{
								pMaterial->m_sMap_ka = line.substr(7, line.length());
								pMaterial->m_iMap_ka_texture = Engine::Managers::TextureManager::getSingleton().loadTexture(pMaterial->m_sMap_ka);
							}
							else if (!line.compare(0, SZ_MAPKD, "map_Kd"))
							{
								pMaterial->m_sMap_kd = line.substr(7, line.length());
								pMaterial->m_iMap_kd_texture = Engine::Managers::TextureManager::getSingleton().loadTexture(pMaterial->m_sMap_kd);
							}
							else if (!line.compare(0, SZ_MAPKS, "map_Ks"))
							{
								pMaterial->m_sMap_ks = line.substr(7, line.length());
								pMaterial->m_iMap_ks_texture = Engine::Managers::TextureManager::getSingleton().loadTexture(pMaterial->m_sMap_ks);
							}
							//... Other values
						}

						//Add the new material
						m_Materials.insert(std::pair < std::string, std::shared_ptr<Engine::Graphics::CMaterial> >(mat_name, pMaterial));
					}
				}
			}
		}
	}
}

std::shared_ptr<Engine::Graphics::CMaterial> MaterialManager::getMaterialByName(const std::string& sName)
{
	std::map < std::string, std::shared_ptr<Engine::Graphics::CMaterial> >::iterator iter;

	//Gets the map of id
	iter = m_Materials.find(sName);

	if (iter != m_Materials.end())
		return iter->second;
	else
		return NULL;
}

GLuint MaterialManager::getTextureId(TEXTURE_MAP_TYPE texType, const std::string& sName)
{
	std::map < std::string, std::shared_ptr<Engine::Graphics::CMaterial> >::iterator iter;

	//Gets the map of id
	iter = m_Materials.find(sName);

	if (iter != m_Materials.end())
	{
		//Return the material type requested
		switch (texType)
		{
			case MAP_KA:
				return iter->second->m_iMap_ka_texture;
			break;
			case MAP_KD:
				return iter->second->m_iMap_kd_texture;
			break;
			case MAP_KS:
				return iter->second->m_iMap_ks_texture;
			break;
			default:
				return 0;
			break;
		}
	}

	return 0;
}

