#pragma once
#include "Manager.h"
#include "UIDialog.h"
#include "UIControl.h"
//#include "ControlID.h"

#include "UIStatic.h"
#include "UIButton.h"
#include "UICheckBox.h"
#include "UIComboBox.h"
#include "UIRadioButton.h"
#include "UIImage.h"
#include "UIListBox.h"
#include "UIScrollBar.h"
#include "UISlider.h"
#include "UIEditBox.h"
#include "UIIMEEditBox.h"
class CDlgFile : public CUIDialog
{
public:
	CDlgFile();
	~CDlgFile();
public:
	enum EVENT_TYPE
	{
		EVENT_NEW,
		EVENT_OPEN,
		EVENT_SAVE,
		EVENT_CANCEL,
	};
	enum OPERATING_TYPE
	{
		OPERATING_TYPE_NEW,
		OPERATING_TYPE_OPEN,
		OPERATING_TYPE_SAVE,
		OPERATING_TYPE_MAX,
	};
	virtual void OnControlRegister();
	virtual void SetVisible(bool bVisible);

	void OnListBoxFolderSelection();
	void OnListBoxFolderItemDblClk();

	void OnListBoxFileSelection();
	void OnListBoxFileDblClk();

	void OnBtnNew();
	void OnBtnOpen();
	void OnBtnSave();
	void OnBtnCancel();
	const std::wstring& GetFilename(){return m_wstrFilename;}
	bool OpenPath(const std::wstring& wstrPath);
	void NewFile(const std::wstring& wstrPath, const std::wstring& wstrFileType=L"*.*");
	void OpenFile(const std::wstring& wstrPath, const std::wstring& wstrFileType=L"*.*");
	void SaveFile(const std::wstring& wstrPath, const std::wstring& wstrFileType);
	bool IsOpenFile(){return OPERATING_TYPE_OPEN==m_eOperatingType;}
	bool IsSaveFile(){return OPERATING_TYPE_SAVE==m_eOperatingType;}
protected:
private:
	CUIListBox m_ListBoxFolder;
	CUIListBox m_ListBoxFile;
	CUIEditBox m_EditBoxFile;
	CUIButton m_BtnNew;
	CUIButton m_BtnOpen;
	CUIButton m_BtnSave;
	std::wstring	m_wstrPath;
	std::wstring	m_wstrFilename;
	std::wstring	m_wstrFileType;
	OPERATING_TYPE	m_eOperatingType;
};