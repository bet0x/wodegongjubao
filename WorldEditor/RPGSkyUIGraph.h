#pragma once
#include "UIGraph.h"
#include "TextRender.h"

class RPGSkyUIGraph:public UIGraph
{
public:
	void frameUpdate();
	void DrawRect(const CRect<float>& rcDest, Color32 color);
	void FillRect(const CRect<float>& rcDest, Color32 color);
	void DrawPolyLine(POINT* apPoints, UINT nNumPoints, Color32 color);
	void* createTexture(const char* szTexture);
	void releaseTexture(void* pTexture);
	void DrawSprite(const CRect<float>& rcSrc, const CRect<float>& rcDest, void* pTexture, Color32 color);
	void DrawSprite(const CRect<float>& rcDest, void* pTexture, Color32 color);
	void DrawSprite3x3Grid(const CRect<float>& rcSrc, const CRect<float>& rcCenterSrc, const CRect<float>& rcDest,void* pTexture, Color32 color);
	void CalcTextRect(const std::wstring& wstrText, CRect<float>& rcDest);
	void DrawText(const std::wstring& wstrText, CUIStyle& style, int nIndex, const CRect<float>& rcDest, int nCount = -1);
	void drawText(const std::wstring& strText, int cchText, const RECT& rc, UINT format, unsigned long color = 0xffffffff, RECT* prcRet=NULL);
	void InitFont(const std::wstring& wstrFontName, size_t size);
	int	GetFontSize();
	Matrix setUIMatrix(const Matrix& mTransform, const CRect<float>& rc, const Vec3D& vTranslation, const Vec3D& vRotate);
	bool scriptStringAnalyse(CScriptStringAnalysis& analysis, const std::wstring& strText);
	void playSound(const char* szFilename);
	CTextRender& getTextRender();
};