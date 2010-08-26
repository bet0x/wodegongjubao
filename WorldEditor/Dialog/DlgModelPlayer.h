#pragma once
#include "UIDialog.h"

class CDlgModelPlayer
{
public:
	virtual void OnControlRegister();
	virtual bool OnInitDialog();
	void OnUpdatePlayer();
	void OnSelectSet();
protected:
private:
	CUIComboBox m_ComboBoxSkeleton;
	CUIComboBox m_ComboBoxHead;
	CUIComboBox m_ComboBoxSet;
	CUIComboBox m_ComboBoxHelm;
	CUIComboBox m_ComboBoxArmor;
	CUIComboBox m_ComboBoxGlove;
	CUIComboBox m_ComboBoxPants;
	CUIComboBox m_ComboBoxBoot;
	CUIComboBox m_ComboBoxLeftHand;
	CUIComboBox m_ComboBoxRightHand;
	CUIComboBox m_ComboBoxBack;
};