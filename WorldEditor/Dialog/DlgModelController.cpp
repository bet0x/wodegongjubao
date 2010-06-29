#include "DlgModelController.h"

CDlgModelController::CDlgModelController()
{
}

CDlgModelController::~CDlgModelController()
{	
}

void CDlgModelController::OnControlRegister()
{
	CUIDialog::OnControlRegister();
	RegisterControl("IDC_TAB_SHOW",	m_RadioButtonShow);
	RegisterControl("IDC_TAB_ANIM",	m_RadioButtonAnim);
	RegisterControl("IDC_TAB_RENDERPASS",	m_RadioButtonRenderPass);
	RegisterControl("IDC_TAB_PARTICLE",	m_RadioButtonParticle);

	m_DlgModelShow.Create("IDD_MODEL_SHOW", this);
	m_DlgModelAnim.Create("IDD_MODEL_ANIM", this);
	m_DlgModelRenderPass.Create("IDD_MODEL_RENDERPASS", this);
	m_DlgModelParticle.Create("IDD_MODEL_ABOUT", this);

	RegisterControlEvent("IDC_TAB_SHOW",		(PEVENT)&CDlgModelController::OnTabChanged);
	RegisterControlEvent("IDC_TAB_ANIM",		(PEVENT)&CDlgModelController::OnTabChanged);
	RegisterControlEvent("IDC_TAB_RENDERPASS",	(PEVENT)&CDlgModelController::OnTabChanged);
	RegisterControlEvent("IDC_TAB_PARTICLE",	(PEVENT)&CDlgModelController::OnTabChanged);

	RegisterControlEvent("IDC_BTN_CLOSE",		(PEVENT)&CDlgModelController::OnClose);
}

bool CDlgModelController::OnInitDialog()
{
	OnTabChanged();
	return true;
}

void CDlgModelController::OnUpdate()
{
	m_DlgModelAnim.ResetAnim();
	m_DlgModelRenderPass.onReset();
}

void CDlgModelController::OnTabChanged()
{
	m_DlgModelShow.SetVisible(m_RadioButtonShow.IsChecked());
	m_DlgModelAnim.SetVisible(m_RadioButtonAnim.IsChecked());
	m_DlgModelRenderPass.SetVisible(m_RadioButtonRenderPass.IsChecked());
	m_DlgModelParticle.SetVisible(m_RadioButtonParticle.IsChecked());
}

void CDlgModelController::OnClose()
{
	SetVisible(false);
}