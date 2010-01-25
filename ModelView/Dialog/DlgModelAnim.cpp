#include "DlgModelAnim.h"

#include "ModelDisplay.h"
#include "DlgModelMain.h"

CDlgModelBaseDialog::CDlgModelBaseDialog()
{
}

CDlgModelBaseDialog::~CDlgModelBaseDialog()
{	
}

CDlgModelMain& CDlgModelBaseDialog::getDlgModelMain()
{
	assert(GetParentDialog());
	return ((CDlgModelBaseDialog*)GetParentDialog())->getDlgModelMain();
}

CModelDisplay& CDlgModelBaseDialog::getModelDisplay()
{
	return getDlgModelMain().getModelDisplay();
}

void CDlgModelAnim::OnControlRegister()
{
	CUIDialog::OnControlRegister();

	RegisterControl("IDC_COMBOBOX_ANIM",m_ComboBoxAnim);
	RegisterControl("IDC_STATIC_SPEED",	m_StaticSpeed);
	RegisterControl("IDC_SLIDER_SPEED",	m_SliderSpeed);
	RegisterControl("IDC_STATIC_FRAME",	m_StaticFrame);
	RegisterControl("IDC_SLIDER_FRAME",	m_SliderFrame);
	RegisterControl("IDC_BTN_PLAY",		m_BtnPlay);
	RegisterControl("IDC_BTN_PREV",		m_BtnPrev);
	RegisterControl("IDC_BTN_NEXT",		m_BtnNext);

	RegisterControlEvent("IDC_COMBOBOX_ANIM",	(PEVENT)&CDlgModelAnim::OnAnimChanged);
	RegisterControlEvent("IDC_SLIDER_SPEED",	(PEVENT)&CDlgModelAnim::OnSpeedChanged);
	RegisterControlEvent("IDC_SLIDER_FRAME",	(PEVENT)&CDlgModelAnim::OnFrameChanged);

	RegisterControlEvent("IDC_BTN_PLAY",	(PEVENT)&CDlgModelAnim::OnPlay);
	RegisterControlEvent("IDC_BTN_PREV",	(PEVENT)&CDlgModelAnim::OnPrev);
	RegisterControlEvent("IDC_BTN_NEXT",	(PEVENT)&CDlgModelAnim::OnNext);
}

bool CDlgModelAnim::OnInitDialog()
{
	m_SliderSpeed.SetRange(1, 50);
	m_SliderSpeed.SetValue(10);
	//OnUpdateShow();
	return true;
}

void CDlgModelAnim::OnFrameMove(double fTime, float fElapsedTime)
{
	if (getModelDisplay().m_pModelObject&&getModelDisplay().m_pModelObject->m_AnimMgr)
	{
		m_SliderFrame.SetValue(getModelDisplay().m_pModelObject->m_AnimMgr->GetFrame());
		std::wstring wstr = FormatW(L"%d", m_SliderFrame.GetValue()-m_SliderFrame.getMin());
		m_StaticFrame.SetText(wstr);
	}
	CUIDialog::OnFrameMove(fTime,fElapsedTime);
}

void CDlgModelAnim::ResetAnim()
{
	int nSelected = m_ComboBoxAnim.GetSelectedItemIndex();
	m_ComboBoxAnim.RemoveAllItems();
	if (getModelDisplay().m_pModelObject&&getModelDisplay().m_pModelObject->m_pModel)
	{
		size_t uAnimSize = getModelDisplay().m_pModelObject->m_pModel->m_AnimList.size();
		if (uAnimSize>0)
		{
			for (size_t i=0; i<uAnimSize; ++i)
			{
				m_ComboBoxAnim.AddItem(FormatW(L"[%d]",i));
			}
			if (nSelected>=uAnimSize)
			{
				nSelected = uAnimSize-1;
			}
			m_ComboBoxAnim.SetSelectedByIndex(nSelected);
		}
		OnSpeedChanged();
	}
	//OnUpdateShow();
}

void CDlgModelAnim::OnPlay()
{
	if (getModelDisplay().m_pModelObject&&getModelDisplay().m_pModelObject->m_AnimMgr)
	{
		getModelDisplay().m_pModelObject->m_AnimMgr->Pause();
	}
}

void CDlgModelAnim::OnPrev()
{
	if (getModelDisplay().m_pModelObject&&getModelDisplay().m_pModelObject->m_AnimMgr)
	{
		getModelDisplay().m_pModelObject->m_AnimMgr->PrevFrame();
	}
}

void CDlgModelAnim::OnNext()
{
	if (getModelDisplay().m_pModelObject&&getModelDisplay().m_pModelObject->m_AnimMgr)
	{
		getModelDisplay().m_pModelObject->m_AnimMgr->NextFrame();
	}
}

void CDlgModelAnim::OnAnimChanged()
{
	if (getModelDisplay().m_pModelObject&&getModelDisplay().m_pModelObject->m_AnimMgr)
	{
		getModelDisplay().m_pModelObject->SetAnim(m_ComboBoxAnim.GetSelectedItemIndex());
		m_SliderFrame.SetRange(getModelDisplay().m_pModelObject->m_AnimMgr->getCurrentAnim().timeStart, getModelDisplay().m_pModelObject->m_AnimMgr->getCurrentAnim().timeEnd);
	}
}

void CDlgModelAnim::OnSpeedChanged()
{
	float fSpeed = m_SliderSpeed.GetValue()*0.1f;
	if (getModelDisplay().m_pModelObject&&getModelDisplay().m_pModelObject->m_AnimMgr)
	{
		getModelDisplay().m_pModelObject->m_AnimMgr->SetSpeed(fSpeed);
	}
	std::wstring wstr = FormatW(L"%.1fx", fSpeed);
	m_StaticSpeed.SetText(wstr);
}

void CDlgModelAnim::OnFrameChanged()
{
	if (getModelDisplay().m_pModelObject&&getModelDisplay().m_pModelObject->m_AnimMgr)
	{
		int nFrame = m_SliderFrame.GetValue();
		getModelDisplay().m_pModelObject->m_AnimMgr->SetFrame(nFrame);
	}
}