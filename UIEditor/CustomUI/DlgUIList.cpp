#include "DlgUIList.h"
#include "EditingDialog.h"
#include "Tinyxml.h"
#include "List"

CDlgUIList::CDlgUIList():
m_pDlgView(NULL)
{
}

CDlgUIList::~CDlgUIList()
{	
}

void CDlgUIList::OnControlRegister()
{
	CUIDialog::OnControlRegister();
	RegisterControl("IDC_DIALOG_LIST", m_ListBox);
	RegisterControlEvent("IDC_DIALOG_LIST", (PEVENT)&CDlgUIList::OnDlgListBoxSelection,EVENT_LISTBOX_SELECTION);
}

bool CDlgUIList::InitDialogListFromXML(const std::string& strFilename)
{
	TiXmlDocument *myDocument = new TiXmlDocument( strFilename );
	myDocument->LoadFile(TIXML_ENCODING_UTF8);
	if ( myDocument->Error() )
	{
		delete myDocument;
		return false;
	}
	std::list<std::wstring> listString;
	//获得根元素，即root。
	TiXmlElement *pRootElement = myDocument->RootElement();
	//获得第一个dialog节点。
	TiXmlElement *pDialogElement = pRootElement->FirstChildElement("dialog");
	while ( pDialogElement )
	{
		if ( pDialogElement->Attribute("id") )
		{
			WCHAR strText[256]={0};
			MultiByteToWideChar(CP_UTF8,0,pDialogElement->Attribute("id"),-1,strText,256);
			//m_ListBox.AddItem( strText, 0 );
			listString.push_back(strText);
		}
		// 查找下一个dialog
		pDialogElement = pDialogElement->NextSiblingElement("dialog");
	}
	delete myDocument;
	//
	listString.sort();
	for(std::list<std::wstring>::iterator it=listString.begin(); it!=listString.end(); it++)
	{
		m_ListBox.AddItem(*it);
	}
	return true;
}

void CDlgUIList::OnDlgListBoxSelection()
{
	static CEditingDialog* s_pDialog = NULL;
	if (s_pDialog)
	{
		S_DEL(s_pDialog);
	}
	char szBuf[256]={0};
	s_pDialog = new CEditingDialog;
	WideCharToMultiByte(CP_UTF8,0,m_ListBox.GetSelectedItem()->wstrText.c_str(),-1,szBuf,256,NULL,NULL);
	s_pDialog->Create(szBuf, m_pDlgView);
	s_pDialog->OnSize(m_pDlgView->GetBoundingBox());
}
