#include "DlgModelShow.h"
#include "ModelDisplay.h"

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
	getModelDisplay().m_bShowBones		= m_CheckBoxBones.IsChecked();
	getModelDisplay().m_bShowBounds		= m_CheckBoxBounds.IsChecked();
	getModelDisplay().m_bShowModel		= m_CheckBoxModel.IsChecked();
	getModelDisplay().m_bShowWireframe	= m_CheckBoxWireframe.IsChecked();
	getModelDisplay().m_bShowMaterial	= m_CheckBoxMaterial.IsChecked();
	getModelDisplay().m_bShowParticles	= m_CheckBoxParticles.IsChecked();
}