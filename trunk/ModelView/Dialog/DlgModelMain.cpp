#include "DlgModelMain.h"
#include "../Config/Config.h"

CDlgModelMain::CDlgModelMain()
{
}

CDlgModelMain::~CDlgModelMain()
{	
}

void CDlgModelMain::OnControlRegister()
{
	CUIDialog::OnControlRegister();
	RegisterControl("IDC_DISPLAY_MODEL", m_ModelDisplay);

	RegisterControl("IDC_BTN_HIDE",	m_BtnHide);
	RegisterControl("IDC_BTN_SHOW",	m_BtnShow);

	m_DlgModelController.Create("IDD_MODEL_CONTROLLER", this);
	//m_DlgFaceDetect.Create("IDD_FACE_DETECT", this);

	RegisterControlEvent("IDC_BTN_SHOW",		(PEVENT)&CDlgModelMain::OnBtnShowController);
	RegisterControlEvent("IDC_BTN_FACE_DETECT",	(PEVENT)&CDlgModelMain::OnBtnFaceDetect);
}

bool CDlgModelMain::OnInitDialog()
{
	//m_DlgFaceDetect.SetVisible(false);

	m_BtnHide.SetVisible(true);
	m_BtnShow.SetVisible(false);
	return true;
}

void CDlgModelMain::CloseController()
{
	m_BtnHide.SetVisible(false);
	m_BtnShow.SetVisible(true);
	m_DlgModelController.SetVisible(false);
	m_ModelDisplay.SetLocation(0,0);
	m_ModelDisplay.OnSize(m_rcBoundingBox);
}

void CDlgModelMain::OnBtnShowController()
{
	m_BtnHide.SetVisible(true);
	m_BtnShow.SetVisible(false);
	m_DlgModelController.SetVisible(true);
	m_ModelDisplay.SetLocation(200,0);
	m_ModelDisplay.OnSize(m_rcBoundingBox);
}

void CDlgModelMain::OnBtnFaceDetect()
{
	/*if (!m_DlgFaceDetect.IsVisible())
	{
		m_DlgFaceDetect.SetAR(true);
		m_DlgFaceDetect.SetVisible(true);
	}
	else if (!m_DlgFaceDetect.GetAR())
	{
		m_DlgFaceDetect.SetVisible(false);
	}
	else
	{
		m_DlgFaceDetect.SetAR(false);
	}*/
}

CModelDisplay&  CDlgModelMain::getModelDisplay()
{
	return m_ModelDisplay;
}