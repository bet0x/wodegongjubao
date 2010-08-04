#include "RPGSkyUIGraph.h"
#include "RenderSystem.h"
#include "Graphics.h"
#include "RPGSkyTextRender.h"

// 
static RPGSkyUIGraph g_uiGraph;
UIGraph* UIGraph::m_pInstace = &g_uiGraph;

struct UI_SCREEN_VERTEX
{
	float x, y, z, h;
	Color32 color;
	float tu, tv;

	static DWORD FVF;
};
DWORD UI_SCREEN_VERTEX::FVF = FVF_XYZRHW | FVF_DIFFUSE | FVF_TEX1;


struct UI_SCREEN_VERTEX_UNTEX
{
	float x, y, z, h;
	Color32 color;

	static DWORD FVF;
};
DWORD UI_SCREEN_VERTEX_UNTEX::FVF = FVF_XYZRHW | FVF_DIFFUSE;

void RPGSkyUIGraph::DrawRect(const CRect<float>& rcDest, Color32 color)
{
	CRenderSystem& R = GetRenderSystem();

	// Since we're doing our own drawing here we need to flush the sprites
	R.SetFVF(UI_SCREEN_VERTEX::FVF);

	R.SetTextureColorOP(0, TBOP_SOURCE2);
	R.SetTextureAlphaOP(0, TBOP_SOURCE2);

	GetGraphics().DrawRect(rcDest, color.c);

	R.SetTextureColorOP(0, TBOP_MODULATE);
	R.SetTextureAlphaOP(0, TBOP_MODULATE);
}

void RPGSkyUIGraph::FillRect(const CRect<float>& rcDest, Color32 color)
{
	CRenderSystem& R = GetRenderSystem();

	// Since we're doing our own drawing here we need to flush the sprites
	R.SetFVF(UI_SCREEN_VERTEX::FVF);


	R.SetTextureColorOP(0, TBOP_SOURCE2);
	R.SetTextureAlphaOP(0, TBOP_SOURCE2);

	GetGraphics().FillRect(rcDest, color.c);

	R.SetTextureColorOP(0, TBOP_MODULATE);
	R.SetTextureAlphaOP(0, TBOP_MODULATE);
}

void RPGSkyUIGraph::DrawPolyLine(POINT* apPoints, UINT nNumPoints, Color32 color)
{
	UI_SCREEN_VERTEX* vertices = new UI_SCREEN_VERTEX[ nNumPoints ];
	if(vertices == NULL)
		return;

	UI_SCREEN_VERTEX* pVertex = vertices;
	POINT* pt = apPoints;
	for(UINT i=0; i < nNumPoints; i++)
	{
		pVertex->x = /*m_x +*/ (float) pt->x;
		pVertex->y = /*m_y +*/ (float) pt->y;
		pVertex->z = 0.5f;
		pVertex->h = 1.0f;
		pVertex->color = color;
		pVertex->tu = 0.0f;
		pVertex->tv = 0.0f;

		pVertex++;
		pt++;
	}
	CRenderSystem& R = GetRenderSystem();

	R.SetFVF(UI_SCREEN_VERTEX::FVF);

	R.SetTextureColorOP(0, TBOP_SOURCE2);
	R.SetTextureAlphaOP(0, TBOP_SOURCE2);

	R.DrawPrimitiveUP(VROT_LINE_STRIP, nNumPoints - 1, vertices, sizeof(UI_SCREEN_VERTEX));

	R.SetTextureColorOP(0, TBOP_MODULATE);
	R.SetTextureAlphaOP(0, TBOP_MODULATE);

	S_DELS(vertices);
}

void RPGSkyUIGraph::DrawSprite3x3Grid(const CRect<float>& rcSrc, const CRect<float>& rcCenterSrc, const CRect<float>& rcDest,const std::string& strTexture, Color32 color)
{
	if(color.a == 0)
		return;
	int TextureID = GetRenderSystem().GetTextureMgr().RegisterTexture(strTexture);
	GetGraphics().Draw3x3Grid3D(rcSrc, rcCenterSrc, rcDest, TextureID, color);
}

void RPGSkyUIGraph::DrawSprite(const CRect<float>& rcSrc, const CRect<float>& rcDest, const std::string& strTexture, Color32 color)
{
	if(color.a == 0)
		return;
	int TextureID = GetRenderSystem().GetTextureMgr().RegisterTexture(strTexture);
	GetGraphics().DrawTex3D(rcSrc, rcDest, TextureID, color);
}

void RPGSkyUIGraph::DrawSprite(const CRect<float>& rcDest, const std::string& strTexture, Color32 color)
{
	if(color.a == 0)
		return;
	int TextureID = GetRenderSystem().GetTextureMgr().RegisterTexture(strTexture);
	GetGraphics().DrawTex(rcDest, TextureID, color);
}

void RPGSkyUIGraph::CalcTextRect(const std::wstring& wstrText, CRect<float>& rcDest)
{
	if(wstrText.empty())
	{
		return;
	}
	if(wstrText[0]==L'[')
	{
		RECT rect=getTextRender().CalcUBBRect(wstrText, rcDest.getRECT());
		rcDest = rect;
	}
	else
	{
		RECT rt;
		getTextRender().drawText(wstrText, -1, rcDest.getRECT(),DTL_CALCRECT,0,&rt);
		rcDest = rt;
	}
}

void RPGSkyUIGraph::DrawText(const std::wstring& wstrText, CUIStyle& style, int nIndex,const CRect<float>& rcDest, bool bShadow, int nCount)
{
	Color32 color = 0xFFFFFFFF;//style.m_mapFont[nIndex].color.getColor();
	uint32 uFormat =  0;//style.GetCyclostyle().m_FontStyle[nIndex].uFormat;
	// No need to draw fully transparent layers
	if(color.a == 0)
		return;

	getTextRender().drawText(wstrText,nCount,rcDest.getRECT(),uFormat, color.c);
}

void RPGSkyUIGraph::InitFont(const std::wstring& wstrFontName, uint32 uSize)
{
	getTextRender().load(ws2s(wstrFontName).c_str());
	getTextRender().setFontSize(uSize);
}

int RPGSkyUIGraph::GetFontSize()
{
	return getTextRender().GetCharHeight();
}

CTextRender& RPGSkyUIGraph::getTextRender()
{
	static CRPGSkyTextRender s_RPGSkyTextRender;
	return s_RPGSkyTextRender;
}