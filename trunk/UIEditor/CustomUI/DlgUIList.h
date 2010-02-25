#pragma once
#include "Manager.h"
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
class CDlgUIList : public CUIDialog
{
public:
	CDlgUIList();
	~CDlgUIList();
public:
	virtual void OnControlRegister();
	bool InitDialogListFromXML(const std::string& strFilename);
	bool createDialogCodeFromXML(const std::string& strFilename,const std::string& strDialogName);
	void SetDlgView(CUIDialog* pDlgView){m_pDlgView = pDlgView;}
protected:
	void OnDlgListBoxSelection();
	void OnDlgListBoxDblClk();
private:
	CUIDialog*	m_pDlgView;
	CUIListBox m_ListBox;
	int nButtonDlgUIList;
	int nButtonExit;
	int nEditBoxUser;
	int nEditBoxPassword;
};