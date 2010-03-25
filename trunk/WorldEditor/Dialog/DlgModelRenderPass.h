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
	// Controls' variable.
	CUIListBox	m_ListboxRenderPass;
};