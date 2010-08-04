// ------------------------------
// Designed by RPGSky
// Date : 2010-8-4
// ------------------------------
// 
#pragma once
#include "TextRender.h"
#include "Texture.h"

class CRPGSkyTextRender:public CTextRender
{
public:
	CRPGSkyTextRender();
	virtual void updateTextureBuffer(unsigned char* pBuffer, size_t size, size_t width, size_t height);
	virtual void drawTextVertexBuffer(int nVertexCount, void* pBuffer);
protected:
	CTexture*	m_pTextTexture;
}; 