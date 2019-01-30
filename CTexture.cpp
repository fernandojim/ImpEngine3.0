#include <SDL_image.h>
#include <memory>

#include "CTexture.h"
#include "utils.h"

using namespace Message;
using namespace Engine::Graphics;

CTexture::CTexture(const std::string& sFilename) : Engine::Base::CObject(sFilename)
{
	std::unique_ptr<CBmp> bmp;
	std::ifstream f;

	//Result by default
	m_result = loadTextureResult::TEXTURE_OK;

	//Aligment is RGB by default
	m_rgbAlignment = GL_RGB;

	//Texture ID
	m_textureId = 0;

	//Test if the file exists
	f.open(sFilename);
	if (f.good())
	{
		// If the texture is a JPG file
		if (sFilename.find(".jpg") != std::string::npos || sFilename.find(".JPG") != std::string::npos || sFilename.find(".tga") != std::string::npos)
		{
			//Load the JPG associated to the texture
			SDL_Surface* image = IMG_Load ( sFilename.c_str() );

			//Test if an image was loaded
			if ( image != nullptr && image->h > 0 && image->w > 0 )
			{
				m_lHeight = image->h;
				m_lWidth  = image->w;
				m_Pixeldata = (BYTE*)image->pixels;

				//Test if RGB internal aligment is RGB or BGR - for that, evaluate Rmask field if it is FF0000 or 0000FF
				if ( (image->format->Rmask & 0xFF) == 0 )
					m_rgbAlignment = GL_BGR;
			}
			else
				m_result = loadTextureResult::TEXTURE_NOT_VALID;

		}
		else if (sFilename.find(".bmp") != std::string::npos)
		{
			//Load the BMP associated to the texture
			bmp = std::unique_ptr<CBmp>(new CBmp(sFilename.c_str()));

			//Test if an image was loaded
			if ( bmp != nullptr && bmp->getResult() && bmp->getHeight() > 0 && bmp->getWidth() > 0 )
			{
				m_lHeight = bmp->getHeight();
				m_lWidth  = bmp->getWidth();
				m_Pixeldata = bmp->getPixelData();
			}
			else
				m_result = loadTextureResult::TEXTURE_NOT_VALID;
		}
	}
	else
		m_result = loadTextureResult::FILE_ERROR;

	//Bind and creates an id for the texture if no fail
	if (m_result == loadTextureResult::TEXTURE_OK)
		bindTexture();
}

void CTexture::bindTexture()
{
	assert(m_Pixeldata);

	glGenTextures(1, &m_textureId);
	glBindTexture(GL_TEXTURE_2D, m_textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_lWidth, m_lHeight, 0, m_rgbAlignment, GL_UNSIGNED_BYTE, m_Pixeldata);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

GLuint CTexture::getTextureId()
{
	return m_textureId;
}

loadTextureResult CTexture::getResult()
{
	return m_result;
}

CTexture::~CTexture()
{

}
