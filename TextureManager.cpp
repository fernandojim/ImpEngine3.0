/*
 * TextureManager.cpp
 *
 *  Created on: 21 de nov. de 2017
 *      Author: fjimartinez
 */

#include "TextureManager.h"
#include "utils.h"

using namespace Engine::Managers;

TextureManager& TextureManager::getSingleton()
{
	static TextureManager tm;

	return tm;
}

TextureManager::TextureManager()
{
	clear();
}

TextureManager::~TextureManager()
{
	clear();
}

void TextureManager::clear()
{
	m_Textures.clear();
}

/*
	Load texture from disk and returns its id
*/
GLuint TextureManager::loadTexture(const std::string& sTexturePath)
{
	std::shared_ptr<Engine::Graphics::CTexture> tex;
	std::shared_ptr<Engine::Graphics::CTexture> new_tex;
	GLuint tex_id = 0;

	//Run the list searching if texture is already loaded
	for (auto iter : m_Textures)
	{
		//Gets the CTexture information in the iter' iteration
		tex = iter.second;

	    assert(tex);

		//Test if the texture is already loaded
		if (!tex->getName().compare(sTexturePath))
			return tex->getTextureId();
	}

	//Add the new texture and its id
	new_tex = std::make_shared<Engine::Graphics::CTexture>(sTexturePath);

	assert(new_tex);

	switch(new_tex->getResult())
	{
		case Engine::Graphics::loadTextureResult::FILE_ERROR:
			Message::MessageBOX("Texture file not found", sTexturePath.c_str());
		break;
		case Engine::Graphics::loadTextureResult::TEXTURE_NOT_VALID:
			Message::MessageBOX("Texture not valid", sTexturePath.c_str());
		break;
		case Engine::Graphics::loadTextureResult::TEXTURE_OK:
			tex_id = new_tex->getTextureId();
			m_Textures.insert(std::pair < GLuint, std::shared_ptr<Engine::Graphics::CTexture> >(tex_id, new_tex));
		break;
		default:
		break;
	}

	return tex_id;
}

/*
	Get texture from index
*/
std::shared_ptr<Engine::Graphics::CTexture> TextureManager::getTextureByID(GLuint id)
{
	std::map < GLuint, std::shared_ptr<Engine::Graphics::CTexture> >::iterator iter;

	//Gets the map of id
	iter = m_Textures.find(id);

	return iter->second;
}

/*
	Delete texture with ID index
*/
bool TextureManager::deleteTexture(GLuint id)
{
	std::map < GLuint, std::shared_ptr<Engine::Graphics::CTexture> >::iterator iter;

	//Gets the map of 'id'
	iter = m_Textures.find(id);

	//If it exists
	if (iter != m_Textures.end())
	{
		m_Textures.erase(iter);
		return true;
	}
	else
		return false;
}
