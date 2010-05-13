#pragma once
#include "DlgModelAnim.h"
#include "DlgModelShow.h"
#include "DlgModelPlayer.h"
#include "DlgModelRenderPass.h"

class CDlgController: public CDlgBaseEditor
{
public:
	CDlgController();
	~CDlgController();
public:
	void C3Model();
	void MPQModel();
	void initRecentPath();
	virtual void OnControlRegister();
	virtual bool OnInitDialog();
	void OnListBoxFolderItemDblClk();

	void OnTabObjectChanged();
	void OpenFile(const std::wstring& wstrFilename);
	void OpenPath(const std::wstring& wstrPath, const std::wstring& wstrFileType=L"*.*");
	void OnClose();
	void OnUpdate();
protected:
private:
	std::wstring	m_wstrPath;
	std::wstring	m_wstrFileType;

	CUIDialog		m_DlgModelAbout;

	CUIRadioButton m_RadioButtonFile;
	CUIRadioButton m_RadioButtonPlayer;
	CUIRadioButton m_RadioButtonAbout;

	CDlgModelPlayer	m_DlgModelPlayer;
	CUIListBox		m_ListBoxFolder;
};