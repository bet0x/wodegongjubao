#pragma once
#include "UIMainDialog.h"
#include "DlgToolBar.h"
#include "DlgFile.h"
#include "DlgFPS.h"
#include "UIWorldEditorDisplay.h"

#include "DlgRegister.h"
#include "DlgModelController.h"
#include "ModelDisplay.h"

//#include "DlgFaceDetect.h"

typedef struct{
	CScenePlugBase * pObj;
	HINSTANCE hIns;
}PLUG_ST, * LPPLUG_ST;

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
public:
	CUIWorldEditorDisplay& getDisplay();
	CModelDisplay& getModelDisplay();
protected:
	bool loadPlugFromPath(const std::string& strPath);
	bool createPlug(const std::string& strFilename);
private:
	CDlgModelController m_DlgModelController;
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
	std::wstring	m_wstrFileType;
	//CDlgFPS			m_DlgFPS;
	CDlgRegister	m_DlgRegister;

	std::vector<PLUG_ST> m_arrPlugObj;
};