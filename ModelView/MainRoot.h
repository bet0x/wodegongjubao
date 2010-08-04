#pragma once
#include "Root.h"
#include "Dialog/DlgModelMain.h"
#include "RECT.h"
#include "TSingleton.h"

// œ‘ æ¿‡
class CMainRoot : public CRoot, public TSingleton<CMainRoot>
{
public:
	CMainRoot();
	~CMainRoot();
public:
	CDlgModelMain& getMainDialog();
	void Run();
	void OnFrameMove(double fTime, float fElapsedTime);
	void OnFrameRender(double fTime, float fElapsedTime);

	bool MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
	CDlgModelMain		m_dlgMain;
	



	
	//CDlgModelController m_dlgModelController;
};