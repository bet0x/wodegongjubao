#include "DlgTileBrush.h"
#include "Terrain.h"

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