#pragma once
#include "DlgModelMaterial.h"

class CDlgModelRenderPass: public CDlgBaseEditor
{
public:
	CDlgModelRenderPass();
	virtual void OnControlRegister();
	virtual void SetVisible(bool bVisible);
	void onReset();
private:
	CMaterial* getSelectedRenderPass();
	// Controls' event.
	void OnListboxRenderPass();
	// Controls' variable.
	CUIListBox	m_ListboxRenderPass;
	CDlgModelMaterial m_DlgModelMaterial;
};