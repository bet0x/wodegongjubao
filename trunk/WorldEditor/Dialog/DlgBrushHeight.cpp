#include "DlgBrushHeight.h"
#include "Terrain.h"

CDlgBrushHeight::CDlgBrushHeight()
{
}

CDlgBrushHeight::~CDlgBrushHeight()
{	
}

void CDlgBrushHeight::OnControlRegister()
{
	CDlgBrush::OnControlRegister();

	RegisterControl("IDC_NUM_MIN",	m_NumMin);
	RegisterControl("IDC_NUM_MAX",	m_NumMax);

	RegisterControl("IDC_RADIO_BTN_NORMAL",	m_RadioBtnNoraml);
	RegisterControl("IDC_RADIO_BTN_SMOOTH",	m_RadioBtnSmooth);

	RegisterControlEvent("IDC_NUM_MIN",	(PEVENT)&CDlgBrushHeight::OnHeightRangChanged);
	RegisterControlEvent("IDC_NUM_MAX",	(PEVENT)&CDlgBrushHeight::OnHeightRangChanged);


	RegisterControlEvent("IDC_RADIO_BTN_NORMAL",	(PEVENT)&CDlgBrushHeight::OnBrushHeightTypeChanged);
	RegisterControlEvent("IDC_RADIO_BTN_SMOOTH",	(PEVENT)&CDlgBrushHeight::OnBrushHeightTypeChanged);
}

bool CDlgBrushHeight::OnInitDialog()
{
#if defined(_MU)
	m_NumMin.setFloat(0.0f);
	m_NumMax.setFloat(255.0f*0.015f);
#else
	m_NumMin.setFloat(-100.0f);
	m_NumMax.setFloat(100.0f);
#endif
	OnHeightRangChanged();
	//m_ListBoxTiles.SetSelectedByIndex(0);
	return CDlgBrush::OnInitDialog();
}

void CDlgBrushHeight::OnHeightRangChanged()
{
	getBrush().setHeightRang(m_NumMin.getFloat(),m_NumMax.getFloat());
}

void CDlgBrushHeight::OnBrushHeightTypeChanged()
{
	getBrush().setBrushHeightType(m_RadioBtnNoraml.IsChecked()?CTerrainBrush::BHT_NORMAL:CTerrainBrush::BHT_SMOOTH);
}