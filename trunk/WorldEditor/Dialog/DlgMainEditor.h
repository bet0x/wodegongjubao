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

	void CloseController();
	void OnBtnShowController();
	void OnBtnFaceDetect();

	void OnRadioModel();
	void OnRadioWorld();
public:
	CUIWorldEditorDisplay& getDisplay();
	CModelDisplay& getModelDisplay();
	CDlgModelMaterial& getMaterialDialog();
private:
	void updateDisplay();
	CDlgController	m_DlgController;
	CUIButton		m_BtnHide;
	CUIButton		m_BtnShow;
	CModelDisplay	m_ModelDisplay;
	//CDlgFaceDetect	m_DlgFaceDetect;

	CUIStatic	m_StaticPosX;
	CUIStatic	m_StaticPosY;
	CUIStatic	m_StaticPosHeight;
	CUIStatic	m_StaticInfo;
	CUIWorldEditorDisplay	m_WorldEditorDisplay;
	CDlgToolbar		m_DlgToolbar;
	CDlgFile		m_DlgFile;
	//CDlgFPS			m_DlgFPS;
	CDlgRegister	m_DlgRegister;

	CDataPlugsMgr	m_DataPlugsMgr;


	CDlgModelMaterial m_DlgMaterialEdit;
};