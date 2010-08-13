#include "MainRoot.h"
#include "RenderSystem.h"
#include "TextRender.h"
#include "IniFile.h"
#include "RegData.h"

typedef CRenderSystem* (WINAPI * PFN_RENDER_SYSTEM_PLUG_CREATE_SYSTEM)();

CMainRoot::CMainRoot()
{
	{
#ifdef _DEBUG
		HMODULE hIns = LoadLibrary("RenderSystemDX9d.dll");
#else
		HMODULE hIns = LoadLibrary("RenderSystemDX9.dll");
#endif
		if (hIns)
		{
			PFN_RENDER_SYSTEM_PLUG_CREATE_SYSTEM pFunc = (PFN_RENDER_SYSTEM_PLUG_CREATE_SYSTEM)GetProcAddress(hIns, "RENDER_SYSTEM_PLUG_CREATE_SYSTEM");
			if (NULL==pFunc)
			{
				MessageBox(NULL,"Create render system failed!","Error",0);
				return;
			}
			m_pRenderSystem = pFunc();
			SetRenderSystem(m_pRenderSystem);
		}
		else
		{
			MessageBox(NULL,"Load \"RenderSystemDX9.dll\" failed!","Error",0);
			return;
		}
	}

	//
	{
		// UI Theme
		std::string strTheme = IniGetStr("WorldEditorUI.cfg","UIConfig","theme");
		std::string strThemeFilename = "Data\\Themes\\"+strTheme+"\\UIStyle.xml";
		GetStyleMgr().Create(strThemeFilename);

		// UI Font
		std::string strLanguage = IniGetStr("WorldEditorUI.cfg","UIConfig","language");
		std::string strFont1 = IniGetStr("Font.cfg",strLanguage.c_str(),"font1");
		UIGraph::getInstance().InitFont(s2ws(strFont1),12);
	}

	// Write into reg.
	{
		wchar_t szFilename[1024];
		ZeroMemory(szFilename,1024);
		GetModuleFileNameW(NULL,szFilename,1024);
		SetRegStr(L"software\\rpgsky\\modelview\\",L"filename",szFilename);
	}

	// Common Materials
	m_pRenderSystem->getMaterialMgr().loadMaterial("WorldEditorRes\\Common.mat.csv");
}

CMainRoot::~CMainRoot()
{
}

CDlgMainEditor& CMainRoot::getMainDialog()
{
	return m_dlgMain;
}

void CMainRoot::Run()
{
	// Create Render System Window
	int nWidth = IniGetInt("WorldEditor.cfg","display","width",800);
	int nHeight = IniGetInt("WorldEditor.cfg","display","height",500);
	CreateRenderWindow(L"RPGSkyEditor V0.7.1 alpha", nWidth, nHeight);
	if(!GetRenderSystem()->GetShaderMgr().createSharedShader("Data/fx/shared.fx"))
	{
		MessageBoxW(NULL,L"Can't find the shared fx",L"Error",0);
	}
	// Create UI
	std::string strUIFilename = IniGetStr("WorldEditorUI.cfg","UIConfig","file");
	m_dlgMain.create(strUIFilename,"IDD_MAIN");
	// Resize UI
	RECT rc={0,0,nWidth, nHeight};
	m_dlgMain.OnSize(rc);
	// Load UI Language
	std::string strLanguage = IniGetStr("WorldEditorUI.cfg","UIConfig","language");
	setUILanguage(strLanguage);
	// Root Run
	CRoot::Run();
}

#include "FileSystem.h"
void CMainRoot::setUILanguage(const std::string& strLanguage)
{
	//IniSetStr("WorldEditorUI.cfg","UIConfig","language",strUIFilename);
	std::string strUIFilename = IniGetStr("WorldEditorUI.cfg","UIConfig","file");
	std::string strStringFilename = ChangeExtension(strUIFilename,"String"+strLanguage+".ini");
	m_dlgMain.loadString(strStringFilename);
}

void CMainRoot::OnFrameMove(double fTime, float fElapsedTime)
{
	CRoot::OnFrameMove(fTime, fElapsedTime);
	m_dlgMain.OnFrameMove(fTime, fElapsedTime);
}

void CMainRoot::OnFrameRender(double fTime, float fElapsedTime)
{
	if(m_pRenderSystem->BeginFrame())
	{
		m_pRenderSystem->SetupRenderState();
		m_pRenderSystem->ClearBuffer(true,true,0x0);
		{
			CRect<int> rcViewport;
			m_pRenderSystem->getViewport(rcViewport);
			Matrix mProjection;
			//R.getProjectionMatrix(mProjection);
			mProjection.MatrixPerspectiveFovLH(PI/4,(float)rcViewport.right/(float)rcViewport.bottom,0.1f,100);
			m_pRenderSystem->setProjectionMatrix(mProjection);

			float fZ = 50;
			Matrix mWorld=Matrix::newTranslation(Vec3D(-fZ/mProjection._11,fZ/mProjection._22,fZ))*
				Matrix::newScale(Vec3D(fZ/mProjection._11/rcViewport.right*2.0f,-fZ/mProjection._22/rcViewport.bottom*2.0f,1.0f));

//  			{
//  				Matrix mRotate;
//  				mRotate.rotate(Vec3D(0,sin(GetGlobalTimer().GetTime()*2)*0.1f,0));
//  				mWorld=mRotate*mWorld;
//  			}
			m_dlgMain.OnFrameRender(mWorld, fTime, fElapsedTime);
		}
		m_pRenderSystem->EndFrame();
	}
	CRoot::OnFrameRender(fTime, fElapsedTime);
}

bool CMainRoot::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Mouse messages
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
	case WM_MOUSEWHEEL:
		{
			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
			switch(uMsg)
			{
			case WM_LBUTTONDBLCLK:
				m_dlgMain.OnLButtonDblClk(mousePoint);
				break;
			case WM_LBUTTONDOWN:
				m_dlgMain.OnLButtonDown(mousePoint);
				break;
			case WM_LBUTTONUP:
				m_dlgMain.OnLButtonUp(mousePoint);
				break;
			case WM_RBUTTONDBLCLK:
				m_dlgMain.OnRButtonDblClk(mousePoint);
				break;
			case WM_RBUTTONDOWN:
				m_dlgMain.OnRButtonDown(mousePoint);
				break;
			case WM_RBUTTONUP:
				m_dlgMain.OnRButtonUp(mousePoint);
				break;
			case WM_MBUTTONDBLCLK:
				m_dlgMain.OnMButtonDblClk(mousePoint);
				break;
			case WM_MBUTTONDOWN:
				m_dlgMain.OnMButtonDown(mousePoint);
				break;
			case WM_MBUTTONUP:
				m_dlgMain.OnMButtonUp(mousePoint);
				break;
			case WM_MOUSEMOVE:
				m_dlgMain.OnMouseMove(mousePoint);
				break;
			case WM_MOUSEWHEEL:
				//GetCursorPos(&mousePoint);
				ScreenToClient(UIGetHWND(),&mousePoint);
				m_dlgMain.OnMouseWheel(mousePoint,(short) HIWORD(wParam));
				break;
			}
		}
		break;
	}

	bool bNoFurtherProcessing = m_dlgMain.MsgProc(hWnd, uMsg, wParam, lParam);
	return false;
}