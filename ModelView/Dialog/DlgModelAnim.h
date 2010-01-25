#pragma once
//#include "Manager.h"
#include "UIDialog.h"
#include "UIControl.h"

#include "UIStatic.h"
#include "UIButton.h"
#include "UICheckBox.h"
#include "UIComboBox.h"
#include "UIRadioButton.h"
#include "UIImage.h"
#include "UIListBox.h"
#include "UIScrollBar.h"
#include "UISlider.h"
#include "UIEditBox.h"
#include "UIIMEEditBox.h"

class CModelDisplay;
class CDlgModelMain;
class CDlgModelBaseDialog: public CUIDialog
{
public:
	CDlgModelBaseDialog();
	~CDlgModelBaseDialog();
	virtual CDlgModelMain& getDlgModelMain();
	CModelDisplay& getModelDisplay();
protected:
};

class CDlgModelAnim: public CDlgModelBaseDialog
{
public:
	virtual void OnControlRegister();
	virtual bool OnInitDialog();
	virtual void OnFrameMove(double fTime, float fElapsedTime);
	void ResetAnim();
	void OnAnimChanged();
	void OnSpeedChanged();
	void OnFrameChanged();
protected:
private:
	void OnPlay();
	void OnPrev();
	void OnNext();

	CUIComboBox m_ComboBoxAnim;
	CUIStatic	m_StaticSpeed;
	CUISlider	m_SliderSpeed;
	CUIStatic	m_StaticFrame;
	CUISlider	m_SliderFrame;
	CUIButton	m_BtnPlay;
	CUIButton	m_BtnPrev;
	CUIButton	m_BtnNext;
};