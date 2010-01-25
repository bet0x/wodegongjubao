#pragma once
#include "Root.h"
#include "Dialog/DlgModelMain.h"
#include "../../shared/Math/include/RECT.h"

// œ‘ æ¿‡
class CMainRoot : public CRoot
{
public:
	CMainRoot();
	~CMainRoot();
public:
	void Run();
	void OnFrameMove( double fTime, float fElapsedTime );
	void OnFrameRender( double fTime, float fElapsedTime );

	bool MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
	CDlgModelMain		m_dlgMain;
	



	
	//CDlgModelController m_dlgModelController;
};