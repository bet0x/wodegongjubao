#pragma once
#include "Root.h"
#include "Dialog/DlgMainEditor.h"
#include "TSingleton.h"

// Root
class CMainRoot : public CRoot, public TSingleton<CMainRoot>
{
public:
	CMainRoot();
	~CMainRoot();
public:
	CDlgMainEditor& getMainDialog();
	void Run();
	void OnFrameMove( double fTime, float fElapsedTime );
	void OnFrameRender( double fTime, float fElapsedTime );

	bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
protected:
	CDlgMainEditor	m_dlgMain;
};