#pragma once
#include "DlgModelShow.h"
#include "DlgModelPlayer.h"

class CDlgModelController: public CDlgModelBaseDialog
{
public:
	CDlgModelController();
	~CDlgModelController();
public:
	void C3Model();
	void MPQModel();
	void initRecentPath();
	virtual void OnControlRegister();
	virtual bool OnInitDialog();
	void OnListBoxFolderItemDblClk();
	void OnTabChanged();
	void OnTabObjectChanged();
	void OpenFile(const std::wstring& wstrFilename);
	void OpenPath(const std::wstring& wstrPath, const std::wstring& wstrFileType=L"*.*");
	void OnClose();
	void OnUpdate();
	virtual CDlgModelMain& getDlgModelMain();
protected:
private:
	std::wstring	m_wstrPath;
	std::wstring	m_wstrFileType;
	CUIRadioButton m_RadioButtonShow;
	CUIRadioButton m_RadioButtonAnim;
	CUIRadioButton m_RadioButtonAbout;
	CDlgModelShow	m_DlgModelShow;
	CDlgModelAnim	m_DlgModelAnim;
	CUIDialog		m_DlgModelAbout;

	CUIRadioButton m_RadioButtonFile;
	CUIRadioButton m_RadioButtonPlayer;

	CDlgModelPlayer	m_DlgModelPlayer;
	CUIListBox		m_ListBoxFolder;
};