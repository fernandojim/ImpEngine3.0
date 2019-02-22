////////////////////////////////////////////////////////////////////////////////
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

#ifndef _CMESH_H_
#define _CMESH_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <memory>

#define GLM_FORCE_RADIANS

#include "glm\vec3.hpp"
#include "glm\vec2.hpp"
#include "glm\mat4x4.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_transform.hpp" //Header for glm::perspective, etc...

#include "CFiledef.h"
#include "CShader.h"
#include "CMaterial.h"
#include "CRenderingComponent.h"

namespace Engine
{
	namespace Graphics
	{
		class CMesh : public Engine::Component::CRenderingComponent
		{
		public:
			std::shared_ptr<Engine::Graphics::CMaterial> m_Material;

		public:
			//Constructor with object and shader files
			CMesh(const std::string& name, GLuint shaderid, const std::string& file);

			 void ReceiveEvent(void *buff);

			//Destructor
			virtual ~CMesh();

		private:
			//Methods for load info from object file
			glm::vec3 getVec3Info(const std::string& cad);
			glm::vec2 getVec2Info(const std::string& cad);
			Engine::Graphics::face getFaceInfo(const std::string& cad);
			void loadMesh();

			//Mesh file (.obj from blender)
			std::shared_ptr<CFileDef> m_File;

			//Load submesh from file
			bool load(const std::string& file);
		};
	}
}

#endif
