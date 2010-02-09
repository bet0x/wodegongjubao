#pragma once
#include "DlgToolBar.h"
#include "DlgFile.h"
#include "DlgFPS.h"
#include "UIWorldEditorDisplay.h"
#include "UIMainDialog.h"
#include "DlgRegister.h"

typedef struct{
	CScenePlugBase * pObj;
	HINSTANCE hIns;
}PLUG_ST, * LPPLUG_ST;

class CDlgWorldEditor : public CUIMainDialog
{
public:
	CDlgWorldEditor();
	~CDlgWorldEditor();
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
public:
	CUIWorldEditorDisplay& getDisplay();
protected:
	bool loadPlugFromPath(const std::string& strPath);
	bool createPlug(const std::string& strFilename);
private:
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