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
	if (getModelDisplay().getModelAnimManager())
	{
		m_SliderFrame.SetValue(getModelDisplay().getModelAnimManager()->GetFrame());
		std::wstring wstr = FormatW(L"%d", m_SliderFrame.GetValue()-m_SliderFrame.getMin());
		m_StaticFrame.SetText(wstr);
	}
	CUIDialog::OnFrameMove(fTime,fElapsedTime);
}

void CDlgModelAnim::ResetAnim()
{
	int nSelected = m_ComboBoxAnim.getListBox().GetSelectedIndex();
	m_ComboBoxAnim.getListBox().RemoveAllItems();
	if (getModelDisplay().getModelData())
	{
		size_t uAnimSize = getModelDisplay().getModelData()->m_AnimList.size();
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
			m_ComboBoxAnim.getListBox().SelectItem(nSelected);
		}
		OnSpeedChanged();
	}
	//OnUpdateShow();
}

void CDlgModelAnim::OnPlay()
{
	if (getModelDisplay().getModelAnimManager())
	{
		getModelDisplay().getModelAnimManager()->Pause();
	}
}

void CDlgModelAnim::OnPrev()
{
	if (getModelDisplay().getModelAnimManager())
	{
		getModelDisplay().getModelAnimManager()->PrevFrame();
	}
}

void CDlgModelAnim::OnNext()
{
	if (getModelDisplay().getModelAnimManager())
	{
		getModelDisplay().getModelAnimManager()->NextFrame();
	}
}

void CDlgModelAnim::OnAnimChanged()
{
	if (getModelDisplay().getModelAnimManager())
	{
		getModelDisplay().getModelObject()->SetAnim(m_ComboBoxAnim.getListBox().GetSelectedIndex());
		m_SliderFrame.SetRange(getModelDisplay().getModelAnimManager()->getCurrentAnim().timeStart, getModelDisplay().getModelAnimManager()->getCurrentAnim().timeEnd);
	}
}

void CDlgModelAnim::OnSpeedChanged()
{
	float fSpeed = m_SliderSpeed.GetValue()*0.1f;
	if (getModelDisplay().getModelAnimManager())
	{
		getModelDisplay().getModelAnimManager()->SetSpeed(fSpeed);
	}
	std::wstring wstr = FormatW(L"%.1fx", fSpeed);
	m_StaticSpeed.SetText(wstr);
}

void CDlgModelAnim::OnFrameChanged()
{
	if (getModelDisplay().getModelAnimManager())
	{
		int nFrame = m_SliderFrame.GetValue();
		getModelDisplay().getModelAnimManager()->SetFrame(nFrame);
	}
}