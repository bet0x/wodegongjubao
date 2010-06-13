#include "DlgToolObject.h"
#include "Scene.h"
#include "DlgToolbar.h"

CDlgToolObject::CDlgToolObject()
{
}

CDlgToolObject::~CDlgToolObject()
{	
}

void CDlgToolObject::OnControlRegister()
{
	CUIDialog::OnControlRegister();


	RegisterControl("IDC_OBJLIST_SCENE_OBJECTS",		m_ObjListSceneObject);

	RegisterControl("IDC_NUM_FLOOR_SNAP",		m_NumFloorSnap);
	RegisterControl("IDC_NUM_GRID_SNAP",		m_NumGridSnap);

	RegisterControl("IDC_VEC3D_POS",		m_Vec3DPos);
	RegisterControl("IDC_VEC3D_ROTATE",		m_Vec3Rotate);
	RegisterControl("IDC_NUM_SCALE",		m_NumScale);

	RegisterControlEvent("IDC_VEC3D_POS",		(PEVENT)&CDlgToolObject::OnFocusObjectValueChanged);
	RegisterControlEvent("IDC_VEC3D_ROTATE",	(PEVENT)&CDlgToolObject::OnFocusObjectValueChanged);
	RegisterControlEvent("IDC_NUM_SCALE",		(PEVENT)&CDlgToolObject::OnFocusObjectValueChanged);

	RegisterControlEvent("IDC_NUM_FLOOR_SNAP",	(PEVENT)&CDlgToolObject::OnNumFloorSnapChanged);
	RegisterControlEvent("IDC_NUM_GRID_SNAP",	(PEVENT)&CDlgToolObject::OnNumGridSnapChanged);

	RegisterEvent("MSG_FOCUS_OBJECT_CHANGED",(PEVENT)&CDlgToolObject::OnFocusObjectChanged);
	RegisterEvent("MSG_ADD_OBJECT",(PEVENT)&CDlgToolObject::OnAddObject);

	RegisterEvent(USER_DEFINED_MSG_TYPE_OBJECT_POS_CHANGED,(PEVENT)&CDlgToolObject::OnObjectPosChanged);
}

void CDlgToolObject::initObject()
{
	m_ObjListSceneObject.initObject(getDisplay().getScene());
}

void CDlgToolObject::OnNumFloorSnapChanged()
{
	getDisplay().setFloorSnap(m_NumFloorSnap.getFloat());
}

void CDlgToolObject::OnNumGridSnapChanged()
{
	getDisplay().setGridSnap(m_NumGridSnap.getFloat());
}

void CDlgToolObject::OnFocusObjectValueChanged()
{
	if (getDisplay().getScene().getFocusObjects().size()>0)
	{
		Vec3D vPos=m_Vec3DPos.getVec3D();
		Vec3D vRotate=m_Vec3Rotate.getVec3D()*(PI/180);
		float fScale = m_NumScale.getFloat();
		Vec3D vScale(fScale,fScale,fScale);
		if (fScale<0.1f||fScale>=100.f)
		{
			fScale = 1.0f;
			m_NumScale.setFloat(fScale);
		}

		getDisplay().getScene().setFocusObjectsPos(vPos);
		getDisplay().getScene().setFocusObjectsRotate(vRotate);
		getDisplay().getScene().setFocusObjectsScale(vScale);
	}
	OnFocusObjectChanged();
}

void CDlgToolObject::OnFocusObjectChanged()
{
	if (getDisplay().getScene().getFocusObjects().size()>0)
	{
		Vec3D vPos = getDisplay().getScene().getFocusObjectsPos();
		Vec3D vRotate = getDisplay().getScene().getFocusObjectsRotate();
		Vec3D vScale = getDisplay().getScene().getFocusObjectsScale();

		m_Vec3DPos.setVec3D(vPos);
		m_Vec3Rotate.setVec3D(vRotate*180/PI);
		m_NumScale.setFloat(vScale.x);

		if (getDisplay().getScene().getFocusObjects().size()==1)
		{
			CMapObj* pObject = getDisplay().getScene().getFocusObjects()[0];
			if (pObject->GetObjType()==MAP_3DSIMPLE)
			{
				m_ObjListSceneObject.SelectObjectByObjectID(((C3DMapSceneObj*)pObject)->getObjectID());
			}
		}
	}
}

void CDlgToolObject::OnAddObject()
{
	Vec3D vRotate=m_Vec3Rotate.getVec3D()*PI/180;
	float fScale = m_NumScale.getFloat();
	if (fScale<0.1f||fScale>=100.f)
	{
		fScale = 1.0f;
		m_NumScale.setFloat(fScale);
	}
	Vec3D vScale=Vec3D(fScale,fScale,fScale);
	CMapObj* pObject = getDisplay().getScene().add3DMapSceneObj(m_ObjListSceneObject.getSelectedObjectID(),getDisplay().getScene().getTargetPos(),vRotate,vScale);

	getDisplay().getScene().addFocusObject(pObject);
	OnFocusObjectChanged();
	OnObjectPosChanged();
}

void CDlgToolObject::OnObjectPosChanged()
{
	if (getDisplay().getScene().getFocusObjects().size()>0)
	{
		Vec3D vPos = getDisplay().getScene().getFocusObjectsPos();
		//if (m_CheckBoxCatchAtGrid.GetCheckValue())
		//{
		//	float fGridSize = m_NumGridSize.getFloat();
		//	if (fGridSize<0.1f||fGridSize>=100.f) // check the value is not zero.
		//	{
		//		fGridSize=0.5f;
		//		m_NumGridSize.setFloat(fGridSize);
		//	}
		//	for (int i=0;i<3;i++)
		//	{
		//		vPos.f[i] = floorf((vPos.f[i]/m_NumGridSize.getFloat()+0.5f))*m_NumGridSize.getFloat();
		//	}
		//}
		//if (m_CheckBoxCatchAtFloor.GetCheckValue())
		//{
		//	vPos.y = getDisplay().getTerrain().GetData().GetHeight(Vec2D(vPos.x,vPos.z));
		//}
		//pObject->setPos(vPos);
		m_Vec3DPos.setVec3D(vPos);
	}
}
