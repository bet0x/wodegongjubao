#pragma once
#include "UIComboObjList.h"
#include "ModelComplex.h"

class CUIComboObjListSceneObject : public CUIComboObjList
{
public:
	CUIComboObjListSceneObject();
	~CUIComboObjListSceneObject();
public:
	void initObject(CScene& scene);
	void SelectObjectByObjectID(__int64 id);
	size_t getSelectedObjectID();


	void OnListBoxObjectSelection();
	void OnObjectListReload();
	void OnObjectListEdit();
	virtual void OnFrameMove(double fTime, float fElapsedTime);
	virtual void OnFrameRender(const Matrix& mTransform, double fTime, float fElapsedTime);
public:
	CModelObject	m_ModelObject;
};