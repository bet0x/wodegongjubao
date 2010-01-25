#pragma once
#include "DlgModelAnim.h"

class CDlgModelShow: public CDlgModelBaseDialog
{
public:
	virtual void OnControlRegister();
protected:
private:
	void OnUpdateShow();
	CUICheckBox m_CheckBoxBones;
	CUICheckBox	m_CheckBoxBounds;
	CUICheckBox m_CheckBoxModel;
	CUICheckBox m_CheckBoxWireframe;
	CUICheckBox m_CheckBoxMaterial;
	CUICheckBox	m_CheckBoxParticles;
};