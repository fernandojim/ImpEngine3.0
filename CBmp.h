/*****************************************
  File:       CBmp.h
  Purpose:    Class for a bmp image container
  class name: CBmp
  ---------------------------------------
  @Author: Yo
  @Version: 0
 *****************************************/

#ifndef _CBMP_H_
#define _CBMP_H_

#include <iostream>
#include <fstream>
#include <windows.h>

#include "cobject.h"

/////////////////////////////////////////
//Estructura que contiene la cabecera del 
//  archivo .bmp
/*typedef struct tagBITMAPFILEHEADER {
        WORD    bfType; 
        DWORD   bfSize; 
        WORD    bfReserved1; 
        WORD    bfReserved2; 
        DWORD   bfOffBits; 
} BITMAPFILEHEADER;*/
//////////////////////////////////////////


//////////////////////////////////////////
//Contiene información específica de la
//  imagen
/*typedef struct tagBITMAPINFOHEADER {
        DWORD  biSize; 
        LONG   biWidth; 
        LONG   biHeight; 
        WORD   biPlanes; 
        WORD   biBitCount; 
        DWORD  biCompression; 
        DWORD  biSizeImage; 
        LONG   biXPelsPerMeter; 
        LONG   biYPelsPerMeter; 
        DWORD  biClrUsed; 
        DWORD  biClrImportant; 
} BITMAPINFOHEADER;*/

//////////////////////////////////////////
//Información sobre la paleta de colores
/*typedef struct tagRGBQUAD {
  BYTE    rgbBlue; 
  BYTE    rgbGreen; 
  BYTE    rgbRed; 
  BYTE    rgbReserved; 
} RGBQUAD;*/


namespace Engine
{
	namespace Graphics
	{
		//////////////////////////////////////////
		//Clase archivo BMP
		class CBmp : Engine::Base::CObject
		{
		private:
			bool m_bOk;

			BITMAPFILEHEADER m_sBmfh;
			BITMAPINFOHEADER m_sBmih;
			DWORD             m_dSize;

			/////////////////////////////////////////////////////
			//Estas tres propiedades serán necesarias para OpenGL
			BYTE             *m_pPixelData;  //Puntero a lista con los datos de los pixels
			LONG              m_lWidth;      //Ancho de la imagen
			LONG              m_lHeight;     //Alto de la imagen
			/////////////////////////////////////////////////////

			float texfactor(int inter, float h1, float h2);
			BYTE* tex_gen(BYTE *tex[4], BYTE heighmap);
			void  clearPixelData();

			void clear();
			bool loadBMP (const std::string& sfile);

		public:
			CBmp(const std::string& sfile);
			~CBmp();

			bool getResult();
			bool isSquare();

			BYTE* getPixelData();
			LONG  getWidth();
			LONG  getHeight();
		};
	}
}

#endif


