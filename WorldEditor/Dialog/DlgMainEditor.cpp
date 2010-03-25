#include "DlgMainEditor.h"
#include "LumpFile.h"
#include "FileSystem.h"
#include "Camera.h"
#include "..\Config\Config.h"

CDlgMainEditor::CDlgMainEditor()
{
}

CDlgMainEditor::~CDlgMainEditor()
{	
}

CUIWorldEditorDisplay& CDlgMainEditor::getDisplay()
{
	return m_WorldEditorDisplay;
}

CModelDisplay&  CDlgMainEditor::getModelDisplay()
{
	return m_ModelDisplay;
}

bool CDlgMainEditor::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//.//
	//m_DlgFPS.SetViewDir(m_WorldEditorDisplay.GetCamera().GetViewDir());
	//
	bool bNoFurtherProcessing = CUIMainDialog::MsgProc(hWnd, uMsg, wParam, lParam);
	if ( bNoFurtherProcessing )
	{
		return true;
	}
	return false;
}

void CDlgMainEditor::OnControlRegister()
{
	CUIMainDialog::OnControlRegister();

	m_DlgToolbar.Create("IDD_TOOLBAR", this);
	m_DlgFile.Create("IDD_FILE", this);
	m_DlgRegister.Create("IDD_REGISTER", this);
	//m_DlgFPS.Create("IDD_FPS", this);

	RegisterControl( "IDC_STATIC_POS_X", m_StaticPosX);
	RegisterControl( "IDC_STATIC_POS_Y", m_StaticPosY);
	RegisterControl( "IDC_STATIC_POS_HEIGHT", m_StaticPosHeight);

	RegisterControl( "IDC_STATIC_INFO", m_StaticInfo);
	RegisterControl( "IDC_DISPLAY_WORLDEDITOR", m_WorldEditorDisplay);

	RegisterControlEvent("IDC_BTN_NEW_FILE",	(PEVENT)&CDlgMainEditor::OnBtnNewFile);
	RegisterControlEvent("IDC_BTN_OPEN_FILE",	(PEVENT)&CDlgMainEditor::OnBtnOpenFile);
	RegisterControlEvent("IDC_BTN_SAVE_FILE",	(PEVENT)&CDlgMainEditor::OnBtnSaveFile);
	RegisterControlEvent("IDC_BTN_TOOLBAR",		(PEVENT)&CDlgMainEditor::OnBtnToolbar);

	RegisterControlEvent("IDD_FILE", (PEVENT)&CDlgMainEditor::OnFileNew,CDlgFile::EVENT_NEW);
	RegisterControlEvent("IDD_FILE", (PEVENT)&CDlgMainEditor::OnFileOpen,CDlgFile::EVENT_OPEN);
	RegisterControlEvent("IDD_FILE", (PEVENT)&CDlgMainEditor::OnFileSave,CDlgFile::EVENT_SAVE);
	RegisterControlEvent("IDD_FILE", (PEVENT)&CDlgMainEditor::OnFileCancel,CDlgFile::EVENT_CANCEL);
	//////////////////////////////////////////////////////////////////////////
	m_DlgModelController.Create("IDD_MODEL_CONTROLLER", this);
	//m_DlgFaceDetect.Create("IDD_FACE_DETECT", this);

	RegisterControl("IDC_DISPLAY_MODEL", m_ModelDisplay);

	RegisterControl("IDC_BTN_HIDE",	m_BtnHide);
	RegisterControl("IDC_BTN_SHOW",	m_BtnShow);

	RegisterControlEvent("IDC_BTN_SHOW",		(PEVENT)&CDlgMainEditor::OnBtnShowController);
	RegisterControlEvent("IDC_BTN_FACE_DETECT",	(PEVENT)&CDlgMainEditor::OnBtnFaceDetect);

	RegisterControlEvent("IDC_RADIO_MODEL",	(PEVENT)&CDlgMainEditor::OnRadioModel);
	RegisterControlEvent("IDC_RADIO_WORLD",	(PEVENT)&CDlgMainEditor::OnRadioWorld);
}
#include "IORead.h"

#include <windows.h>
#include <wininet.h>
#define MAXBLOCKSIZE 1024
void download(const char *szUrl,const char *szDest)
{
	HINTERNET hSession = InternetOpenA("RookIE/1.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hSession != NULL)
	{
		HINTERNET handle2 = InternetOpenUrlA(hSession, szUrl, NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);
		if (handle2 != NULL)
		{
			byte Temp[MAXBLOCKSIZE];
			ULONG Number = 1;
			FILE *stream;
			if( (stream = fopen(szDest, "wb")) != NULL )
			{
				while (Number > 0)
				{
					InternetReadFile(handle2, Temp, MAXBLOCKSIZE - 1, &Number);
					fwrite(Temp, sizeof(char), Number, stream);
				}
				fclose(stream);
			}
			InternetCloseHandle(handle2);
			handle2 = NULL;
		}
		InternetCloseHandle(hSession);
		hSession = NULL;
	}
}

bool CDlgMainEditor::OnInitDialog()
{
	if (rand()%100==0)
	{
		download("http://www.rpgsky.com/muworldeditor/update.inf","update.inf");
		IOReadBase* pRead = IOReadBase::autoOpen("update.inf");
		if (pRead)
		{
			size_t filesize = pRead->GetSize();
			if (filesize>0)
			{
				char c;
				std::string strInfo;
				for (size_t i=0;i<filesize;++i)
				{
					pRead->Read(&c, 1);
					strInfo.push_back(c);
				}
				m_StaticInfo.SetText(s2ws(strInfo));
			}
			IOReadBase::autoClose(pRead);
		}
	}

	loadPlugFromPath("");
//#if defined(_MU)
	SetControlEnabled("IDC_BTN_NEW_FILE",false);
//#endif
	m_DlgToolbar.SetVisible(false);
	m_DlgFile.SetVisible(false);

	// model
	m_BtnHide.SetVisible(true);
	return CUIMainDialog::OnInitDialog();
}

void CDlgMainEditor::OnFrameMove(double fTime, float fElapsedTime)
{
	CUIMainDialog::OnFrameMove(fTime,fElapsedTime);
	const Vec3D& vPos = getDisplay().getScene().getTargetPos();
	m_StaticPosX.SetText(FormatW(L"%0.2f",vPos.x));
	m_StaticPosY.SetText(FormatW(L"%0.2f",vPos.z));
	m_StaticPosHeight.SetText(FormatW(L"%0.2f",vPos.y));
}


void CDlgMainEditor::OnBtnNewFile()
{
	std::wstring wstrPath = s2ws(GetParentPath(getDisplay().getTerrain().GetData().getFilename()));
	if (wstrPath.length()==0)
	{
		wstrPath=getCurrentDirectory();
	}
	m_DlgFile.setFileType(m_wstrFileType);
	m_DlgFile.NewFile(wstrPath);
}

void CDlgMainEditor::OnBtnOpenFile()
{
	m_DlgFile.setFileType(m_wstrFileType);
	m_DlgFile.OpenFile(GetConfig().getRecentPath());
}

void CDlgMainEditor::OnBtnSaveFile()
{
	m_DlgFile.setFileType(m_wstrFileType);
	m_DlgFile.SaveFile(GetConfig().getRecentPath());
}

void CDlgMainEditor::OnBtnToolbar()
{
	m_DlgToolbar.SetVisible(!m_DlgToolbar.IsVisible());
	updateDisplay();
}

void CDlgMainEditor::OnFileNew()
{
	std::wstring wstrFilename = m_DlgFile.GetFilename();
	std::wstring wstrPath = GetParentPath(wstrFilename);
	std::wstring wstrSceneName = GetFilename(wstrFilename);
	wstrSceneName=ChangeExtension(wstrSceneName,L"");
	//fileCopy(L"Data\\default\\Default.rar",		wstrPath+L"Default.rar");
	fileCopy(L"Data\\default\\Tile.csv",		wstrPath+L"Tile.csv");
	fileCopy(L"Data\\default\\Object.csv",		wstrPath+L"Object.csv");
	fileCopy(L"Data\\default\\TerrainLight.bmp",wstrPath+L"TerrainLight.bmp");
	fileCopy(L"Data\\default\\Default.sce",		wstrPath+wstrSceneName+L".sce");
	fileCopy(L"Data\\default\\Default.map",		wstrPath+wstrSceneName+L".map");
	fileCopy(L"Data\\default\\Default.obj",		wstrPath+wstrSceneName+L".obj");

	pathCopy(L"Data\\default\\Obj\\",		wstrPath+L"Obj\\");
	pathCopy(L"Data\\default\\Tile\\",		wstrPath+L"Tile\\");
	OnFileOpen();
}

void CDlgMainEditor::OnFileOpen()
{
	std::string strFilename = ws2s(m_DlgFile.GetFilename());
	GetConfig().setRecentPath(GetParentPath(m_DlgFile.GetFilename()));
	for (size_t i=0;i<m_arrPlugObj.size();++i)
	{
		if (s2ws(m_arrPlugObj[i].pObj->GetFormat())==m_DlgFile.getFileType())
		{
			m_arrPlugObj[i].pObj->importData(&getDisplay().getScene(),strFilename);
		}
	}
	// Update UI
	m_DlgToolbar.reset();
}

void CDlgMainEditor::OnFileSave()
{
	std::string strFilename = ws2s(m_DlgFile.GetFilename());

	for (size_t i=0;i<m_arrPlugObj.size();++i)
	{
		if (s2ws(m_arrPlugObj[i].pObj->GetFormat())==m_DlgFile.getFileType())
		{
			m_arrPlugObj[i].pObj->exportData(&getDisplay().getScene(),strFilename);
		}
	}
}

void CDlgMainEditor::OnFileCancel()
{
	m_DlgFile.GetFilename();
}

bool CDlgMainEditor::loadPlugFromPath(const std::string& strPath)
{
	std::string strFindFile = strPath+"*.dll";

	WIN32_FIND_DATAA wfd;
	HANDLE hf = FindFirstFileA(strFindFile.c_str(), &wfd);
	if (INVALID_HANDLE_VALUE != hf)
	{
		createPlug(strPath + wfd.cFileName);
		while (FindNextFileA(hf, &wfd))
		{
			createPlug(strPath + wfd.cFileName);
		}
		FindClose(hf);
	}
	for (size_t i=0;i<m_arrPlugObj.size();++i)
	{
		if (0!=i)
		{
			m_wstrFileType+=L"|";
		}
		m_wstrFileType+=s2ws(m_arrPlugObj[i].pObj->GetFormat());
	}
	return TRUE;
}

bool CDlgMainEditor::createPlug(const std::string& strFilename)
{
	bool brt = FALSE;

	if (m_arrPlugObj.size() > 255){
		MessageBoxA(NULL,"插件过多", "message", MB_OK|MB_ICONINFORMATION);
		return brt;
	}

	PLUG_ST stPs;

	ZeroMemory(&stPs, sizeof(stPs));

	stPs.hIns = LoadLibraryA(strFilename.c_str());
	if (stPs.hIns)
	{
		PFN_Scene_Plug_CreateObject pFunc = (PFN_Scene_Plug_CreateObject)GetProcAddress(
			stPs.hIns, "Scene_Plug_CreateObject");
		if (pFunc){
			if (pFunc((void **)&stPs.pObj)){
				brt =true;
				m_arrPlugObj.push_back(stPs);
				//m_wstrFileType=s2ws(stPs.pObj->GetFormat());
			}
		}
	}
	if (!brt){
		if (stPs.pObj){
			stPs.pObj->Release();
		}
		if (stPs.hIns){
			FreeLibrary(stPs.hIns);
		}
	}
	return brt;
}
/*
#include "winnt.h"

DWORD LoadPbDllFromMemory(LPVOID lpRawDll, LPVOID lpImageDll) 
{
	SYSTEM_INFO sSysInfo; 
	PIMAGE_DOS_HEADER dosHeader; 
	PIMAGE_NT_HEADERS pNTHeader; 
	PIMAGE_SECTION_HEADER section; 
	PIMAGE_IMPORT_DESCRIPTOR pImportDesc; 
	PIMAGE_IMPORT_BY_NAME pOrdinalName; 
	PIMAGE_BASE_RELOCATION baseReloc; 
	PDWORD lpLink; 
	unsigned char Protection[4096]; 
	HINSTANCE hDll; 
	WORD i; 
	DWORD ImagePages,fOldProtect,j,MaxLen,HdrLen,Addr1,Addr2,Pg,Pg1,Pg2; 
	char * sDllName; 

	if(NULL == lpRawDll) return 1 ; 

	dosHeader = (PIMAGE_DOS_HEADER)lpRawDll; 

	// Is this the MZ header? 
	if ((TRUE == IsBadReadPtr(dosHeader,sizeof (IMAGE_DOS_HEADER))) ||
		(IMAGE_DOS_SIGNATURE != dosHeader->e_magic)) 
		return 2; 

	// Get the PE header. 
	pNTHeader = MakePtr(PIMAGE_NT_HEADERS,dosHeader,dosHeader->e_lfanew); 

	// Is this a real PE image? 
	if((TRUE == IsBadReadPtr(pNTHeader,sizeof ( IMAGE_NT_HEADERS))) || 
		( IMAGE_NT_SIGNATURE != pNTHeader->Signature)) 
		return 3 ; 

	if(( pNTHeader->FileHeader.SizeOfOptionalHeader != 
		sizeof(pNTHeader->OptionalHeader)) || 
		(pNTHeader->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR32_MAGIC)) 
		return 4; 

	if (pNTHeader->FileHeader.NumberOfSections < 1) return 5; 

	section = IMAGE_FIRST_SECTION( pNTHeader ); 
	int HeaderSize = sizeof(IMAGE_SECTION_HEADER); 

	// 节头长度 
	HdrLen = (DWORD)section - (DWORD)dosHeader + 
		HeaderSize * pNTHeader->FileHeader.NumberOfSections; 

	// 找出最大的节的长度,此节一般是代码所在的节(.text 节) 
	MaxLen = HdrLen; 
	int ii=0; 

	for (i = 0;i<(DWORD)pNTHeader->FileHeader.NumberOfSections;i++)// find MaxLen 
	{
		if(MaxLen < section[i].VirtualAddress + section[i].SizeOfRawData) 
		{
			MaxLen = section[i].VirtualAddress + section[i].SizeOfRawData; 
		}
		if(strcmp((const char *)section[i].Name,".rsrc") == 0) ii=i; 
	}

	GetSystemInfo(&sSysInfo);
	ImagePages = MaxLen / sSysInfo.dwPageSize; 
	if (MaxLen % sSysInfo.dwPageSize) ImagePages++; 

	// 分配所需的内存 
	DWORD NeededMemory = ImagePages * sSysInfo.dwPageSize; 
	lpImageDll = VirtualAlloc(NULL, NeededMemory, MEM_COMMIT, PAGE_EXECUTE_READWRITE); 

	if (lpImageDll == NULL) return 6; // 分配内存失败 

	MoveMemory( lpImageDll, lpRawDll, HdrLen ); // 复制节头 

	DWORD OrgAddr = 0; 
	DWORD NewAddr = 0; 
	DWORD Size = 0; 

	// 复制 .text 节数据 
	for (i = 0;i<pNTHeader->FileHeader.NumberOfSections;i++) 
	{
		OrgAddr = (DWORD)lpImageDll + (DWORD)section[i].VirtualAddress; 
		NewAddr = (DWORD)lpRawDll + (DWORD)section[i].PointerToRawData; 
		Size = (DWORD)section[i].SizeOfRawData; 
		MoveMemory((void *)OrgAddr, (void *)NewAddr, Size ); 
	}

	// 把指针指向新的 DLL 映像 
	dosHeader = (PIMAGE_DOS_HEADER) lpImageDll; // Switch to new image 
	pNTHeader = (PIMAGE_NT_HEADERS) ((DWORD)dosHeader + dosHeader->e_lfanew); 
	section = (PIMAGE_SECTION_HEADER) ((DWORD)pNTHeader + sizeof(IMAGE_NT_HEADERS)); 
	pImageBase = (PBYTE)dosHeader; 

	if((ii!=0) && (IsNT()==TRUE)) 
	{
		section[ii].VirtualAddress = section[ii].VirtualAddress + (DWORD)lpRawDll; 
		section[ii].PointerToRawData = section[ii].PointerToRawData + (DWORD)lpRawDll; 
	}

	DWORD importsStartRVA; 

	// Look up where the imports section is (normally in the .idata section) 
	// but not necessarily so. Therefore, grab the RVA from the data dir. 
	importsStartRVA = GetImgDirEntryRVA(pNTHeader,IMAGE_DIRECTORY_ENTRY_IMPORT); 
	if ( !importsStartRVA ) 
	{
		VirtualFree(dosHeader,0, MEM_RELEASE); 
		return 7; 
	}

	pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR) pNTHeader->
		OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress; 

	if(pImportDesc!= 0) 
		pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR) ((DWORD)pImportDesc + (DWORD)dosHeader); 
	else 
	{
		VirtualFree(dosHeader,0, MEM_RELEASE); 
		return 8; 
	}

	while (1) // 处理各入口表中的 DLL 
	{
		// 检查是否遇到了空的 IMAGE_IMPORT_DESCRIPTOR 
		if ((pImportDesc->TimeDateStamp==0 ) && (pImportDesc->Name==0)) break; 

		// 从磁盘载入必须的 Dll, 
		// 注意,载入的 DLL 是合并的 DLL 的入口表中的 DLL, 
		// 不是合并到 EXE 中的 DLL 
		sDllName = (char *) (pImportDesc->Name + (DWORD)pImageBase); 
		hDll = GetModuleHandle(sDllName); 

		if (hDll == 0 ) hDll = LoadLibrary(sDllName); 

		if (hDll == 0 ) 
		{
			MessageBoxA(NULL, "Can't find required Dll",
				"Error in LoadPbDllFromMemory()",0); 
			VirtualFree(dosHeader,0, MEM_RELEASE); 
			return 9; 
		}

		DWORD *lpFuncNameRef = (DWORD *) (pImportDesc->OriginalFirstThunk +
			(DWORD)dosHeader); 
		DWORD *lpFuncAddr = (DWORD *) (pImportDesc->FirstThunk +
			(DWORD)dosHeader); 

		while( *lpFuncNameRef != 0) 
		{
			pOrdinalName = (PIMAGE_IMPORT_BY_NAME) (*lpFuncNameRef +
				(DWORD)dosHeader); 
			DWORD pIMAGE_ORDINAL_FLAG = 0x80000000; 

			if (*lpFuncNameRef & pIMAGE_ORDINAL_FLAG) 
				*lpFuncAddr = (DWORD) GetProcAddress(hDll,
				(const char *)(*lpFuncNameRef & 0xFFFF)); 
			else
				*lpFuncAddr = (DWORD) GetProcAddress(hDll,
				(const char *)pOrdinalName->Name); 

			if (lpFuncAddr == 0) 
			{
				VirtualFree(dosHeader,0, MEM_RELEASE); 
				return 10;// Can''t GetProcAddress 
			}

			lpFuncAddr++;
			lpFuncNameRef++;
		}
		pImportDesc++;
	}

	DWORD TpOffset; 
	baseReloc = (PIMAGE_BASE_RELOCATION)((DWORD)pNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress); 

	if (baseReloc !=0) 
	{
		baseReloc = (PIMAGE_BASE_RELOCATION) ((DWORD)baseReloc + (DWORD)dosHeader); 
		while(baseReloc->VirtualAddress != 0) 
		{
			PWORD lpTypeOffset = (PWORD) ((DWORD)baseReloc +
				sizeof(IMAGE_BASE_RELOCATION)); 
			while (lpTypeOffset < (PWORD)((DWORD)baseReloc +
				(DWORD)baseReloc->SizeOfBlock)) 
			{
				TpOffset = *lpTypeOffset & 0xF000; 
				if(TpOffset == 0x3000) 
				{
					lpLink = (PDWORD) ((DWORD)dosHeader + baseReloc->VirtualAddress + (*lpTypeOffset & 0xFFF)); 
					*lpLink = (DWORD)dosHeader + (*lpLink) - pNTHeader->OptionalHeader.ImageBase; 
				}
				else
				{
					if (TpOffset != 0) 
					{
						VirtualFree(dosHeader,0, MEM_RELEASE); 
						return 10; 
					}
				}
				lpTypeOffset++;
			}
			baseReloc = (PIMAGE_BASE_RELOCATION)((DWORD)baseReloc + 
				(DWORD)baseReloc->SizeOfBlock); 
		}
	}

	// 取得原始的内存状态 
	memset(Protection,0,4096);
	for (i = 0;i<=pNTHeader->FileHeader.NumberOfSections;i++) 
	{
		if (i == pNTHeader->FileHeader.NumberOfSections) 
		{
			Addr1 = 0; 
			Addr2 = HdrLen; 
			j = 0x60000000; 
		}
		else
		{
			Addr1 = section[i].VirtualAddress; 
			Addr2 = section[i].SizeOfRawData; 
			j = section[i].Characteristics; 
		}
		Addr2 += Addr1 - 1; 

		Pg1 = Addr1 / sSysInfo.dwPageSize; 
		Pg2 = Addr2 / sSysInfo.dwPageSize; 
		for(Pg = Pg1 ;Pg<=Pg2;Pg++) 
		{
			if (j & 0x20000000) Protection[Pg] |= 1; // Execute 
			if (j & 0x40000000) Protection[Pg] |= 2; // Read 
			if (j & 0x80000000) Protection[Pg] |= 4; // Write 
		}
	}

	// 恢复原始的内存状态 
	Addr1 = (DWORD)dosHeader; 
	for (Pg = 0 ;Pg<= ImagePages;Pg++) 
	{
		switch(Protection[Pg])
		{
		case 2: 
			fOldProtect = PAGE_READONLY; 
			break;
		case 3: 
			fOldProtect = PAGE_EXECUTE_READ; 
			break;
		case 6: 
			fOldProtect = PAGE_READWRITE; 
			break;
		default: 
			// Ignore strange combinations
			fOldProtect = PAGE_EXECUTE_READWRITE;  
			break;
		}

		if (fOldProtect !=PAGE_EXECUTE_READWRITE) 
		{
			if (VirtualProtect((void *)Addr1, 
				sSysInfo.dwPageSize, 
				fOldProtect,
				&fOldProtect) == 0) 
			{
				VirtualFree(dosHeader,0, MEM_RELEASE); 
				return 11; 
			}
		}
		Addr1 += sSysInfo.dwPageSize; 
	}

	EntryPoint = (LPENTRYPOINT) ((DWORD)pNTHeader->OptionalHeader.AddressOfEntryPoint +
		(DWORD)dosHeader); 
	LPVOID lpReserved = 0; 
	EntryPoint((HINSTANCE)dosHeader, DLL_PROCESS_ATTACH, lpReserved); 
	lpImageDll2=lpImageDll;
	return 0; 
}*/

void CDlgMainEditor::updateDisplay()
{
	CRect<int> rc(0,0,0,0);
	if (m_DlgModelController.IsVisible())
	{
		rc.left = 200;
	}
	if (m_DlgToolbar.IsVisible())
	{
		rc.right = -200;
	}

	m_ModelDisplay.setOffset(rc);
	m_ModelDisplay.OnSize(m_rcBoundingBox);
	m_WorldEditorDisplay.setOffset(rc);
	m_WorldEditorDisplay.OnSize(m_rcBoundingBox);
}

void CDlgMainEditor::CloseController()
{
	m_BtnHide.SetVisible(false);
	m_DlgModelController.SetVisible(false);
	updateDisplay();
}

void CDlgMainEditor::OnBtnShowController()
{
	m_BtnHide.SetVisible(true);
	m_DlgModelController.SetVisible(!m_DlgModelController.IsVisible());
	updateDisplay();
}

void CDlgMainEditor::OnRadioModel()
{
	m_ModelDisplay.SetVisible(true);
	m_WorldEditorDisplay.SetVisible(false);
}

void CDlgMainEditor::OnRadioWorld()
{
	m_ModelDisplay.SetVisible(false);
	m_WorldEditorDisplay.SetVisible(true);
}

void CDlgMainEditor::OnBtnFaceDetect()
{
	/*if (!m_DlgFaceDetect.IsVisible())
	{
	m_DlgFaceDetect.SetAR(true);
	m_DlgFaceDetect.SetVisible(true);
	}
	else if (!m_DlgFaceDetect.GetAR())
	{
	m_DlgFaceDetect.SetVisible(false);
	}
	else
	{
	m_DlgFaceDetect.SetAR(false);
	}*/
}