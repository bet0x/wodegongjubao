#include "MainRoot.h"
#include "RenderSystem.h"
#include "TextRender.h"
#include "IniFile.h"
#include "RegData.h"
#include "Audio.h"

CMainRoot::CMainRoot()
{
#ifdef _DEBUG
	if (LoadRenderSystemLibrary("Plugins\\RenderSystemDX9d.dll")==false){return;}
#else
	if (LoadRenderSystemLibrary("Plugins\\RenderSystemDX9.dll")==false){return;}
#endif

	// UI Theme
	std::string strTheme = IniGetStr("WorldEditorUI.cfg","UIConfig","theme");
	std::string strThemeFilename = "Data\\Themes\\"+strTheme+"\\UIStyle.xml";
	GetStyleMgr().Create(strThemeFilename.c_str());

	// UI Font
	std::string strLanguage = IniGetStr("WorldEditorUI.cfg","UIConfig","language");
	std::string strFont1 = IniGetStr("Font.cfg",strLanguage.c_str(),"font1");
	UIGraph::getInstance().InitFont(s2ws(strFont1),12);

	// Create UI
	std::string strUIFilename = IniGetStr("WorldEditorUI.cfg","UIConfig","file");
	m_dlgMain.create(strUIFilename.c_str(),"IDD_MAIN");

	// Load UI Language
	setUILanguage(strLanguage);

	// Create Render System Window
	int nWidth = IniGetInt("WorldEditor.cfg","display","width",800);
	int nHeight = IniGetInt("WorldEditor.cfg","display","height",500);
	CreateRenderWindow(L"RPGSkyEditor V0.7.1 alpha", nWidth, nHeight);

	// Create Common Shared Shader
	if(!GetRenderSystem()->GetShaderMgr().createSharedShader("Data/fx/shared.fx"))
	{
		MessageBoxW(NULL,L"Can't find the shared fx",L"Error",0);
	}

	// Common Materials
	m_pRenderSystem->getMaterialMgr().loadMaterial("WorldEditorRes\\Common.mat.csv");

	// Write into reg.
	wchar_t szFilename[1024];
	ZeroMemory(szFilename,1024);
	GetModuleFileNameW(NULL,szFilename,1024);
	SetRegStr(L"software\\rpgsky\\modelview\\",L"filename",szFilename);
}

CMainRoot::~CMainRoot()
{
}

CDlgMainEditor& CMainRoot::getMainDialog()
{
	return m_dlgMain;
}

#include "FileSystem.h"
void CMainRoot::setUILanguage(const std::string& strLanguage)
{
	//IniSetStr("WorldEditorUI.cfg","UIConfig","language",strUIFilename);
	std::string strUIFilename = IniGetStr("WorldEditorUI.cfg","UIConfig","file");
	std::string strStringFilename = ChangeExtension(strUIFilename,"String"+strLanguage+".ini");
	m_dlgMain.loadString(strStringFilename.c_str());
}

void CMainRoot::OnFrameMove(double fTime, float fElapsedTime)
{
	CRoot::OnFrameMove(fTime, fElapsedTime);
	m_dlgMain.OnFrameMove(fTime, fElapsedTime);
	GetAudio().FrameMove(fElapsedTime);
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
				Matrix::newScale(Vec3D(fZ/mProjection._11/rcViewport.right*2.0f,-fZ/mProjection._22/rcViewport.bottom*2.0f,-fZ/mProjection._33/rcViewport.bottom*2.0f));

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
	bool bNoFurtherProcessing = m_dlgMain.MsgProc(hWnd, uMsg, wParam, lParam);
	return bNoFurtherProcessing;
}