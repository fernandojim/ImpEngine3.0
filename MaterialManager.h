/*********************************************
  File:       MaterialManager.h
  Purpose:    Class for MaterialManager
  class name: MaterialManager
  --------------------------------------------
  @Author: Yo
  @Version: 0
 *********************************************/

#ifndef MATERIALMANAGER_H_
#define MATERIALMANAGER_H_

#include <iostream>
#include <map>
#include <memory>

#include "CFiledef.h"
#include "CMaterial.h"

namespace Engine
{
	namespace Managers
	{
		enum TEXTURE_MAP_TYPE {MAP_KA = 0, MAP_KD, MAP_KS};

		class MaterialManager
		{
		public:
			MaterialManager();
			~MaterialManager();

			void loadMaterialsFromMTL(const std::string& sMaterialPath);

			std::shared_ptr<Engine::Graphics::CMaterial> getMaterialByName(const std::string& sName);
			GLuint getTextureId(TEXTURE_MAP_TYPE texType, const std::string& sName);

			bool deleteMaterial(const std::string& sName);

			static MaterialManager& getSingleton();
		private:
			//Material mapping by name
			std::map < std::string, std::shared_ptr<Engine::Graphics::CMaterial> > m_Materials;

			//Total num of materials
			GLuint m_uiMaterials;
		};
	}
}

#endif /* MATERIALMANAGER_H_ */
