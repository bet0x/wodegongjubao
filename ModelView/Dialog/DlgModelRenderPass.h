#pragma once
#include "DlgModelAnim.h"
#include "Material.h"
#include "DlgModelMaterial.h"

class CDlgModelRenderPass: public CDlgModelBaseDialog
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