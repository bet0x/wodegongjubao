#pragma once
#include "DlgBaseEditor.h"
#include "UIComboNumber.h"
#include "UIComboObjListSceneObject.h"

class CDlgToolObject : public CDlgBaseEditor
{
public:
	CDlgToolObject();
	~CDlgToolObject();
public:
	virtual void OnControlRegister();
	void initObject();
	void OnFocusObjectValueChanged();
	void OnFocusObjectChanged();
	void OnAddObject();
	void OnObjectPosChanged();
	void OnObjectListReload();
private:
	CUIComboObjListSceneObject m_ObjListSceneObject;

	CUICheckBox m_CheckBoxCatchAtFloor;
	CUICheckBox m_CheckBoxCatchAtGrid;
	CUIComboNumber	m_NumGridSize;
	CUIComboNumber	m_NumPosX;
	CUIComboNumber	m_NumPosY;
	CUIComboNumber	m_NumPosZ;
	CUIComboNumber	m_NumRotateX;
	CUIComboNumber	m_NumRotateY;
	CUIComboNumber	m_NumRotateZ;
	CUIComboNumber	m_NumScale;
};