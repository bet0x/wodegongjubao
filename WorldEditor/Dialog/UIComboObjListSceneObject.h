#pragma once
#include "UIComboObjList.h"

class CUIComboObjListSceneObject : public CUIComboObjList
{
public:
	CUIComboObjListSceneObject();
	~CUIComboObjListSceneObject();
public:
	void initObject(CScene& scene);
	void SelectObjectByObjectID(uint64 id);
	uint64 getSelectedObjectID();


	void OnListBoxObjectSelection();
	void OnObjectListReload();
	void OnObjectListEdit();
	virtual void OnFrameMove(double fTime, float fElapsedTime);
	virtual void OnFrameRender(double fTime, float fElapsedTime);
public:
	CModelObject	m_ModelObject;
};