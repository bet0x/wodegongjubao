#include "DlgModelPlayer.h"
#include "..\Config\Config.h"
#include "DlgModelController.h"
#include "ModelDisplay.h"

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

	std::wstring wstrFindPath = s2ws(GetModelConfig().m_strMUPath)+L"Player\\";//|{"+wstrFileType+L"}";
	CDir dir;
	dir.ReadDir(wstrFindPath);
	for (int i=0; i<dir.m_FileInfo.size(); i++)
	{
		if (!dir.m_FileInfo[i].IsDirectory())
		{
			std::wstring wstrFilename = dir.m_FileInfo[i].wstrFilename;
			//if (wstrFileType.find(GetExtension(dir.m_FileInfo[i].wstrFilename))!=std::wstring::npos)
			if (GetExtension(wstrFilename)==L".bmd")
			{
				if (wstrFilename.find(L"HelmClass")!=std::wstring::npos)
				{
					m_ComboBoxHead.AddItem(wstrFilename);
				}
				else if (wstrFilename.find(L"Helm")!=std::wstring::npos)
				{
					m_ComboBoxHelm.AddItem(wstrFilename);
					int index=wstrFilename.find(L"Helm");
					wstrFilename.replace(index, wcslen(L"Helm"), L"*");
					setString.insert(wstrFilename);
				}
				else if (wstrFilename.find(L"Armor")!=std::wstring::npos)
				{
					m_ComboBoxArmor.AddItem(wstrFilename);
					int index=wstrFilename.find(L"Armor");
					wstrFilename.replace(index, wcslen(L"Armor"), L"*");
					setString.insert(wstrFilename);
				}
				else if (wstrFilename.find(L"Glove")!=std::wstring::npos)
				{
					m_ComboBoxGlove.AddItem(wstrFilename);
					int index=wstrFilename.find(L"Glove");
					wstrFilename.replace(index, wcslen(L"Glove"), L"*");
					setString.insert(wstrFilename);
				}
				else if (wstrFilename.find(L"Pant")!=std::wstring::npos)
				{
					m_ComboBoxPants.AddItem(wstrFilename);
					int index=wstrFilename.find(L"Pant");
					wstrFilename.replace(index, wcslen(L"Pant"), L"*");
					setString.insert(wstrFilename);
				}
				else if (wstrFilename.find(L"Boot")!=std::wstring::npos)
				{
					m_ComboBoxBoot.AddItem(wstrFilename);
					int index=wstrFilename.find(L"Boot");
					wstrFilename.replace(index, wcslen(L"Boot"), L"*");
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
	if (m_ComboBoxSkeleton.GetText().length()==0)
	{
		return;
	}
	std::string strPlayerPath = GetModelConfig().m_strMUPath+"Player\\";
	getModelDisplay().loadComplexModel(strPlayerPath+ws2s(m_ComboBoxSkeleton.GetText()));
	CModelComplex* pModelComplex = (CModelComplex*)getModelDisplay().getModelObject();
	if (pModelComplex)
	{
		pModelComplex->loadSkinModel("head",strPlayerPath+ ws2s(m_ComboBoxHead.GetText()));
		pModelComplex->loadSkinModel("helm",strPlayerPath+ ws2s(m_ComboBoxHelm.GetText()));
		pModelComplex->loadSkinModel("armor",strPlayerPath+ ws2s(m_ComboBoxArmor.GetText()));
		pModelComplex->loadSkinModel("glove",strPlayerPath+ ws2s(m_ComboBoxGlove.GetText()));
		pModelComplex->loadSkinModel("pant",strPlayerPath+ ws2s(m_ComboBoxPants.GetText()));
		pModelComplex->loadSkinModel("boot",strPlayerPath+ ws2s(m_ComboBoxBoot.GetText()));
	}
	((CDlgModelController*)GetParentDialog())->OnUpdate();
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
			wstrPart.replace(index, wcslen(L"*"), L"Helm");
			m_ComboBoxHead.getListBox().selectByText(wstrPart);
		}
		{
			m_ComboBoxHelm.getListBox().SelectItem(0);
			std::wstring wstrPart=wstrSet;
			wstrPart.replace(index, wcslen(L"*"), L"Helm");
			m_ComboBoxHelm.getListBox().selectByText(wstrPart);
			if (m_ComboBoxHelm.getListBox().GetSelectedIndex()==0)
			{
				m_ComboBoxHead.getListBox().selectByText(wstrPart);
			}
		}
		{
			std::wstring wstrPart=wstrSet;
			wstrPart.replace(index, wcslen(L"*"), L"Armor");
			m_ComboBoxArmor.getListBox().selectByText(wstrPart);
		}
		{
			std::wstring wstrPart=wstrSet;
			wstrPart.replace(index, wcslen(L"*"), L"Glove");
			m_ComboBoxGlove.getListBox().selectByText(wstrPart);
		}
		{
			std::wstring wstrPart=wstrSet;
			wstrPart.replace(index, wcslen(L"*"), L"Pant");
			m_ComboBoxPants.getListBox().selectByText(wstrPart);
		}
		{
			std::wstring wstrPart=wstrSet;
			wstrPart.replace(index, wcslen(L"*"), L"Boot");
			m_ComboBoxBoot.getListBox().selectByText(wstrPart);
		}
	}
}