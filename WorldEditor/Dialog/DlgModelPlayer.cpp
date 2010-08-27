#include "DlgModelPlayer.h"
#include "ModelDisplay.h"
#include "FileSystem.h"
#include "IniFile.h"
#include "..\MainRoot.h"

void CDlgModelPlayer::OnControlRegister()
{
	CUIDialog::OnControlRegister();

	RegisterControl("IDC_CMB_SKELETON",		m_ComboBoxSkeleton);
	RegisterControl("IDC_CMB_HEAD",			m_ComboBoxHead);
	RegisterControl("IDC_CMB_SET",			m_ComboBoxSet);
	RegisterControl("IDC_CMB_HELM",			m_ComboBoxHelm);
	RegisterControl("IDC_CMB_ARMOR",		m_ComboBoxArmor);
	RegisterControl("IDC_CMB_GLOVE",		m_ComboBoxGlove);
	RegisterControl("IDC_CMB_PANTS",		m_ComboBoxPants);
	RegisterControl("IDC_CMB_BOOT",			m_ComboBoxBoot);
	RegisterControl("IDC_CMB_LEFTHAND",		m_ComboBoxLeftHand);
	RegisterControl("IDC_CMB_RIGHTHAND",	m_ComboBoxRightHand);
	RegisterControl("IDC_CMB_BACK",			m_ComboBoxBack);


	RegisterControlEvent("IDC_CMB_SKELETON",	(PEVENT)&CDlgModelPlayer::OnUpdatePlayer);
	RegisterControlEvent("IDC_CMB_HEAD",		(PEVENT)&CDlgModelPlayer::OnUpdatePlayer);
	RegisterControlEvent("IDC_CMB_HELM",		(PEVENT)&CDlgModelPlayer::OnUpdatePlayer);
	RegisterControlEvent("IDC_CMB_ARMOR",		(PEVENT)&CDlgModelPlayer::OnUpdatePlayer);
	RegisterControlEvent("IDC_CMB_GLOVE",		(PEVENT)&CDlgModelPlayer::OnUpdatePlayer);
	RegisterControlEvent("IDC_CMB_PANTS",		(PEVENT)&CDlgModelPlayer::OnUpdatePlayer);
	RegisterControlEvent("IDC_CMB_BOOT",		(PEVENT)&CDlgModelPlayer::OnUpdatePlayer);
	RegisterControlEvent("IDC_CMB_LEFTHAND",	(PEVENT)&CDlgModelPlayer::OnUpdatePlayer);
	RegisterControlEvent("IDC_CMB_RIGHTHAND",	(PEVENT)&CDlgModelPlayer::OnUpdatePlayer);
	RegisterControlEvent("IDC_CMB_BACK",		(PEVENT)&CDlgModelPlayer::OnUpdatePlayer);

	RegisterControlEvent("IDC_CMB_SET",			(PEVENT)&CDlgModelPlayer::OnSelectSet);
}
#include <algorithm>
#include <cctype>
bool CDlgModelPlayer::OnInitDialog()
{
	m_ComboBoxSkeleton.AddItem(L"");
	m_ComboBoxHead.AddItem(L"");
	m_ComboBoxSet.AddItem(L"");
	m_ComboBoxHelm.AddItem(L"");
	m_ComboBoxArmor.AddItem(L"");
	m_ComboBoxGlove.AddItem(L"");
	m_ComboBoxPants.AddItem(L"");
	m_ComboBoxBoot.AddItem(L"");
	m_ComboBoxLeftHand.AddItem(L"");
	m_ComboBoxRightHand.AddItem(L"");
	m_ComboBoxBack.AddItem(L"");
	std::set<std::wstring>	setString;

	std::string strDirMU = IniGetStr("WorldEditor.cfg","ResDir","mu");
	std::wstring wstrFindPath = s2ws(strDirMU)+L"Player\\";//|{"+wstrFileType+L"}";
	CDir dir;
	dir.ReadDir(wstrFindPath);
	for (int i=0; i<dir.m_FileInfo.size(); i++)
	{
		if (!dir.m_FileInfo[i].IsDirectory())
		{
			std::wstring wstrFilename = dir.m_FileInfo[i].wstrFilename;
			std::transform(wstrFilename.begin(),wstrFilename.end(),wstrFilename.begin(),std::tolower);
			//if (wstrFileType.find(GetExtension(dir.m_FileInfo[i].wstrFilename))!=std::wstring::npos)
			if (GetExtension(wstrFilename)==L".bmd")
			{
				if (wstrFilename.find(L"helmclass")!=std::wstring::npos)
				{
					m_ComboBoxHead.AddItem(wstrFilename);
				}
				else if (wstrFilename.find(L"helm")!=std::wstring::npos)
				{
					m_ComboBoxHelm.AddItem(wstrFilename);
					int index=wstrFilename.find(L"helm");
					wstrFilename.replace(index, wcslen(L"helm"), L"*");
					setString.insert(wstrFilename);
				}
				else if (wstrFilename.find(L"armor")!=std::wstring::npos)
				{
					m_ComboBoxArmor.AddItem(wstrFilename);
					int index=wstrFilename.find(L"armor");
					wstrFilename.replace(index, wcslen(L"armor"), L"*");
					setString.insert(wstrFilename);
				}
				else if (wstrFilename.find(L"glove")!=std::wstring::npos)
				{
					m_ComboBoxGlove.AddItem(wstrFilename);
					int index=wstrFilename.find(L"glove");
					wstrFilename.replace(index, wcslen(L"glove"), L"*");
					setString.insert(wstrFilename);
				}
				else if (wstrFilename.find(L"pant")!=std::wstring::npos)
				{
					m_ComboBoxPants.AddItem(wstrFilename);
					int index=wstrFilename.find(L"pant");
					wstrFilename.replace(index, wcslen(L"pant"), L"*");
					setString.insert(wstrFilename);
				}
				else if (wstrFilename.find(L"boot")!=std::wstring::npos)
				{
					m_ComboBoxBoot.AddItem(wstrFilename);
					int index=wstrFilename.find(L"boot");
					wstrFilename.replace(index, wcslen(L"boot"), L"*");
					setString.insert(wstrFilename);
				}
				else if (wstrFilename.find(L"player")!=std::wstring::npos)
				{
					m_ComboBoxSkeleton.AddItem(wstrFilename);
				}
			}
		}
	}
	for (std::set<std::wstring>::iterator it=setString.begin();it!=setString.end();it++)
	{
		m_ComboBoxSet.AddItem(*it);
	}
	return true;
}

void CDlgModelPlayer::OnUpdatePlayer()
{
	if (wcslen(m_ComboBoxSkeleton.GetText())==0)
	{
		return;
	}
	std::string strDirMU = IniGetStr("WorldEditor.cfg","ResDir","mu");
	std::string strPlayerPath = strDirMU+"Player\\";
	CMainRoot::getInstance().getMainDialog().getModelDisplay().loadComplexModel(strPlayerPath+ws2s(m_ComboBoxSkeleton.GetText()));
	CModelComplex* pModelComplex = (CModelComplex*)CMainRoot::getInstance().getMainDialog().getModelDisplay().getModelObject();
	if (pModelComplex)
	{
		pModelComplex->loadSkinModel("head",strPlayerPath+ ws2s(m_ComboBoxHead.GetText()));
		pModelComplex->loadSkinModel("helm",strPlayerPath+ ws2s(m_ComboBoxHelm.GetText()));
		pModelComplex->loadSkinModel("armor",strPlayerPath+ ws2s(m_ComboBoxArmor.GetText()));
		pModelComplex->loadSkinModel("glove",strPlayerPath+ ws2s(m_ComboBoxGlove.GetText()));
		pModelComplex->loadSkinModel("pant",strPlayerPath+ ws2s(m_ComboBoxPants.GetText()));
		pModelComplex->loadSkinModel("boot",strPlayerPath+ ws2s(m_ComboBoxBoot.GetText()));
	}
	CMainRoot::getInstance().getMainDialog().getDlgModelController().OnUpdate();
}

void CDlgModelPlayer::OnSelectSet()
{
	std::wstring wstrSet = m_ComboBoxSet.GetText();
	int index=wstrSet.find(L"*");
	if (index!=std::wstring::npos)
	{
		{
			m_ComboBoxHelm.getListBox().SelectItem(0);
			std::wstring wstrPart=wstrSet;
			wstrPart.replace(index, wcslen(L"*"), L"helm");
			m_ComboBoxHead.getListBox().selectByText(wstrPart);
		}
		{
			m_ComboBoxHelm.getListBox().SelectItem(0);
			std::wstring wstrPart=wstrSet;
			wstrPart.replace(index, wcslen(L"*"), L"helm");
			m_ComboBoxHelm.getListBox().selectByText(wstrPart);
			if (m_ComboBoxHelm.getListBox().GetSelectedIndex()==0)
			{
				m_ComboBoxHead.getListBox().selectByText(wstrPart);
			}
		}
		{
			std::wstring wstrPart=wstrSet;
			wstrPart.replace(index, wcslen(L"*"), L"armor");
			m_ComboBoxArmor.getListBox().selectByText(wstrPart);
		}
		{
			std::wstring wstrPart=wstrSet;
			wstrPart.replace(index, wcslen(L"*"), L"glove");
			m_ComboBoxGlove.getListBox().selectByText(wstrPart);
		}
		{
			std::wstring wstrPart=wstrSet;
			wstrPart.replace(index, wcslen(L"*"), L"pant");
			m_ComboBoxPants.getListBox().selectByText(wstrPart);
		}
		{
			std::wstring wstrPart=wstrSet;
			wstrPart.replace(index, wcslen(L"*"), L"boot");
			m_ComboBoxBoot.getListBox().selectByText(wstrPart);
		}
	}
}