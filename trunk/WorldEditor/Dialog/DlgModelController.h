#pragma once
#include "DlgModelAnim.h"
#include "DlgModelShow.h"
#include "DlgModelPlayer.h"
#include "DlgModelRenderPass.h"

class CDlgModelController: public CDlgBaseEditor
{
public:
	CDlgModelController();
	~CDlgModelController();
public:
	virtual void OnControlRegister();
	virtual bool OnInitDialog();
	void OnUpdate();
protected:
	void OnTabChanged();
	void OnClose();
private:
	CUIRadioButton m_RadioButtonShow;
	CUIRadioButton m_RadioButtonAnim;
	CUIRadioButton m_RadioButtonRenderPass;
	CUIRadioButton m_RadioButtonParticle;
	CDlgModelShow		m_DlgModelShow;
	CDlgModelAnim		m_DlgModelAnim;
	CDlgModelRenderPass	m_DlgModelRenderPass;
	CUIDialog			m_DlgModelParticle;
};