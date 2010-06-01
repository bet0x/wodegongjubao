#include "UIWorldEditorDisplay.h"
#include "RenderSystem.h"

/*#include "AntTweakBar/AntTweakBar.h"*/
#include "Scene.h"
#include "SkyBox.h"
#include "Shader.h"
//#include "SceneEffect.h"
//#include "Config.h"
//#include "Quake3Bsp.h"
#include "UIDialog.h"

#include "DlgBaseEditor.h"

#include "Graphics.h"
#include "TextRender.h"
CUIWorldEditorDisplay::CUIWorldEditorDisplay():
m_bKeyCtrl(false),
m_bKeyUp(false),
m_bKeyDown(false),
m_bKeyLeft(false),
m_bKeyRight(false),
m_fCoordScale(0.2f),
m_fGridSize(0.5f)
{
	//// 生成摄像机的视角参数
	//Vec3D vEye(20.0f, 20.0f, 20.0f);
	//Vec3D vAt (0.0f, -1.0f, 2.0f);
	//vAt+=vEye;
	//m_Camera.SetViewParams(vEye, vAt);
	//m_Camera.setTargetPos(Vec3D(10,10,10));

	m_Camera.setYawAngle(PI/4);
	m_Camera.setPitchAngle(-PI/4);
	m_Scene.SetTerrain(&m_Terrain);

	g_bLeftButtonDown = false;
	g_bRightButtonDown = false;
	g_bMiddleButtonDown = false;
}

CUIWorldEditorDisplay::~CUIWorldEditorDisplay()
{
}

//HRESULT CUIWorldEditorDisplay::OnCreateDevice(LPDIRECT3DDEVICE9 pd3dDevice)
//{
//	HRESULT hr = S_OK;
//	int i=0;


	// bsp
	//GetBsp()->LoadBSP("Data/maps/tutorial.bsp");

	//GetSkyBox().Create();

// 	TwInit(TW_DIRECT3D9, pd3dDevice);
// 	TwBar *bar = TwNewBar("RPGSky");
// 	TwDefine(" GLOBAL help='How are you, world?' "); // Message added to the help bar.
// 	TwAddVarRW(bar, "FogColor", TW_TYPE_COLOR32, &GetFog().dwColor, " label='Background color' help='Number of segments of the strip.' ");
// 	// Add 'color' to 'bar': it is a modifable variable of type TW_TYPE_COLOR3F (3 floats color)
// 	TwAddVarRW(bar, "FogStart", TW_TYPE_FLOAT, &GetFog().fStart, " label='Background color' help='Number of segments of the strip.' ");
// 	TwAddVarRW(bar, "FogEnd", TW_TYPE_FLOAT, &GetFog().fEnd, " label='Background color' help='Number of segments of the strip.' ");
// 	TwAddVarRW(bar, "FogDensity", TW_TYPE_FLOAT, &GetFog().fDensity, " label='Background color' help='Number of segments of the strip.' ");

//	return S_OK;
//}

void CUIWorldEditorDisplay::OnFrameMove(double fTime, float fElapsedTime)
{
	if (!IsVisible())
	{
		return;
	}
	CRect<int> rcViewport = getViewport();
	m_Camera.SetProjParams(PI/3, rcViewport.getWidth(), rcViewport.getHeight(), 0.1f, m_Scene.getFog().fEnd);
	// 更新视矩阵
	m_Camera.FrameMove(fElapsedTime);

	CUIDisplay::OnFrameMove(fTime, fElapsedTime);
	m_Scene.OnFrameMove(fTime, fElapsedTime);

	MoveCamera(	m_bKeyLeft?-1000.0f*fElapsedTime:(m_bKeyRight?1000.0f*fElapsedTime:0),
				m_bKeyUp?1000.0f*fElapsedTime:(m_bKeyDown?-1000.0f*fElapsedTime:0));
}

void CUIWorldEditorDisplay::OnFrameRender(double fTime, float fElapsedTime)
{
	if (!IsVisible())//&&!isStyleVisible())
	{
		return;
	}
	CRect<int> rcViewport = getViewport();
	CRenderSystem& R = GetRenderSystem();
	CShader* pShader = R.GetShaderMgr().getSharedShader();
	pShader->setFloat("g_fTime",fTime);
	pShader->setMatrix("g_mViewProj",m_Camera.GetProjXView());
	pShader->setMatrix("g_mView",m_Camera.GetViewMatrix());
	pShader->setVec3D("g_vLightDir",m_Scene.getTerrain()->GetData().GetLightDir());
	pShader->setVec3D("g_vEyePot",m_Camera.GetEyePt());

	R.SetupRenderState();
	R.setWorldMatrix(Matrix::UNIT);
	R.SetSamplerFilter(0, TEXF_LINEAR, TEXF_LINEAR, TEXF_LINEAR);
	R.SetSamplerFilter(1, TEXF_LINEAR, TEXF_LINEAR, TEXF_LINEAR);
	R.setProjectionMatrix(m_Camera.GetProjMatrix());
	R.setViewMatrix(m_Camera.GetViewMatrix());

	bool bBloom = false;
	bool bCamma = false;

	if (bBloom||bCamma)
	{
		m_SceneEffect.renderTargetBegin();
		CRect<int> rcRenderTarget(0,0,rcViewport.getWidth(),rcViewport.getHeight());
		R.setViewport(rcRenderTarget);
	}
	else
	{
		R.setViewport(rcViewport);
	}

	// 渲染天空盒
	//m_SkyBox.Render(m_Camera.GetViewMatrix());
	//pShader->setTexture("g_texEnvironment",m_SkyBox.m_pCubeMap);
	// 刷雾色
	//R.ClearBuffer(true, true, GetFog().dwColor);
	/*GetRenderSystem().setFog(16.0f,32.0f,0.04f,0xFF000044);
	GetRenderSystem().setFogEnable(false);*/
	{

		//室内
		//D3DXMatrixScaling(&matW, 0.05, 0.05, 0.05);
		//pDevice->SetWorldMatrix(matW);
		//		GetBsp()->Render(m_Camera.GetFrustum());
		//GetBsp()->RenderBox(m_Camera.GetFrustum());
		//g_SceneBsp.Pick(GetRoleMgr()->GetLeader()->GetPos(), &(*GetRoleMgr()->GetLeader()->GetPos()+m_Camera.GetViewDir()));
		// 地形渲染
		R.setProjectionMatrix(m_Camera.GetProjMatrix());
		R.setViewMatrix(m_Camera.GetViewMatrix());
		R.setWorldMatrix(Matrix::UNIT);
		m_Scene.UpdateRender(m_Camera.GetFrustum());
		m_Scene.OnFrameRender(fTime,fElapsedTime);

		R.SetCullingMode(CULL_NONE);

		R.setWorldMatrix(Matrix::UNIT);

		{
			//D3DLIGHT9 light;
			//light.Type		= D3DLIGHT_DIRECTIONAL;
			//light.Ambient	= D3DXCOLOR(0.6f,0.6f,0.6f,0.6f);
			//light.Diffuse	= D3DXCOLOR(0.6f,0.6f,0.6f,0.6f);
			//light.Specular	= D3DXCOLOR(0.6f,0.6f,0.6f,0.6f);
			//light.Direction = D3DXVECTOR3(-1,-1,-1);
			//R.SetLight(0, &light);
		}

		//GetObjectMgr().OnRender();
	}
	R.setWorldMatrix(Matrix::UNIT);


	R.ClearBuffer(true,false,0x0);
	if (m_Scene.getFocusObjects().size()>0)
	{
		m_MeshCoordinate.setPos(m_Scene.getFocusObjectsPos());
		if (!IsPressed())
		{
			Vec3D vLength = m_Scene.getFocusObjectsPos()-m_Camera.GetEyePt();
			m_MeshCoordinate.setScale(vLength.length()*m_fCoordScale);
		}
		if (m_vPosMoveOn.length()>0)
		{
			m_MeshCoordinate.render(m_vPosMoveOn);
		}
		else
		{
			m_MeshCoordinate.render(m_vPosPressed);
		}
	}
	else
	{
		m_MeshCoordinate.setPos(Vec3D(0,0,0));
		m_MeshCoordinate.setScale(1);
		m_MeshCoordinate.render(Vec3D(0,0,0));
	}
	R.setWorldMatrix(Matrix::UNIT);
	if (g_bLeftButtonDown)
	{
		CGraphics& G=GetGraphics();
		//R.ClearBuffer(true,false,0x0);
		if (R.prepareMaterial("Coordinate1"))
		{
			G.DrawLine3D(m_vObjectLastPos,m_vAfterCatchPos,0xFF00FFFF);
			{
				Pos2D pos;
				R.world2Screen(m_vObjectLastPos,pos);
				CRect<float> rc(pos.x,pos.y,pos.x,pos.y);
				rc.InflateRect(2,2);
				G.FillRect(rc,0xFF00FFFF);
			}
		}

		if (m_vBeforeCatchPos!=m_vAfterCatchPos)
		{
			if (R.prepareMaterial("Coordinate1"))
			{
				//for (size_t i=0;i<3;++i)
				//{
				//	for (size_t j=0;j<2;++j)
				//	{
				//		Pos2D pos;
				//		Vec3D vPos=m_vAfterCatchPos;
				//		vPos.f[i]+=(0==j?m_fGridSize:-m_fGridSize);
				//		R.world2Screen(vPos,pos);
				//		CRect<float> rc(pos.x,pos.y,pos.x,pos.y);
				//		rc.InflateRect(2,2);
				//		G.FillRect(rc,0xFF00FFFF);
				//	}
				//}
				G.DrawLine3D(m_vObjectLastPos,m_vBeforeCatchPos,0xFF00FFFF);

				G.drawCross3D(m_vAfterCatchPos,m_fGridSize,0xFF00FFFF);

				//{
					Pos2D posBeforeCatchPos;
					R.world2Screen(m_vBeforeCatchPos,posBeforeCatchPos);
					CRect<float> rcBeforeCatch(posBeforeCatchPos.x,posBeforeCatchPos.y,posBeforeCatchPos.x,posBeforeCatchPos.y);
				//	rcBeforeCatch.InflateRect(2,2);
				//	G.FillRect(rcBeforeCatch,0xFF00FFFF);
					rcBeforeCatch.InflateRect(4,4);
					G.DrawRect(rcBeforeCatch,0xFF00FFFF);
					R.finishMaterial();
				//}
			}
		}
	}

	if (bBloom||bCamma)
	{
		//m_SceneEffect.RenderTemporalBloom();
		//m_SceneEffect.RenderBloom();
		if(bCamma)
		{
			m_SceneEffect.renderGammaCorrection();
		}
		if(bBloom)
		{
			m_SceneEffect.renderTargetBloom();
		}
		m_SceneEffect.renderTargetEnd();
		m_SceneEffect.compose(rcViewport);
		R.setViewport(rcViewport);
	}
	if (0)
	{
		m_SceneEffect.glowRenderTargetBegin();
		R.ClearBuffer(false, true, 0x00000000);
		//m_Scene.renderGlow();
		m_SceneEffect.glowRenderTargetEnd();
		m_SceneEffect.RenderBloom();
	}

	if (0)
	{
		m_SceneEffect.renderTargetBegin();
		R.ClearBuffer(false, true, 0x00000000);
		//m_SceneEffect.renderTargetViewportBegin();
		{
			R.setProjectionMatrix(m_Camera.GetProjMatrix());
			R.setViewMatrix(m_Camera.GetViewMatrix());
			//m_Scene.renderGlow();
		}
		//m_SceneEffect.renderTargetViewportEnd();
		//m_SceneEffect.renderTargetGlow();
		m_SceneEffect.renderTargetBloom();
		m_SceneEffect.renderTargetEnd();
		//m_SceneEffect.compose();
	}
	//
	//if (GetConfig().GetSceneEffectEnable()&&1)
	//{
	//	if (GetConfig().GetSceneBloomEnable()&&0)
	//	{
	//		GetSceneEffect().RenderBloom();
	//	}
	//	if (GetConfig().GetSceneTBloomEnable())
	//	{
	//		GetSceneEffect().RenderTemporalBloom();
	//	}
	//}

	R.SetupRenderState();
	R.setViewport(GetParentDialog()->GetBoundingBox());

	//TwDraw();
	CUIDisplay::OnFrameRender(fTime,fElapsedTime);
	return;
}

bool CUIWorldEditorDisplay::HandleKeyboard(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(!m_bEnabled || !m_bVisible)
		return false;

	switch(uMsg)
	{
	case WM_KEYDOWN:
		{
			switch(wParam)
			{
			case VK_SHIFT:
				{
					float fStrength = m_Terrain.GetBrushDecal().GetStrength();
					fStrength = -abs(fStrength);
					m_Terrain.GetBrushDecal().SetStrength(fStrength);
					SetCapture(UIGetHWND());
				}
				return true;
			case VK_CONTROL:
				m_bKeyCtrl=true;
				SetPressed(true);
				SetCapture(UIGetHWND());
				return true;
			case 'W':
				m_bKeyUp=true;
				SetPressed(true);
				SetCapture(UIGetHWND());
				return true;
			case 'S':
				m_bKeyDown=true;
				SetPressed(true);
				SetCapture(UIGetHWND());
				return true;
			case 'A':
				m_bKeyLeft=true;
				SetPressed(true);
				SetCapture(UIGetHWND());
				return true;
			case 'D':
				m_bKeyRight=true;
				SetPressed(true);
				SetCapture(UIGetHWND());
				return true;
			case VK_DELETE:
				m_Scene.delMapObjsByFocusObjects();
				return true;
			case VK_SUBTRACT:
				m_fCoordScale=max(0.0f,m_fCoordScale-0.05f);
				return true;
			case VK_ADD:
				m_fCoordScale=min(0.5f,m_fCoordScale+0.05f);
				return true;
			case 'Z':
				if(m_bKeyCtrl)
				{
					m_Terrain.rebackEdit();
				}
				return true;
			case 'Y':
				if(m_bKeyCtrl)
				{
					m_Terrain.redoEdit();
				}
				return true;
			}

			//////////////////////////////////////////////////////////////////////////
			if (m_Terrain.GetBrushDecal().GetBrushType()==CTerrainBrush::BRUSH_TYPE_SCENE_OBJECT)
			{
				if (m_Scene.getFocusObjects().size()>0)
				{
					if (m_bKeyCtrl)
					{
						switch(wParam)
						{
						case VK_UP:
							{
								Vec3D vPos=m_Scene.getFocusObjectsPos();
								vPos.y+=m_fGridSize;
								m_Scene.setFocusObjectsPos(vPos);
								GetParentDialog()->postMsg(USER_DEFINED_MSG_TYPE_OBJECT_POS_CHANGED);
							}
							return true;
						case VK_DOWN:
							{
								Vec3D vPos=m_Scene.getFocusObjectsPos();
								vPos.y-=m_fGridSize;
								m_Scene.setFocusObjectsPos(vPos);
								GetParentDialog()->postMsg(USER_DEFINED_MSG_TYPE_OBJECT_POS_CHANGED);
							}
							return true;
						case VK_LEFT:
							{
								Vec3D vRotate=m_Scene.getFocusObjectsRotate();
								vRotate.y+=PI/4;
								m_Scene.setFocusObjectsRotate(vRotate);
								GetParentDialog()->postMsg(USER_DEFINED_MSG_TYPE_OBJECT_POS_CHANGED);
							}
							return true;
						case VK_RIGHT:
							{
								Vec3D vRotate=m_Scene.getFocusObjectsRotate();
								vRotate.y-=PI/4;
								m_Scene.setFocusObjectsRotate(vRotate);
								GetParentDialog()->postMsg(USER_DEFINED_MSG_TYPE_OBJECT_POS_CHANGED);
							}
							return true;
						}
					}
					else
					{
						switch(wParam)
						{
						case VK_UP:
							{
								Vec3D vPos=m_Scene.getFocusObjectsPos();
								vPos.z+=m_fGridSize;
								m_Scene.setFocusObjectsPos(vPos);
								GetParentDialog()->postMsg(USER_DEFINED_MSG_TYPE_OBJECT_POS_CHANGED);
							}
							return true;
						case VK_DOWN:
							{
								Vec3D vPos=m_Scene.getFocusObjectsPos();
								vPos.z-=m_fGridSize;
								m_Scene.setFocusObjectsPos(vPos);
								GetParentDialog()->postMsg(USER_DEFINED_MSG_TYPE_OBJECT_POS_CHANGED);
							}
							return true;
						case VK_LEFT:
							{
								Vec3D vPos=m_Scene.getFocusObjectsPos();
								vPos.x-=m_fGridSize;
								m_Scene.setFocusObjectsPos(vPos);
								GetParentDialog()->postMsg(USER_DEFINED_MSG_TYPE_OBJECT_POS_CHANGED);
							}
							return true;
						case VK_RIGHT:
							{
								Vec3D vPos=m_Scene.getFocusObjectsPos();
								vPos.x+=m_fGridSize;
								m_Scene.setFocusObjectsPos(vPos);
								GetParentDialog()->postMsg(USER_DEFINED_MSG_TYPE_OBJECT_POS_CHANGED);
							}
							return true;
						}
					}
					return true;
				}
			}
		}
	case WM_KEYUP:
		{
			switch(wParam)
			{
			case VK_SHIFT:
				{
					float fStrength = m_Terrain.GetBrushDecal().GetStrength();
					fStrength = abs(fStrength);
					m_Terrain.GetBrushDecal().SetStrength(fStrength);
				}
				return true;
			case VK_CONTROL:
				m_bKeyCtrl=false;
				return true;
			case 'W':
				m_bKeyUp=false;
				return true;
			case 'S':
				m_bKeyDown=false;
				return true;
			case 'A':
				m_bKeyLeft=false;
				return true;
			case 'D':
				m_bKeyRight=false;
				return true;
			}
		}
	}
	return false;
}

void CUIWorldEditorDisplay::MoveCamera(int x,int y)
{
	Vec3D vPos = m_Camera.getTargetPos();
	// 基于摄像机的 yaw 创建旋转矩阵
	Matrix mCameraRot;
	mCameraRot.rotationYawPitchRoll(m_Camera.getYawAngle(), 0, 0);
	vPos += mCameraRot * Vec3D(x, 0, y)*0.001f*m_Camera.GetRadius();
	vPos.y=m_Terrain.GetData().GetHeight(Vec2D(vPos.x,vPos.z));
	m_Camera.setTargetPos(vPos);
}

void CUIWorldEditorDisplay::OnMouseMove(POINT point)
{
	Vec3D vRayPos, vRayDir;
	Vec3D vTargetPos;
	m_Camera.GetPickRay( vRayPos, vRayDir, point.x, point.y,m_rcBoundingBox);
	m_Terrain.GetData().Pick( vRayPos, vRayDir, &vTargetPos );
	m_Scene.setTargetPos(vTargetPos);
	m_Terrain.GetBrushDecal().SetPos(vTargetPos.x, vTargetPos.z);
	if (IsPressed())
	{
		if (g_bRightButtonDown)
		{
			m_Camera.addMouseDelta(Vec3D(m_ptLastMousePosition.x-point.x,m_ptLastMousePosition.y-point.y,0));
			m_ptLastMousePosition=point;
		}
		else if (g_bMiddleButtonDown)
		{
			MoveCamera(-point.x+m_ptLastMousePosition.x, point.y-m_ptLastMousePosition.y);
			m_ptLastMousePosition=point;
		}
		else if (g_bLeftButtonDown)
		{
			if (m_Terrain.GetBrushDecal().GetBrushType()==CTerrainBrush::BRUSH_TYPE_SCENE_OBJECT)
			{
				if (m_Scene.getFocusObjects().size()>0)
				{
					{
						float t = (m_vObjectLastPos.f[m_CoordPlanType]-vRayPos.f[m_CoordPlanType])/vRayDir.f[m_CoordPlanType];
						Vec3D vMousePos = vRayDir*t+vRayPos;
						m_vBeforeCatchPos = (vMousePos+m_vObjectLastPos-m_vLastMousePos)*m_vPosPressed+m_vObjectLastPos*(Vec3D(1,1,1)-m_vPosPressed);
					}
					m_vAfterCatchPos = m_vBeforeCatchPos;



					if (m_vPosPressed.length()>1)
					{
						//float fGridSize = 0.5f;
						//if (fGridSize<0.1f||fGridSize>=100.f) // check the value is not zero.
						//{
						//	fGridSize=0.5f;
						//	//m_NumGridSize.setFloat(fGridSize);
						//}
						if((m_vAfterCatchPos-m_Scene.getFocusObjectsPos()).length()<m_fGridSize*0.8f)
						{
							m_vAfterCatchPos = m_Scene.getFocusObjectsPos();
						}
						else for (int i=0;i<3;i++)
						{
							float fSize = floorf((m_vAfterCatchPos.f[i]/m_fGridSize+0.5f))*m_fGridSize;
							if (abs(fSize-m_vAfterCatchPos.f[i])>m_fGridSize/3)
							{
								m_vAfterCatchPos.f[i] = fSize;
							}
						}
					}
					// 吸附地表
					if (m_vPosPressed.y==0.0f&&m_Terrain.GetData().GetHeight(Vec2D(m_vObjectLastPos.x,m_vObjectLastPos.z))==m_vObjectLastPos.y)
					{
						m_vAfterCatchPos.y = m_Terrain.GetData().GetHeight(Vec2D(m_vAfterCatchPos.x,m_vAfterCatchPos.z));
					}
					else if(abs(m_Terrain.GetData().GetHeight(Vec2D(m_vAfterCatchPos.x,m_vAfterCatchPos.z))-m_vAfterCatchPos.y)<0.2f)
					{
						m_vAfterCatchPos.y = m_Terrain.GetData().GetHeight(Vec2D(m_vAfterCatchPos.x,m_vAfterCatchPos.z));
					}
					m_Scene.setFocusObjectsPos(m_vAfterCatchPos);
					//GetParentDialog()->postMsg(USER_DEFINED_MSG_TYPE_OBJECT_POS_CHANGED);
				}
			}
			else
			{
				m_Terrain.Brush(vTargetPos.x, vTargetPos.z);
			}


			////if (WM_LBUTTONUP==uMsg)
			//{
			//m_Terrain.AddObject(vTargetPos);
			//}
		}
	}
	else
	{
		if (m_Scene.getFocusObjects().size()>0)
		{
			if(!m_MeshCoordinate.intersect(vRayPos, vRayDir,m_vPosMoveOn))
			{
				m_vPosMoveOn=Vec3D(0,0,0);
			}
		}
	}
}

void CUIWorldEditorDisplay::OnMouseWheel(POINT point,short wheelDelta)
{
	if(ContainsPoint(point))
	{
		if (wheelDelta!=0)
		{
			m_Camera.addMouseDelta(Vec3D(0,0,-wheelDelta/12.0f*m_Camera.GetRadius()));
		}
	}
}

void CUIWorldEditorDisplay::OnLButtonDown(POINT point)
{
	if(ContainsPoint(point))
	{
		m_Terrain.markEdit(); // Mark this edit.
		SetFocus();
		if (m_Terrain.GetBrushDecal().GetBrushType()==CTerrainBrush::BRUSH_TYPE_SCENE_OBJECT)
		{
			Vec3D vRayPos, vRayDir;
			m_Camera.GetPickRay( vRayPos, vRayDir, point.x, point.y,m_rcBoundingBox);
			if (m_bKeyCtrl)
			{
				CMapObj* pObject = m_Scene.pickObject(vRayPos, vRayDir);
				if (m_Scene.findFocusObject(pObject))
				{
					m_Scene.delFocusObject(pObject);
				}
				else
				{
					m_Scene.addFocusObject(pObject);
				}
				GetParentDialog()->postMsg("MSG_FOCUS_OBJECT_CHANGED");
				//Vec3D vPos;
				//if (m_Terrain.GetData().Pick(vRayPos, vRayDir,&vPos))
				//{
				//	GetParentDialog()->postMsg("MSG_ADD_OBJECT");
				//	//CSceneObject* pObject = m_Scene.getFocusObject();
				//	//if (pObject)
				//	//{
				//	//	pObject->setPos(vPos);
				//	//	GetParentDialog()->postMsg("MSG_FOCUS_OBJECT_CHANGED");
				//	//}
				//}
			}
			else
			{
				if (m_Scene.getFocusObjects().size()>0&&m_MeshCoordinate.intersect(vRayPos, vRayDir,m_vPosPressed))
				{
				}
				else
				{
					CMapObj* pObject = m_Scene.pickObject(vRayPos, vRayDir);
					if (m_Scene.findFocusObject(pObject)==false)
					{
						m_Scene.clearFocusObjects();
						m_Scene.addFocusObject(pObject);
						GetParentDialog()->postMsg("MSG_FOCUS_OBJECT_CHANGED");
					}
				}
			}
		}
		else if(m_bKeyCtrl)
		{
			Vec3D vRayPos, vRayDir;
			Vec3D vTargetPos;
			m_Camera.GetPickRay( vRayPos, vRayDir, point.x, point.y,m_rcBoundingBox);
			m_Terrain.GetData().Pick( vRayPos, vRayDir, &vTargetPos );

			Pos2D posCell(vTargetPos.x, vTargetPos.z);
			int nTileID = m_Terrain.GetData().GetCellTileID(posCell,0);
			m_Terrain.GetBrushDecal().SetTileID(nTileID);
		}
	
		SetPressed(true);
		SetCapture(UIGetHWND());
		g_bLeftButtonDown = true;
		if (m_Scene.getFocusObjects().size()>0)
		{
			m_ptLastMousePosition=point;
			m_vObjectLastPos = m_Scene.getFocusObjectsPos();
			{
				m_CoordPlanType = CPT_XY;
				if (m_vPosPressed[0]&&m_vPosPressed[1])
				{
					m_CoordPlanType=CPT_XY;
				}
				else if (m_vPosPressed[1]&&m_vPosPressed[2])
				{
					m_CoordPlanType=CPT_YZ;
				}
				else if (m_vPosPressed[2]&&m_vPosPressed[0])
				{
					m_CoordPlanType=CPT_ZX;
				}
				else
				{
					Matrix mProjXView = m_Camera.GetProjXView();
					Vec3D vOrigin = mProjXView*m_vObjectLastPos;
					Vec3D vX = mProjXView*(m_vObjectLastPos+Vec3D(1,0,0))-vOrigin;
					Vec3D vY = mProjXView*(m_vObjectLastPos+Vec3D(0,1,0))-vOrigin;
					Vec3D vZ = mProjXView*(m_vObjectLastPos+Vec3D(0,0,1))-vOrigin;
					vX.z=0;vY.z=0;vZ.z=0;
					vX.normalize();vY.normalize();vZ.normalize();
					float fDotXY = vX.dot(vY);
					float fDotYZ = vY.dot(vZ);
					float fDotZX = vZ.dot(vX);

					if (m_vPosPressed[0])
					{
						if (abs(fDotXY)<abs(fDotZX))
						{
							m_CoordPlanType=CPT_XY;
						}
						else
						{
							m_CoordPlanType=CPT_ZX;
						}
					}
					else if (m_vPosPressed[1])
					{
						if (abs(fDotXY)<abs(fDotYZ))
						{
							m_CoordPlanType=CPT_XY;
						}
						else
						{
							m_CoordPlanType=CPT_YZ;
						}
					}
					else if (m_vPosPressed[2])
					{
						if (abs(fDotYZ)<abs(fDotZX))
						{
							m_CoordPlanType=CPT_YZ;
						}
						else
						{
							m_CoordPlanType=CPT_ZX;
						}
					}
				}
			}
			{
				Vec3D vLastMouseRayPos, vLastMouseRayDir;
				m_Camera.GetPickRay(vLastMouseRayPos, vLastMouseRayDir, m_ptLastMousePosition.x, m_ptLastMousePosition.y,m_rcBoundingBox);
				float t = (m_vObjectLastPos.f[m_CoordPlanType]-vLastMouseRayPos.f[m_CoordPlanType])/vLastMouseRayDir.f[m_CoordPlanType];
				m_vLastMousePos = vLastMouseRayDir*t+vLastMouseRayPos;
			}

		}
	}


}

void CUIWorldEditorDisplay::OnLButtonUp(POINT point)
{
	g_bLeftButtonDown = false;
	if (IsPressed())
	{
		ReleaseCapture();
	}
}

void CUIWorldEditorDisplay::OnRButtonDown(POINT point)
{
	if(ContainsPoint(point))
	{
		SetPressed(true);
		SetCapture(UIGetHWND());
		m_ptLastMousePosition=point;
		g_bRightButtonDown = true;
	}
}

void CUIWorldEditorDisplay::OnRButtonUp(POINT point)
{
	g_bRightButtonDown = false;
	if (IsPressed())
	{
		ReleaseCapture();
	}
}

void CUIWorldEditorDisplay::OnMButtonDown(POINT point)
{
	if(ContainsPoint(point))
	{
		SetPressed(true);
		SetCapture(UIGetHWND());
		m_ptLastMousePosition=point;
		g_bMiddleButtonDown=true;
	}

}

void CUIWorldEditorDisplay::OnMButtonUp(POINT point)
{
	g_bMiddleButtonDown=false;
	if (IsPressed())
	{
		ReleaseCapture();
	}
}

void CUIWorldEditorDisplay::OnFocusOut()
{
	g_bLeftButtonDown = false;
	g_bRightButtonDown = false;
	g_bMiddleButtonDown = false;
	m_bKeyCtrl = false;
	m_bKeyUp = false;
	m_bKeyDown = false;
	m_bKeyLeft = false;
	m_bKeyRight = false;
	if (IsPressed())
	{
		ReleaseCapture();
	}
}

bool CUIWorldEditorDisplay::HandleMouse(UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam)
{
	if (WM_CAPTURECHANGED==uMsg)
	{
		if((HWND)lParam != UIGetHWND())
		{
			if (IsPressed())
			{
				ReleaseCapture();
			}
		}
	}
	return true;
}

void CUIWorldEditorDisplay::OnSize(const CRect<int>& rc)
{
	CUIDisplay::OnSize(rc);
	m_SceneEffect.Reset(GetBoundingBox());
}