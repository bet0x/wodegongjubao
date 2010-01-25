#include "DlgTerrainEditor.h"
#include "Terrain.h"

CDlgTerrainEditor::CDlgTerrainEditor()
{
}

CDlgTerrainEditor::~CDlgTerrainEditor()
{	
}

void CDlgTerrainEditor::OnControlRegister()
{
	CDlgBaseEditor::OnControlRegister();

	m_dlgTerrainHeightBrush.Create("IDD_TERRAIN_BRUSH_HEIGHT",	this);
	m_dlgTerrainAttributeBrush.Create("IDD_TERRAIN_BRUSH_ATT",	this);
	m_dlgTerrainTileBrush.Create("IDD_TERRAIN_TILE_BRUSH",		this);
	m_dlgTerrainAlphaBrush.Create("IDD_TERRAIN_BRUSH_ALPHA",	this);

	RegisterControlEvent("IDC_BTN_HEIGHT",			(PEVENT)&CDlgTerrainEditor::OnBtnTerrainHeight);
	RegisterControlEvent("IDC_BTN_ATTRIBUTE",		(PEVENT)&CDlgTerrainEditor::OnBtnTerrainAttribute);
	RegisterControlEvent("IDC_BTN_TEXTURE_PAINT",	(PEVENT)&CDlgTerrainEditor::OnBtnTerrainTile);
	RegisterControlEvent("IDC_BTN_ALPHA_PAINT",		(PEVENT)&CDlgTerrainEditor::OnBtnTerrainAlpha);
}

bool CDlgTerrainEditor::OnInitDialog()
{
	OnBtnTerrainHeight();
	return true;
}

void CDlgTerrainEditor::closeAllChildDialog()
{
	m_dlgTerrainHeightBrush.SetVisible(false);
	m_dlgTerrainAttributeBrush.SetVisible(false);
	m_dlgTerrainTileBrush.SetVisible(false);
	m_dlgTerrainAlphaBrush.SetVisible(false);
}

void CDlgTerrainEditor::OnBtnTerrainHeight()
{
	getBrush().SetBrushType(CTerrainBrush::BRUSH_TYPE_TERRAIN_HEIGHT);
	closeAllChildDialog();
	m_dlgTerrainHeightBrush.SetVisible(true);
}

void CDlgTerrainEditor::OnBtnTerrainAttribute()
{
	getBrush().SetBrushType(CTerrainBrush::BRUSH_TYPE_TERRAIN_ATT);
	closeAllChildDialog();
	m_dlgTerrainAttributeBrush.SetVisible(true);
}

void CDlgTerrainEditor::OnBtnTerrainTile()
{
	getBrush().SetBrushType(CTerrainBrush::BRUSH_TYPE_TERRAIN_TEXTURE_PAINT);
	closeAllChildDialog();
	m_dlgTerrainTileBrush.SetVisible(true);
}

void CDlgTerrainEditor::OnBtnTerrainAlpha()
{
	getBrush().SetBrushType(CTerrainBrush::BRUSH_TYPE_TERRAIN_TEXTURE_ALPHA_PAINT);
	closeAllChildDialog();
	m_dlgTerrainAlphaBrush.SetVisible(true);
}

void CDlgTerrainEditor::reset()
{
	m_dlgTerrainTileBrush.initTiles();
}