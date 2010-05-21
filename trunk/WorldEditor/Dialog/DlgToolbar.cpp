#include "DlgToolbar.h"

CDlgToolbar::CDlgToolbar()
{
}

CDlgToolbar::~CDlgToolbar()
{	
}

void CDlgToolbar::OnControlRegister()
{
	CUIDialog::OnControlRegister();

	m_dlgSetting.Create("IDD_EDITOR_SETTING", this);
	m_DlgTerrainEditor.Create("IDD_EDITOR_TERRAIN", this);
	m_DlgToolObject.Create("IDD_OBJECT_BRUSH", this);

	RegisterControlEvent("IDC_BTN_SETTING",			(PEVENT)&CDlgToolbar::OnBtnSetting);
	RegisterControlEvent("IDC_BTN_TERRAIN",			(PEVENT)&CDlgToolbar::OnBtnTerrain);
	RegisterControlEvent("IDC_BTN_OBJECT",			(PEVENT)&CDlgToolbar::OnBtnObject);
}

bool CDlgToolbar::OnInitDialog()
{
	OnBtnSetting();
	return true;
}

void CDlgToolbar::OnBtnSetting()
{
	closeAllChildDialog();
	m_dlgSetting.SetVisible(true);
}

void CDlgToolbar::OnBtnTerrain()
{
	closeAllChildDialog();
	m_DlgTerrainEditor.SetVisible(true);
	getDisplay().getTerrain().showBrushDecal(true);
}

void CDlgToolbar::OnBtnObject()
{
	getBrush().SetBrushType(CTerrainBrush::BRUSH_TYPE_SCENE_OBJECT);
	closeAllChildDialog();
	m_DlgToolObject.SetVisible(true);
	getDisplay().getTerrain().showBrushDecal(false);
}

void CDlgToolbar::closeAllChildDialog()
{
	m_dlgSetting.SetVisible(false);
	m_DlgTerrainEditor.SetVisible(false);
	m_DlgToolObject.SetVisible(false);
}

void CDlgToolbar::reset()
{
	m_DlgTerrainEditor.reset();
	m_DlgToolObject.initObject();
	m_dlgSetting.init();
}