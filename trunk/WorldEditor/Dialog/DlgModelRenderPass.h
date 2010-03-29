#pragma once
#include "DlgBaseEditor.h"

class CDlgModelRenderPass: public CDlgBaseEditor
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
	// Controls' variable.
	CUIListBox	m_ListboxRenderPass;
	CUIComboBox	m_CmbSubID;
	CUIEditBox	m_EditBoxMaterial;
};