#include "DlgColEdit.h"
#include "..\Config\Config.h"
#include "DlgModelMain.h"


void CDlgColEditDialog::OnControlRegister()
{
	CUIDialog::OnControlRegister();

	RegisterControl("IDC_BTN_Clear",m_BtnClearColRect);
	RegisterControl("IDC_BTN_Update",m_BtnUpdateColRect);
	RegisterControl("IDC_CMB_COLTYPE",m_ColTypeCombo);
	RegisterControl("IDC_BTN_CAMERA",m_BtnCamera);
	RegisterControl("IDC_BTN_SAVE",m_BtnSave);
	RegisterControl("IDC_CMB_COLID",m_EditColID);

	RegisterControlEvent("IDC_BTN_Clear",	(PEVENT)&CDlgColEditDialog::OnClearColRect);
	RegisterControlEvent("IDC_BTN_Update",	(PEVENT)&CDlgColEditDialog::OnUpdateColRect);
	RegisterControlEvent("IDC_BTN_CAMERA",(PEVENT)&CDlgColEditDialog::OnCamera);
	RegisterControlEvent("IIDC_BTN_SAVE",(PEVENT)&CDlgColEditDialog::OnSave);

}

bool CDlgColEditDialog::OnInitDialog()
{
	m_ColTypeCombo.AddItem(L"·ÀÓù¿ò");
	m_ColTypeCombo.AddItem(L"¹¥»÷¿ò");
	m_ColTypeCombo.AddItem(L"Åö×²¿ò");
	return true;
}

void CDlgColEditDialog::OnClearColRect()
{
	assert(GetParentDialog());
	return ((CDlgModelMain*)GetParentDialog())->getModelDisplay().m_vecColRect.clear();
}

void CDlgColEditDialog::OnUpdateColRect()
{
	
	sFrameColRect FrameColRect;
	if(((CDlgModelMain*)GetParentDialog())->getModelDisplay().m_pModelObject)
		FrameColRect.nFrameID = ((CDlgModelMain*)GetParentDialog())->getModelDisplay().m_pModelObject->m_AnimMgr->GetFrame();
	FrameColRect.vecRect = ((CDlgModelMain*)GetParentDialog())->getModelDisplay().m_vecColRect;

	m_FrameRectMgr.AddFrameRect("act1",FrameColRect);
}

void CDlgColEditDialog::OnCamera()
{
	Vec3D vEye(0.0f, 5.0f, 2.0f);
	Vec3D vAt (0.0f, 5.0f, 4.0f);
	Vec3D vRot(0,PI*0.5f,0);

	assert(GetParentDialog());
	/*if(((CDlgModelMain*)GetParentDialog())->getModelDisplay().m_pModelObject != NULL)
		((CDlgModelMain*)GetParentDialog())->getModelDisplay().m_pModelObject->SetRotate(vRot);*/

	((CDlgModelMain*)GetParentDialog())->getModelDisplay().m_Camera.SetViewParams(vEye,vAt);
}



void CDlgColEditDialog::OnSave()
{
	if(((CDlgModelMain*)GetParentDialog())->getModelDisplay().m_pModelObject)
	{
		CString str = ((CDlgModelMain*)GetParentDialog())->getModelDisplay().m_pModelObject->getModelFilename().c_str();
		CString strExt,strFile,strPath;
		strFile = GetFileTitleFromFileName(str,strExt,strPath);

		strPath += "\\";
		strPath += strFile + ".col";


		m_FrameRectMgr.Save(strPath.GetBuffer());
	}
	

	
}

int CDlgColEditDialog::GetCurSelectRectType()
{
	return m_ColTypeCombo.GetSelectedItemIndex();
}

int CDlgColEditDialog::GetColID()
{
	return m_EditColID.GetInt();
}
