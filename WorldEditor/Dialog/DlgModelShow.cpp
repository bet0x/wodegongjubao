#include "DlgModelShow.h"
#include "ModelDisplay.h"
#include "..\MainRoot.h"

void CDlgModelShow::OnControlRegister()
{
	CUIDialog::OnControlRegister();

	RegisterControl("IDC_CHECKBOX_BONES",		m_CheckBoxBones);
	RegisterControl("IDC_CHECKBOX_BOUNDS",		m_CheckBoxBounds);
	RegisterControl("IDC_CHECKBOX_MODEL",		m_CheckBoxModel);
	RegisterControl("IDC_CHECKBOX_WIREFRAME",	m_CheckBoxWireframe);
	RegisterControl("IDC_CHECKBOX_MATERIAL",	m_CheckBoxMaterial);
	RegisterControl("IDC_CHECKBOX_PARTICLES",	m_CheckBoxParticles);

	RegisterControlEvent("IDC_CHECKBOX_BONES",		(PEVENT)&CDlgModelShow::OnUpdateShow);
	RegisterControlEvent("IDC_CHECKBOX_BOUNDS",		(PEVENT)&CDlgModelShow::OnUpdateShow);
	RegisterControlEvent("IDC_CHECKBOX_MODEL",		(PEVENT)&CDlgModelShow::OnUpdateShow);
	RegisterControlEvent("IDC_CHECKBOX_WIREFRAME",	(PEVENT)&CDlgModelShow::OnUpdateShow);
	RegisterControlEvent("IDC_CHECKBOX_MATERIAL",	(PEVENT)&CDlgModelShow::OnUpdateShow);
	RegisterControlEvent("IDC_CHECKBOX_PARTICLES",	(PEVENT)&CDlgModelShow::OnUpdateShow);
}

void CDlgModelShow::OnUpdateShow()
{
	CMainRoot::getInstance().getMainDialog().getModelDisplay().m_bShowBones		= m_CheckBoxBones.IsChecked();
	CMainRoot::getInstance().getMainDialog().getModelDisplay().m_bShowBounds		= m_CheckBoxBounds.IsChecked();
	CMainRoot::getInstance().getMainDialog().getModelDisplay().m_bShowModel		= m_CheckBoxModel.IsChecked();
	CMainRoot::getInstance().getMainDialog().getModelDisplay().m_bShowWireframe	= m_CheckBoxWireframe.IsChecked();
	CMainRoot::getInstance().getMainDialog().getModelDisplay().m_bShowMaterial	= m_CheckBoxMaterial.IsChecked();
	CMainRoot::getInstance().getMainDialog().getModelDisplay().m_bShowParticles	= m_CheckBoxParticles.IsChecked();
}