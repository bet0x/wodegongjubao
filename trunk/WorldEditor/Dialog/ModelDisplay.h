#pragma once
#include "UIDisplay.h"
#include "Camera.h"
#include "SkyBox.h"
#include "ModelComplex.h"
#include "SceneEffect.h"

// œ‘ æ¿‡
class CModelDisplay : public CUIDisplay
{
public:
	CModelDisplay();
	~CModelDisplay();

public:
	void	LoadModel(const std::string& strFilename);
	void	loadComplexModel(const std::string& strFilename);
	void	OnFrameMove( double fTime, float fElapsedTime );
	void	OnFrameRender( double fTime, float fElapsedTime );
	bool	HandleMouse(UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam);
	void	MouseProc( bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown,
		bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta, int xPos, int yPos );
	void	SetRealEye(const Vec3D& vEye){m_vEye = vEye;}
	const Vec3D& GetRealEye(){return m_vEye;}

	virtual void OnMouseMove(POINT point);
	virtual void OnMouseWheel(POINT point,short wheelDelta);
	virtual void OnLButtonDown(POINT point);
	virtual void OnLButtonUp(POINT point);
	virtual void OnRButtonDown(POINT point);
	virtual void OnRButtonUp(POINT point);
	virtual void OnMButtonDown(POINT point);
	virtual void OnMButtonUp(POINT point);

	virtual void OnDropRect();
	virtual void OnSize(const CRect<int>& rc);

	CModelObject* getModelObject();
	CModelData*  getModelData()const;
public:
	CCamera				m_Camera;
	Vec3D				m_vEye;
	CSceneEffect		m_SceneEffect;
public:
	POINT				m_ptLastMousePosition;
	bool m_bShowBones;
	bool m_bShowBounds;
	bool m_bShowModel;
	bool m_bShowWireframe;
	bool m_bShowMaterial;
	bool m_bShowParticles;


	bool m_bLbuttonDrop;
	CRect<float> m_DropRect;
	
	Vec3D m_vPickRectLeft;
	Vec3D m_vPickRectBottom;

	VERTEX_XYZ_DIF m_PickPlane[5];

	CMeshCoordinate		m_MeshCoordinate;
private:
	CModelObject* m_pModelObject;

	bool g_bLeftButtonDown;
	bool g_bRightButtonDown;
	bool g_bMiddleButtonDown;
};