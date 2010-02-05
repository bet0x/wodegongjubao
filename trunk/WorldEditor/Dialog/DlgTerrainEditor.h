#pragma once
#include "DlgBaseEditor.h"
#include "DlgBrushAtt.h"
#include "DlgBrushHeight.h"
#include "DlgTileBrush.h"

class CDlgTerrainEditor : public CDlgBaseEditor
{
public:
	CDlgTerrainEditor();
	~CDlgTerrainEditor();
public:
	virtual void OnControlRegister();
	virtual bool OnInitDialog();
	virtual void SetVisible(bool bVisible);

	void OnBtnTerrainHeight();
	void OnBtnTerrainAttribute();
	void OnBtnTerrainTile();
	void OnBtnTerrainAlpha();

	void OnHeightRangChanged();
	void OnBrushHeightTypeChanged();
	void reset();
private:
	void closeAllChildDialog();

	CDlgBrushHeight	m_dlgTerrainHeightBrush;
	CDlgBrushAtt	m_dlgTerrainAttributeBrush;
	CDlgTileBrush	m_dlgTerrainTileBrush;
	CDlgBrush		m_dlgTerrainAlphaBrush;
};