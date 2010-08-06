#pragma once
#include "UIGraph.h"
#include "TextRender.h"

class RPGSkyUIGraph:public UIGraph
{
public:
	void DrawRect(const CRect<float>& rcDest, Color32 color);
	void FillRect(const CRect<float>& rcDest, Color32 color);
	void DrawPolyLine(POINT* apPoints, UINT nNumPoints, Color32 color);
	void DrawSprite(const CRect<float>& rcSrc, const CRect<float>& rcDest, const std::string& strTexture, Color32 color);
	void DrawSprite(const CRect<float>& rcDest, const std::string& strTexture, Color32 color);
	void DrawSprite3x3Grid(const CRect<float>& rcSrc, const CRect<float>& rcCenterSrc, const CRect<float>& rcDest,const std::string& strTexture, Color32 color);
	void CalcTextRect(const std::wstring& wstrText, CRect<float>& rcDest);
	void DrawText(const std::wstring& wstrText, CUIStyle& style, int nIndex, const CRect<float>& rcDest, bool bShadow = false, int nCount = -1);
	void InitFont(const std::wstring& wstrFontName, uint32 uSize);
	int	GetFontSize();
	Matrix setUIMatrix(const Matrix& mTransform, const CRect<float>& rc, const Vec3D& vTranslation, const Vec3D& vRotate);
	CTextRender& getTextRender();
};