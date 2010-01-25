#pragma once
#include "DlgModelController.h"
#include "ModelDisplay.h"
#include "UIMainDialog.h"
#include "DlgColEdit.h"
//#include "DlgFaceDetect.h"

class CDlgModelMain: public CUIMainDialog
{
public:
	CDlgModelMain();
	~CDlgModelMain();
public:
	virtual void OnControlRegister();
	virtual bool OnInitDialog();
	void CloseController();
	void OnBtnShowController();
	void OnBtnFaceDetect();
	CModelDisplay& getModelDisplay();
	CDlgColEditDialog &getColEditDialog(){return m_dlgColEdit;}
protected:
private:
	CDlgModelController m_DlgModelController;
	CUIButton		m_BtnHide;
	CUIButton		m_BtnShow;
	CModelDisplay	m_ModelDisplay;
	CDlgColEditDialog   m_dlgColEdit;
	//CDlgFaceDetect	m_DlgFaceDetect;
};