/*******************************************
  File:       CMaterial.h
  Purpose:    Class for Materials
  class name: CMaterial inherits from CObject
  ------------------------------------------
  @Author: Yo
  @Version: 0
 *******************************************/

#ifndef _CMATERIAL_H_
#define _CMATERIAL_H_

#include <vector>
#include <string>
#include <memory>

#ifdef _OPENGL44_
 #include "gl_core_4_4.h"
#else
 #include "gl_core_3_3.h"
#endif

#include <ImpEngine3.0/src/CObject.h>
#include "glm\vec3.hpp"

const int SZ_NEWMTL  = sizeof("newmtl")-1;
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
const int SZ_MAPBUMP = sizeof("map_Bump")-1;

namespace Engine
{
	namespace Graphics
	{
		class CMaterial : public Engine::Base::CObject
		{
		public:
			CMaterial(const std::string& file);
			~CMaterial();

			void clear();

			std::string getMaterialName();

		public:
			/* Material Fields - fields to be passed to shader - can be several materials */
			float m_fNs;
			glm::vec3 m_vKa; //Ambient color (in RGB)
			glm::vec3 m_vKd; //Diffuse color (in RGB)
			glm::vec3 m_vKs; //Specular color (in RGB)
			float m_fNi;
			float m_fd;
			GLuint m_iIllum;
			GLuint m_iMap_ka_texture;   //Texture id for Ambient
			GLuint m_iMap_ks_texture;   //Texture id for Specular
			GLuint m_iMap_kd_texture;   //Texture id for Diffuse
			GLuint m_iMap_Bump_texture; //Texture id for Bump mapping

			std::string m_sMap_ka;   //Texture ambient map
			std::string m_sMap_kd;   //Texture diffuse map
			std::string m_sMap_ks;   //Texture specular map
			std::string m_sMap_Bump; //Texture bump mapping
		};
	}
}

#endif

//Special values from material file:
//Ns	Exponent specular light. Values from 0 to 1000
//Ni	Optical density -> from 0.001 to 10.0 value=1.0 Not blend value=1.5 glass
//d		Dissolve
//
//
//illum		Illumination    Properties that are turned on in the
//0		Color on and Ambient off
//1		Color on and Ambient on
//2		Highlight on
//3		Reflection on and Ray trace on
//4		Transparency: Glass on
//		 Reflection : Ray trace on
//5		Reflection : Fresnel on and Ray trace on
//6		Transparency : Refraction on
//		 Reflection : Fresnel off and Ray trace on
//7		Transparency : Refraction on
//		 Reflection : Fresnel on and Ray trace on
//8		Reflection on and Ray trace off
//9		Transparency : Glass on
//	     Reflection : Ray trace off
//10	Casts shadows onto invisible surfaces
