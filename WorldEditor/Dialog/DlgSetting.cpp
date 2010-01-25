#include "DlgSetting.h"
#include "DlgToolbar.h"

CDlgSetting::CDlgSetting()
{
}

CDlgSetting::~CDlgSetting()
{	
}

void CDlgSetting::OnControlRegister()
{
	CUIDialog::OnControlRegister();

	RegisterControl("IDC_EDITBOX_TERRAIN_WIDTH", m_EditBoxWidth);
	RegisterControl("IDC_EDITBOX_TERRAIN_HEIGHT", m_EditBoxHeight);
	RegisterControl("IDC_CHECKBOX_ANIM_OBJECT", m_CheckBoxAnimObject);

	RegisterControl("IDC_NUM_CAMERA_ANGLE",	m_NumCameraAngle);
	RegisterControl("IDC_NUM_CAMERA_FAR",	m_NumCameraFar);
	RegisterControl("IDC_NUM_FOG_START",	m_NumFogStart);
	RegisterControl("IDC_NUM_FOG_End",		m_NumFogEnd);
	RegisterControl("IDC_NUM_FOG_DENSITY",	m_NumFogDensity);

	RegisterControl("IDC_COLOR_FOG",		m_ColorFog);
	RegisterControl("IDC_COLOR_AMBIENT",	m_ColorAmbient);
	RegisterControl("IDC_COLOR_DIFFUSE",	m_ColorDiffuse);


	RegisterControlEvent("IDC_CHECKBOX_TERRAIN_LAYER1",	(PEVENT)&CDlgSetting::OnBtnTerrainLayer0Visible);
	RegisterControlEvent("IDC_CHECKBOX_TERRAIN_LAYER2",	(PEVENT)&CDlgSetting::OnBtnTerrainLayer1Visible);
	RegisterControlEvent("IDC_CHECKBOX_TERRAIN_GRID",	(PEVENT)&CDlgSetting::OnBtnTerrainGridVisible);
	RegisterControlEvent("IDC_CHECKBOX_TERRAIN_BBOX",	(PEVENT)&CDlgSetting::OnBtnTerrainBBoxVisible);
	RegisterControlEvent("IDC_CHECKBOX_STATIC_OBJECT",	(PEVENT)&CDlgSetting::OnBtnStaticObjectVisible);
	RegisterControlEvent("IDC_CHECKBOX_ANIM_OBJECT",	(PEVENT)&CDlgSetting::OnBtnAnimObjectVisible);
	RegisterControlEvent("IDC_CHECKBOX_OBJECT_BBOX",	(PEVENT)&CDlgSetting::OnBtnObjectBBoxVisible);

	RegisterControlEvent("IDC_BTN_TERRAIN_RESIZE",		(PEVENT)&CDlgSetting::OnBtnTerrainResize);

	RegisterControlEvent("IDC_BTN_LIGHTMAP",			(PEVENT)&CDlgSetting::OnBtnCalcLightMap);

	RegisterControlEvent("IDC_NUM_CAMERA_ANGLE",	(PEVENT)&CDlgSetting::OnCameraChanged);
	RegisterControlEvent("IDC_NUM_CAMERA_FAR",		(PEVENT)&CDlgSetting::OnCameraChanged);
	RegisterControlEvent("IDC_NUM_FOG_START",		(PEVENT)&CDlgSetting::OnFogChanged);
	RegisterControlEvent("IDC_NUM_FOG_End",			(PEVENT)&CDlgSetting::OnFogChanged);
	RegisterControlEvent("IDC_NUM_FOG_DENSITY",		(PEVENT)&CDlgSetting::OnFogChanged);
	RegisterControlEvent("IDC_COLOR_FOG",			(PEVENT)&CDlgSetting::OnFogChanged);
	RegisterControlEvent("IDC_COLOR_AMBIENT",		(PEVENT)&CDlgSetting::OnMaterialChanged);
	RegisterControlEvent("IDC_COLOR_DIFFUSE",		(PEVENT)&CDlgSetting::OnMaterialChanged);
}

bool CDlgSetting::OnInitDialog()
{
	//getDisplay().getTerrain().showLayer0(!getDisplay().getTerrain().isShowLayer0());
	//getDisplay().getTerrain().showLayer1(!getDisplay().getTerrain().isShowLayer1());
	//getDisplay().getTerrain().showGrid(!getDisplay().getTerrain().isShowGrid());
	//getDisplay().getTerrain().ShowBox(!getDisplay().getTerrain().IsShowBox());
	return true;
}

void CDlgSetting::SetVisible(bool bVisible)
{
	CUIDialog::SetVisible(bVisible);
	if (bVisible)
	{
		m_EditBoxWidth.SetInt(getDisplay().getTerrain().GetData().GetWidth());
		m_EditBoxHeight.SetInt(getDisplay().getTerrain().GetData().GetHeight());
	}
}

void CDlgSetting::OnBtnTerrainLayer0Visible()
{
	getDisplay().getTerrain().showLayer0(!getDisplay().getTerrain().isShowLayer0());
}

void CDlgSetting::OnBtnTerrainLayer1Visible()
{
	getDisplay().getTerrain().showLayer1(!getDisplay().getTerrain().isShowLayer1());
}

void CDlgSetting::OnBtnTerrainGridVisible()
{
	getDisplay().getTerrain().showGrid(!getDisplay().getTerrain().isShowGrid());
}

void CDlgSetting::OnBtnTerrainBBoxVisible()
{
	getDisplay().getTerrain().ShowBox(!getDisplay().getTerrain().IsShowBox());
}

void CDlgSetting::OnBtnStaticObjectVisible()
{
	getDisplay().getScene().showStaticObject(!getDisplay().getScene().isShowStaticObject());
}

void CDlgSetting::OnBtnAnimObjectVisible()
{
	getDisplay().getScene().showAnimObject(!getDisplay().getScene().isShowAnimObject());
}

void CDlgSetting::OnBtnObjectBBoxVisible()
{
	getDisplay().getScene().showObjectBBox(!getDisplay().getScene().isShowObjectBBox());
}

void CDlgSetting::OnBtnTerrainResize()
{
	getDisplay().getTerrain().resize(m_EditBoxWidth.GetInt(),m_EditBoxHeight.GetInt(),0);
}

void CDlgSetting::OnBtnCalcLightMap()
{
	getDisplay().getScene().CalcLightMap();
}

void CDlgSetting::OnCameraChanged()
{
}

void CDlgSetting::OnFogChanged()
{
	Fog fog;
	fog.fStart=m_NumFogStart.getFloat();
	fog.fEnd=m_NumFogEnd.getFloat();
	fog.fDensity=m_NumFogDensity.getFloat();
	fog.color=m_ColorFog.getColor();
	getDisplay().getScene().setFog(fog);
}

void CDlgSetting::OnMaterialChanged()
{
	DirectionalLight light;
	light.vAmbient = Vec4D(m_ColorAmbient.getColor());
	light.vDiffuse = Vec4D(m_ColorDiffuse.getColor());
	light.vSpecular = Vec4D(1.0f,1.0f,1.0f,1.0f);
	light.vDirection = Vec3D(-1.0f,-1.0f,1.0f);
	getDisplay().getScene().setLight(light);
}

void CDlgSetting::init()
{
	const Fog& fog = getDisplay().getScene().getFog();
	m_NumFogStart.setFloat(fog.fStart);
	m_NumFogEnd.setFloat(fog.fEnd);
	m_NumFogDensity.setFloat(fog.fDensity);
	m_ColorFog.setColor(fog.color);

	const DirectionalLight& light = getDisplay().getScene().getLight();
	m_ColorAmbient.setColor(light.vAmbient);
	m_ColorDiffuse.setColor(light.vDiffuse);
}