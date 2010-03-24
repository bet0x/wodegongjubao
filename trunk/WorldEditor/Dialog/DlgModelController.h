#pragma once
#include "DlgModelAnim.h"
#include "DlgModelShow.h"
#include "DlgModelPlayer.h"
#include "DlgModelRenderPass.h"

class CDlgModelController: public CDlgBaseEditor
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
protected:
private:
	std::wstring	m_wstrPath;
	std::wstring	m_wstrFileType;
	CUIRadioButton m_RadioButtonShow;
	CUIRadioButton m_RadioButtonAnim;
	CUIRadioButton m_RadioButtonRenderPass;
	CUIRadioButton m_RadioButtonParticle;
	CUIRadioButton m_RadioButtonAbout;
	CDlgModelShow	m_DlgModelShow;
	CDlgModelAnim	m_DlgModelAnim;
	CUIDialog		m_DlgModelAbout;
	CDlgModelRenderPass	m_DlgModelRenderPass;
	CUIDialog		m_DlgModelParticle;

	CUIRadioButton m_RadioButtonFile;
	CUIRadioButton m_RadioButtonPlayer;

	CDlgModelPlayer	m_DlgModelPlayer;
	CUIListBox		m_ListBoxFolder;
};