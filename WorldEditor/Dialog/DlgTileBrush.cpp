#include "DlgTileBrush.h"
#include "Terrain.h"
// For Material
#include "DlgMainEditor.h"

CDlgTileBrush::CDlgTileBrush()
{
}

CDlgTileBrush::~CDlgTileBrush()
{	
}

void CDlgTileBrush::OnControlRegister()
{
	CDlgBrush::OnControlRegister();

	RegisterControl( "IDC_OBJLIST_TILES", m_ObjListTiles);
	RegisterControl("IDC_RADIO_BTN_FIRST_LAYER",	m_RadioBtnFirstLayer);

	RegisterControlEvent("IDC_RADIO_BTN_FIRST_LAYER",	(PEVENT)&CDlgTileBrush::OnRadioBtnLayerChanged);
	RegisterControlEvent("IDC_RADIO_BTN_SECOND_LAYER",	(PEVENT)&CDlgTileBrush::OnRadioBtnLayerChanged);

	RegisterEvent("EDIT_TILE_MATERIAL", (PEVENT)&CDlgTileBrush::OnBtnTileEdit);
}

bool CDlgTileBrush::OnInitDialog()
{
	//m_ObjListTiles.m_ListBoxObject.SetSelectedByIndex(0);
	return CDlgBrush::OnInitDialog();
}

void CDlgTileBrush::OnRadioBtnLayerChanged()
{
	getBrush().SetTileLayer(m_RadioBtnFirstLayer.IsChecked()?0:1);
}

void CDlgTileBrush::initTiles()
{
	m_ObjListTiles.initObject(getDisplay().getScene());
}

void CDlgTileBrush::OnBtnTileEdit()
{
	getMainDialog().getMaterialDialog().setMaterial(m_ObjListTiles.getSelectedTileMaterial(),"");
	getMainDialog().getMaterialDialog().SetVisible(true);
}