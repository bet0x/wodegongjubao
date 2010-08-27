#pragma once
#include "UIDialog.h"

class CDlgModelRenderPass :public CUIDialog
{
public:
	CDlgModelRenderPass();
	virtual void OnControlRegister();
	virtual void SetVisible(bool bVisible);
	void onReset();
private:
	std::string getSelectedRenderPass();
	// Controls' event.
	void OnListboxRenderPass();
	void OnCmbSubIDSelected();
	void OnEditBoxMaterialChanged();
	void OnBtnAdd();
	void OnBtnDelete();
private:
	// Controls' variable.
	CUIListBox	m_ListboxRenderPass;
	CUIComboBox	m_CmbSubID;
	CUIEditBox	m_EditBoxMaterial;
};