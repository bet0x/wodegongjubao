#include "DlgModelController.h"
#include "../Config/Config.h"
/*#include "mpq/mpq_libmpq.h"*/
/*#include "database.h"*/
#include "CsvFile.h"
#include "IORead.h"
#include "DlgModelMain.h"

CDlgModelController::CDlgModelController()
{
}

CDlgModelController::~CDlgModelController()
{	
}

void CDlgModelController::OnControlRegister()
{
	CUIDialog::OnControlRegister();
	RegisterControl("IDC_LISTBOX_FILE",		m_ListBoxFolder);


	RegisterControl("IDC_TAB_SHOW",	m_RadioButtonShow);
	RegisterControl("IDC_TAB_ANIM",	m_RadioButtonAnim);
	RegisterControl("IDC_TAB_MATERIAL",	m_RadioButtonMaterial);
	RegisterControl("IDC_TAB_PARTICLE",	m_RadioButtonParticle);
	RegisterControl("IDC_TAB_ABOUT",m_RadioButtonAbout);

	RegisterControl("IDC_TAB_OBJECT_FILE",		m_RadioButtonFile);
	RegisterControl("IDC_TAB_OBJECT_PLAYER",	m_RadioButtonPlayer);


	m_DlgModelShow.Create("IDD_MODEL_SHOW", this);
	m_DlgModelAnim.Create("IDD_MODEL_ANIM", this);
	m_DlgModelMaterial.Create("IDD_MODEL_MATERIAL", this);
	m_DlgModelParticle.Create("IDD_MODEL_ABOUT", this);
	m_DlgModelAbout.Create("IDD_MODEL_ABOUT", this);

	m_DlgModelPlayer.Create("IDD_MODEL_PLAYER", this);

	RegisterControlEvent("IDC_LISTBOX_FILE",	(PEVENT)&CDlgModelController::OnListBoxFolderItemDblClk, EVENT_LISTBOX_ITEM_DBLCLK);
	RegisterControlEvent("IDC_TAB_SHOW",		(PEVENT)&CDlgModelController::OnTabChanged);
	RegisterControlEvent("IDC_TAB_ANIM",		(PEVENT)&CDlgModelController::OnTabChanged);
	RegisterControlEvent("IDC_TAB_MATERIAL",	(PEVENT)&CDlgModelController::OnTabChanged);
	RegisterControlEvent("IDC_TAB_PARTICLE",	(PEVENT)&CDlgModelController::OnTabChanged);
	RegisterControlEvent("IDC_TAB_ABOUT",		(PEVENT)&CDlgModelController::OnTabChanged);

	RegisterControlEvent("IDC_TAB_OBJECT_FILE",		(PEVENT)&CDlgModelController::OnTabObjectChanged);
	RegisterControlEvent("IDC_TAB_OBJECT_PLAYER",	(PEVENT)&CDlgModelController::OnTabObjectChanged);

	RegisterControlEvent("IDC_BTN_CLOSE",		(PEVENT)&CDlgModelController::OnClose);
}

bool CDlgModelController::OnInitDialog()
{
	//SetButtonGroup();

	OnTabChanged();
	OnTabObjectChanged();
	initRecentPath();
	//MPQModel();
	//
	int   argc=0;
	LPWSTR   *argv=::CommandLineToArgvW(::GetCommandLineW(),&argc);
	if (argc>1&&argv)
	{
		OpenFile(argv[1]);
	}
	LocalFree(argv); 

	return true;
}

void CDlgModelController::C3Model()
{
	m_wstrPath=L"";
	m_wstrFileType=L"";
	m_ListBoxFolder.RemoveAllItems();
	// miku
	m_ListBoxFolder.AddItem( L"D:/Miku/miku.sm");
	//
	CCsvFile csvFile;
	if (csvFile.Open("csv/3DObj.csv"))
	{
		while (csvFile.SeekNextLine())
		{
			m_ListBoxFolder.AddItem(s2ws(csvFile.GetStr("Name")));
		}
		csvFile.Close();
	}
}

void CDlgModelController::MPQModel()
{
/*	m_wstrPath=L"";
	m_wstrFileType=L"";
	m_ListBoxFolder.RemoveAllItems();
	// 载入游戏MPQ文件到内存
	SetGamePath(GetModelConfig().m_strWOWPath);
	InitMPQArchives();
	std::set<FileTreeItem> filelist;
	getFileLists(filelist, filterModels);

	// 把所有可视文件放到树里面
	for (std::set<FileTreeItem>::iterator it = filelist.begin(); it != filelist.end(); ++it)
	{
		WCHAR wszBuf[256]={0};
		const std::string& str = (*it).fn;
		MultiByteToWideChar(CP_UTF8,0,str.c_str(),-1,wszBuf,256);
		m_ListBoxFolder.AddItem(wszBuf);
	}

	// clear filelist
	filelist.clear();

	//if (!
	GetAnimDB().open();//)
	//wxLogMessage(_T("Error: Could not open the Animation DB."));
	//if (!
	GetCreatureModelDB().open();//)
	//wxLogMessage(_T("Error: Could not open the Creatures DB."));
	//if (!
	GetCreatureSkinDB().open();//)
	//wxLogMessage(_T("Error: Could not open the CreatureDisplayInfo DB."));*/
}

void CDlgModelController::initRecentPath()
{
	// read the recent path from reg.
	std::wstring wstrRecentPath;
	{
		HKEY hKey;
		if (ERROR_SUCCESS==RegOpenKeyExW(HKEY_LOCAL_MACHINE,L"software\\rpgsky\\modelview\\",
			0, KEY_READ, &hKey))
		{
			DWORD dwType = REG_SZ;
			wchar_t wszFilename[256];
			DWORD dwSize = sizeof(wszFilename);

			if (ERROR_SUCCESS==RegQueryValueExW(hKey, L"recentpath",
				NULL, &dwType, (PBYTE)&wszFilename, &dwSize))
			{
				wstrRecentPath = wszFilename;
			}
			RegCloseKey(hKey);
		}
	}
	if (wstrRecentPath.length()>0)
	{
		OpenPath(wstrRecentPath,s2ws(GetModelMgr().getAllExtensions()));
	}
	else
	{
		OpenPath(getCurrentDirectory(),s2ws(GetModelMgr().getAllExtensions()));
	}
}

void CDlgModelController::OpenFile(const std::wstring& wstrFilename)
{
	if (IOReadBase::Exists(ws2s(wstrFilename)))
	{
		OpenPath(GetParentPath(wstrFilename),s2ws(GetModelMgr().getAllExtensions()));
		//m_ListBoxFolder.SetSelec()
		//GetFilename(wstrFilename)
		getModelDisplay().LoadModel(ws2s(wstrFilename));
		OnUpdate();
	}
}

void CDlgModelController::OpenPath(const std::wstring& wstrPath, const std::wstring& wstrFileType)
{
	m_wstrPath=wstrPath;
	m_wstrFileType=wstrFileType;
	m_ListBoxFolder.RemoveAllItems();
	//|{"+wstrFileType+L"}";
	CDir dir;
	dir.ReadDir(wstrPath);
	for (int i=0; i<dir.m_FileInfo.size(); i++)
	{
		if (dir.m_FileInfo[i].IsDirectory())
		{
			m_ListBoxFolder.AddItem(L'\\'+dir.m_FileInfo[i].wstrFilename);
		}
	}
	for (int i=0; i<dir.m_FileInfo.size(); i++)
	{
		if (!dir.m_FileInfo[i].IsDirectory())
		{
			if (wstrFileType.find(GetExtension(dir.m_FileInfo[i].wstrFilename))!=std::wstring::npos)
			{
				m_ListBoxFolder.AddItem(dir.m_FileInfo[i].wstrFilename);
			}
		}

	}
}

void CDlgModelController::OnListBoxFolderItemDblClk()
{
	std::wstring wstrFolder = m_ListBoxFolder.GetSelectedItem()->wstrText;
	if (wstrFolder.length()>0&&wstrFolder[0]==L'\\')
	{
		if (L"\\.."==wstrFolder)
		{
			OpenPath(GetParentPath(m_wstrPath),m_wstrFileType);
		}
		else
		{
			OpenPath(m_wstrPath+wstrFolder.substr(1)+L'\\',m_wstrFileType);
		}
	}
	else
	{
		// write the recent path to reg.
		{
			HKEY hKey;
			if (ERROR_SUCCESS==RegCreateKeyExW(HKEY_LOCAL_MACHINE,L"software\\rpgsky\\modelview\\",
				NULL,NULL,REG_OPTION_NON_VOLATILE,KEY_WRITE,NULL,&hKey,NULL))
			{
				RegSetValueExW(hKey,L"recentpath",0,REG_SZ,(LPBYTE)m_wstrPath.c_str(),sizeof(wchar_t)*m_wstrPath.length());
				RegCloseKey(hKey);
			}
		}
		std::string strFilename = ws2s(m_wstrPath+m_ListBoxFolder.GetSelectedItem()->wstrText);
		getModelDisplay().LoadModel( strFilename );
		OnUpdate();
	}
}

void CDlgModelController::OnUpdate()
{
	m_DlgModelAnim.ResetAnim();
	m_DlgModelMaterial.onReset();
}

void CDlgModelController::OnTabChanged()
{
	m_DlgModelShow.SetVisible(m_RadioButtonShow.IsChecked());
	m_DlgModelAnim.SetVisible(m_RadioButtonAnim.IsChecked());
	m_DlgModelMaterial.SetVisible(m_RadioButtonMaterial.IsChecked());
	m_DlgModelParticle.SetVisible(m_RadioButtonParticle.IsChecked());
	m_DlgModelAbout.SetVisible(m_RadioButtonAbout.IsChecked());
	if (m_DlgModelMaterial.IsVisible()||m_DlgModelParticle.IsVisible())
	{
		m_ListBoxFolder.SetVisible(false);
		m_DlgModelPlayer.SetVisible(false);
		m_RadioButtonFile.SetVisible(false);
		m_RadioButtonPlayer.SetVisible(false);
	}
	else
	{
		if (!m_RadioButtonFile.IsVisible())
		{
			m_ListBoxFolder.SetVisible(true);
			m_RadioButtonFile.SetVisible(true);
			m_RadioButtonPlayer.SetVisible(true);
		}
	}
}

void CDlgModelController::OnTabObjectChanged()
{
	m_ListBoxFolder.SetVisible(m_RadioButtonFile.IsChecked());
	m_DlgModelPlayer.SetVisible(m_RadioButtonPlayer.IsChecked());
}

void CDlgModelController::OnClose()
{
	((CDlgModelMain*)GetParentDialog())->CloseController();
}

CDlgModelMain& CDlgModelController::getDlgModelMain()
{
	assert(GetParentDialog());
	return *((CDlgModelMain*)GetParentDialog());
}