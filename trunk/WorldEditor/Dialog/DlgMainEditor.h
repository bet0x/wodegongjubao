#pragma once
#include "UIMainDialog.h"
#include "DlgToolBar.h"
#include "DlgFile.h"
#include "DlgFPS.h"
#include "UIWorldEditorDisplay.h"

#include "DlgRegister.h"
#include "DlgModelController.h"
#include "DlgModelMaterial.h"
#include "ModelDisplay.h"

#include "DataPlugsMgr.h"
//#include "DlgFaceDetect.h"
#include "DlgController.h"
#include "DlgModelController.h"

class CDlgMainEditor : public CUIMainDialog
{
public:
	CDlgMainEditor();
	~CDlgMainEditor();
public:
	virtual bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual void OnControlRegister();
	virtual bool OnInitDialog();
	virtual void OnFrameMove(double fTime, float fElapsedTime);

	void OnBtnNewFile();
	void OnBtnOpenFile();
	void OnBtnSaveFile();
	void OnBtnToolbar();
	void OnBtnTerrainResize();

	void OnFileNew();
	void OnFileOpen();
	void OnFileSave();
	void OnFileCancel();

	void OnBtnShowController();
	void OnBtnFaceDetect();

	void OnRadioModel();
	void OnRadioWorld();
public:
	CUIWorldEditorDisplay& getDisplay();
	CModelDisplay& getModelDisplay();
	CDlgModelMaterial& getMaterialDialog();
	CDlgModelController& getDlgModelController(){return m_DlgModelController;}
private:
	void updateDisplay();

	CUIButton		m_BtnShow;
	//CDlgFaceDetect	m_DlgFaceDetect;

	CUIStatic	m_StaticPosX;
	CUIStatic	m_StaticPosY;
	CUIStatic	m_StaticPosHeight;
	CUIStatic	m_StaticInfo;
	CModelDisplay			m_ModelDisplay;
	CUIWorldEditorDisplay	m_WorldEditorDisplay;
	CDlgController	m_DlgController;
	CDlgModelController	m_DlgModelController;
	CDlgToolbar		m_DlgToolbar;
	CDlgFile		m_DlgFile;
	//CDlgFPS			m_DlgFPS;
	CDlgRegister	m_DlgRegister;

	CDataPlugsMgr	m_DataPlugsMgr;


	CDlgModelMaterial m_DlgMaterialEdit;
};