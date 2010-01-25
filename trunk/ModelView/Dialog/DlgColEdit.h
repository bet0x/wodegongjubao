#pragma once

#include "UIMainDialog.h"
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
#include "../FrameRectMgr.h"

class CModelDisplay;


class CDlgColEditDialog : public CUIMainDialog
{
public:
	virtual void OnControlRegister();
	virtual bool OnInitDialog();
	
	void OnClearColRect();
	void OnUpdateColRect();
	void OnCamera();
	void OnSave();
	
	int GetCurSelectRectType();
	int GetColID();

protected:
private:
	CUIButton		m_BtnClearColRect;
	CUIButton		m_BtnUpdateColRect;
	CUIButton       m_BtnCamera;
	CUIButton       m_BtnSave;
	CUIEditBox      m_EditColID;
	
	CUIComboBox     m_ColTypeCombo;

	CFrameRectMgr  m_FrameRectMgr;
};