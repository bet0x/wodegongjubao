#include "DlgFile.h"
#include "FileSystem.h"
#include "IORead.h"

CDlgFile::CDlgFile():
m_eOperatingType(OPERATING_TYPE_MAX)
{
}

CDlgFile::~CDlgFile()
{	
}

void CDlgFile::OnControlRegister()
{
	CUIDialog::OnControlRegister();

	RegisterControl("IDC_LIST_BOX_FOLDER", m_ListBoxFolder);
	RegisterControl("IDC_EDIT_BOX_FILE", m_EditBoxFile);
	RegisterControl("IDC_BTN_NEW", m_BtnNew);
	RegisterControl("IDC_BTN_OPEN", m_BtnOpen);
	RegisterControl("IDC_BTN_SAVE", m_BtnSave);	

	RegisterControlEvent("IDC_LIST_BOX_FOLDER", (PEVENT)&CDlgFile::OnListBoxFolderSelection, EVENT_LISTBOX_SELECTION);
	RegisterControlEvent("IDC_LIST_BOX_FOLDER", (PEVENT)&CDlgFile::OnListBoxFolderItemDblClk, EVENT_LISTBOX_ITEM_DBLCLK);

	//RegisterControlEvent("IDC_EDIT_BOX_FILE", (PEVENT)&CDlgFile::OnEditBoxFile, EVENT_LISTBOX_SELECTION);

	RegisterControlEvent("IDC_BTN_NEW", (PEVENT)&CDlgFile::OnBtnNew);
	RegisterControlEvent("IDC_BTN_OPEN", (PEVENT)&CDlgFile::OnBtnOpen);
	RegisterControlEvent("IDC_BTN_SAVE", (PEVENT)&CDlgFile::OnBtnSave);
	RegisterControlEvent("IDC_BTN_CANCEL", (PEVENT)&CDlgFile::OnBtnCancel);
}

void CDlgFile::SetVisible(bool bVisible)
{
	CUIDialog::SetVisible(bVisible);

	m_BtnNew.SetVisible(false);
	m_BtnOpen.SetVisible(false);
	m_BtnSave.SetVisible(false);
	m_EditBoxFile.ClearText();
}

void CDlgFile::OnListBoxFolderSelection()
{
	std::wstring wstrFilename = m_ListBoxFolder.GetSelectedItem()->wstrText;
	if (wstrFilename.length()>0&&wstrFilename[0]!=L'\\')
	{
		m_EditBoxFile.SetText(wstrFilename);
	}
}

void CDlgFile::OnListBoxFolderItemDblClk()
{
	std::wstring wstrFolder = m_ListBoxFolder.GetSelectedItem()->wstrText;
	if (wstrFolder.length()>0&&wstrFolder[0]==L'\\')
	{
		if (L"\\.."==wstrFolder)
		{
			m_wstrPath = GetParentPath(m_wstrPath);
		}
		else
		{
			m_wstrPath = GetChildPath(m_wstrPath,wstrFolder.substr(1));
		}
		if(false == OpenPath(m_wstrPath))
		{
			OpenPath(L"");
		}
	}
	else
	{
		switch(m_eOperatingType)
		{
		case OPERATING_TYPE_NEW:
			OnBtnNew();
		case OPERATING_TYPE_OPEN:
			OnBtnOpen();
			break;
		case OPERATING_TYPE_SAVE:
			OnBtnSave();
			break;
		default:break;
		}
	}
}
void CDlgFile::OnBtnNew()
{
	if (m_EditBoxFile.GetText().length()==0)
	{
		return;
	}
	std::wstring wstrFilename = GetChildPath(m_wstrPath,m_EditBoxFile.GetText());
	if(IOReadBase::Exists(ws2s(wstrFilename)))
	{
		//!!
		m_wstrFilename = wstrFilename;
	}
	else if (IOReadBase::Exists(ws2s(m_EditBoxFile.GetText())))
	{
		//!!
		m_wstrFilename = m_EditBoxFile.GetText();
	}
	else
	{
		//OpenPath(m_EditBoxFile.GetText());
		//return;
		m_wstrFilename = wstrFilename;
	}
	m_wstrFilename=ChangeExtension(m_wstrFilename,m_wstrFileType);
	GetParentDialog()->SendEvent( EVENT_NEW, this );
	SetVisible(false);
}

void CDlgFile::OnBtnOpen()
{
	if (m_EditBoxFile.GetText().length()==0)
	{
		return;
	}
	std::wstring wstrFilename = GetChildPath(m_wstrPath,m_EditBoxFile.GetText());
	if(IOReadBase::Exists(ws2s(wstrFilename)))
	{
		m_wstrFilename = wstrFilename;
	}
	else if (IOReadBase::Exists(ws2s(m_EditBoxFile.GetText())))
	{
		m_wstrFilename = m_EditBoxFile.GetText();
	}
	else
	{
		OpenPath(m_EditBoxFile.GetText());
		return;
	}
	GetParentDialog()->SendEvent( EVENT_OPEN, this );
	SetVisible(false);
}

void CDlgFile::OnBtnSave()
{
	m_wstrFilename = m_wstrPath+m_EditBoxFile.GetText();
	GetParentDialog()->SendEvent( EVENT_SAVE, this );
	SetVisible(false);
}

void CDlgFile::OnBtnCancel()
{
	GetParentDialog()->SendEvent( EVENT_CANCEL, this );
	SetVisible(false);
}

bool CDlgFile::OpenPath(const std::wstring& wstrPath)
{
	CDir dir;
	if(false == dir.ReadDir(wstrPath))
	{
		return false;
	}
	m_wstrPath = wstrPath;
	m_ListBoxFolder.RemoveAllItems();
	for (size_t i=0; i<dir.m_FileInfo.size(); i++)
	{
		if (dir.m_FileInfo[i].IsDirectory())
		{
			if(dir.m_FileInfo[i].wstrFilename!=L".")
			{
				m_ListBoxFolder.AddItem(L"\\"+dir.m_FileInfo[i].wstrFilename);
			}
		}
	}
	for (size_t i=0; i<dir.m_FileInfo.size(); i++)
	{
		if (!dir.m_FileInfo[i].IsDirectory())
		{
			if (m_wstrFileType.find(GetExtension(dir.m_FileInfo[i].wstrFilename))!=std::wstring::npos)
			{

				m_ListBoxFolder.AddItem(dir.m_FileInfo[i].wstrFilename);
			}
		}
	}
	return true;
}

void CDlgFile::NewFile(const std::wstring& wstrPath, const std::wstring& wstrFileType)
{
	SetVisible(true);
	m_eOperatingType = OPERATING_TYPE_NEW;
	m_BtnNew.SetVisible(true);
	m_wstrFileType = wstrFileType;
	if(false == OpenPath(wstrPath))
	{
		OpenPath(L"");
	}
}

void CDlgFile::OpenFile(const std::wstring& wstrPath, const std::wstring& wstrFileType)
{
	SetVisible(true);
	m_eOperatingType = OPERATING_TYPE_OPEN;
	m_BtnOpen.SetVisible(true);
	m_wstrFileType = wstrFileType;
	if(false == OpenPath(wstrPath))
	{
		OpenPath(L"");
	}
}

void CDlgFile::SaveFile(const std::wstring& wstrPath, const std::wstring& wstrFileType)
{
	SetVisible(true);
	m_eOperatingType = OPERATING_TYPE_SAVE;
	m_BtnSave.SetVisible(true);
	m_wstrPath = wstrPath;
	m_wstrFileType = wstrFileType;
	if(false == OpenPath(m_wstrPath))
	{
		OpenPath(L"");
	}
}