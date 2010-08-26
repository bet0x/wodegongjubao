#include "DlgMaterial.h"
#include "FileSystem.h"

CDlgMaterial::CDlgMaterial():m_pSelectedMaterial(NULL)
{
}

void CDlgMaterial::OnControlRegister()
{
	RegisterControl("IDC_EDITBOX_DIFFUSE",	m_EditboxDiffuse);
	RegisterControl("IDC_EDITBOX_EMISSIVE",	m_EditboxEmissive);
	RegisterControl("IDC_EDITBOX_SPECULAR",	m_EditboxSpecular);
	RegisterControl("IDC_EDITBOX_BUMP",		m_EditboxBump);
	RegisterControl("IDC_EDITBOX_REFLECTION",	m_EditboxReflection);
	RegisterControl("IDC_EDITBOX_LIGHTMAP",		m_EditboxLightmap);
	RegisterControl("IDC_EDITBOX_EFFECT",		m_EditboxEffect);
	RegisterControl("IDC_CHECKBOX_ALPHATEST",	m_CheckboxAlphatest);
	RegisterControl("IDC_NUM_ALPHATESTVALUE",	m_NumAlphatestvalue);
	RegisterControl("IDC_CHECKBOX_BLEND",	m_CheckboxBlend);
	RegisterControl("IDC_CHECKBOX_CULL",	m_CheckboxCull);
	RegisterControl("IDC_NUM_TEXANIM_X",	m_NumTexanimX);
	RegisterControl("IDC_NUM_TEXANIM_Y",	m_NumTexanimY);
	RegisterControl("IDC_NUM_OPACITY",		m_NumOpacity);
	RegisterControl("IDC_COLOR_EMISSIVE",	m_ColorEmissive);
	RegisterControl("IDC_NUM_UVSCALE_X",	m_NumUvscaleX);
	RegisterControl("IDC_NUM_UVSCALE_Y",	m_NumUvscaleY);

	RegisterControlEvent("IDC_EDITBOX_DIFFUSE",		(PEVENT)&CDlgMaterial::OnEditboxDiffuse);
	RegisterControlEvent("IDC_EDITBOX_EMISSIVE",	(PEVENT)&CDlgMaterial::OnEditboxEmissive);
	RegisterControlEvent("IDC_EDITBOX_SPECULAR",	(PEVENT)&CDlgMaterial::OnEditboxSpecular);
	RegisterControlEvent("IDC_EDITBOX_BUMP",		(PEVENT)&CDlgMaterial::OnEditboxBump);
	RegisterControlEvent("IDC_EDITBOX_REFLECTION",	(PEVENT)&CDlgMaterial::OnEditboxReflection);
	RegisterControlEvent("IDC_EDITBOX_LIGHTMAP",	(PEVENT)&CDlgMaterial::OnEditboxLightmap);
	RegisterControlEvent("IDC_EDITBOX_EFFECT",		(PEVENT)&CDlgMaterial::OnEditboxEffect);
	RegisterControlEvent("IDC_CHECKBOX_ALPHATEST",	(PEVENT)&CDlgMaterial::OnCheckboxAlphatest);
	RegisterControlEvent("IDC_NUM_ALPHATESTVALUE",	(PEVENT)&CDlgMaterial::OnNumAlphatestvalue);
	RegisterControlEvent("IDC_CHECKBOX_BLEND",	(PEVENT)&CDlgMaterial::OnCheckboxBlend);
	RegisterControlEvent("IDC_CHECKBOX_CULL",	(PEVENT)&CDlgMaterial::OnCheckboxCull);
	RegisterControlEvent("IDC_NUM_TEXANIM_X",	(PEVENT)&CDlgMaterial::OnNumTexanimX);
	RegisterControlEvent("IDC_NUM_TEXANIM_Y",	(PEVENT)&CDlgMaterial::OnNumTexanimY);
	RegisterControlEvent("IDC_NUM_OPACITY",		(PEVENT)&CDlgMaterial::OnNumOpacity);
	RegisterControlEvent("IDC_COLOR_EMISSIVE",	(PEVENT)&CDlgMaterial::OnColorEmissive);
	RegisterControlEvent("IDC_NUM_UVSCALE_X",	(PEVENT)&CDlgMaterial::OnNumUvscaleX);
	RegisterControlEvent("IDC_NUM_UVSCALE_Y",	(PEVENT)&CDlgMaterial::OnNumUvscaleY);
	RegisterControlEvent("IDC_BTN_CLOSE",	(PEVENT)&CDlgMaterial::OnBtnClose);
}

void CDlgMaterial::setMaterial(const std::string& strMaterial, const std::string& strPath)
{
	m_pSelectedMaterial = &GetRenderSystem().getMaterialMgr().getItem(strMaterial);
	if (m_pSelectedMaterial)
	{
		CTextureMgr& TM=GetRenderSystem().GetTextureMgr();
		CShaderMgr& SM=GetRenderSystem().GetShaderMgr();
		m_strPath = strPath;

		m_EditboxDiffuse.	SetText(s2ws(getSimpleFilename(strPath,TM.getItemName(m_pSelectedMaterial->uDiffuse))).c_str());
		m_EditboxEmissive.	SetText(s2ws(getSimpleFilename(strPath,TM.getItemName(m_pSelectedMaterial->uEmissive))).c_str());
		m_EditboxSpecular.	SetText(s2ws(getSimpleFilename(strPath,TM.getItemName(m_pSelectedMaterial->uSpecular))).c_str());
		m_EditboxBump.		SetText(s2ws(getSimpleFilename(strPath,TM.getItemName(m_pSelectedMaterial->uNormal))).c_str());
		m_EditboxReflection.SetText(s2ws(getSimpleFilename(strPath,TM.getItemName(m_pSelectedMaterial->uReflection))).c_str());
		m_EditboxLightmap.	SetText(s2ws(getSimpleFilename(strPath,TM.getItemName(m_pSelectedMaterial->uLightMap))).c_str());
		m_EditboxEffect.	SetText(s2ws(getSimpleFilename(strPath,SM.getItemName(m_pSelectedMaterial->uShader))).c_str());
		m_CheckboxAlphatest.SetChecked(m_pSelectedMaterial->bAlphaTest);
		m_NumAlphatestvalue.setFloat(m_pSelectedMaterial->uAlphaTestValue,0,2);
		m_CheckboxBlend.SetChecked(m_pSelectedMaterial->bBlend);
		m_CheckboxCull.SetChecked(m_pSelectedMaterial->uCull);
		m_NumTexanimX.setFloat(m_pSelectedMaterial->vTexAnim.x,0,2);
		m_NumTexanimY.setFloat(m_pSelectedMaterial->vTexAnim.y,0,2);
		m_NumOpacity.setFloat(m_pSelectedMaterial->m_fOpacity,0,2);
		m_ColorEmissive.setColor(m_pSelectedMaterial->cEmissive);
		m_NumUvscaleX.setFloat(m_pSelectedMaterial->vUVScale.x,0,2);
		m_NumUvscaleY.setFloat(m_pSelectedMaterial->vUVScale.y,0,2);
	}
}

std::string CDlgMaterial::getEditBoxFilename(const CUIEditBox& editBox)
{
	return getRealFilename(m_strPath,ws2s(editBox.GetText()));
}

unsigned int CDlgMaterial::getTextureID(const CUIEditBox& editBox)
{
	return GetRenderSystem().GetTextureMgr().RegisterTexture(getEditBoxFilename(editBox));
}

void CDlgMaterial::OnEditboxDiffuse()
{
	if (m_pSelectedMaterial==NULL){return;}
	m_pSelectedMaterial->uDiffuse = getTextureID(m_EditboxDiffuse);
}

void CDlgMaterial::OnEditboxEmissive()
{
	if (m_pSelectedMaterial==NULL){return;}
	m_pSelectedMaterial->uEmissive = getTextureID(m_EditboxEmissive);
}

void CDlgMaterial::OnEditboxSpecular()
{
	if (m_pSelectedMaterial==NULL){return;}
	m_pSelectedMaterial->uSpecular = getTextureID(m_EditboxSpecular);
}

void CDlgMaterial::OnEditboxBump()
{
	if (m_pSelectedMaterial==NULL){return;}
	m_pSelectedMaterial->uNormal = getTextureID(m_EditboxBump);
}

void CDlgMaterial::OnEditboxReflection()
{
	if (m_pSelectedMaterial==NULL){return;}
	m_pSelectedMaterial->uReflection = getTextureID(m_EditboxReflection);
}

void CDlgMaterial::OnEditboxLightmap()
{
	if (m_pSelectedMaterial==NULL){return;}
	m_pSelectedMaterial->uLightMap = getTextureID(m_EditboxLightmap);
}

void CDlgMaterial::OnEditboxEffect()
{
	if (m_pSelectedMaterial==NULL){return;}
	CShaderMgr& SM=GetRenderSystem().GetShaderMgr();
	SM.del(m_pSelectedMaterial->uShader);
	std::string strFilename = getEditBoxFilename(m_EditboxEffect);
	m_pSelectedMaterial->uShader = SM.registerItem(strFilename);
}

void CDlgMaterial::OnCheckboxAlphatest()
{
	if (m_pSelectedMaterial==NULL){return;}
	m_pSelectedMaterial->bAlphaTest = m_CheckboxAlphatest.IsChecked();
}

void CDlgMaterial::OnNumAlphatestvalue()
{
	if (m_pSelectedMaterial==NULL){return;}
	m_pSelectedMaterial->uAlphaTestValue = m_NumAlphatestvalue.getFloat();
}

void CDlgMaterial::OnCheckboxBlend()
{
	if (m_pSelectedMaterial==NULL){return;}
	m_pSelectedMaterial->bBlend = m_CheckboxBlend.IsChecked();
}

void CDlgMaterial::OnCheckboxCull()
{
	if (m_pSelectedMaterial==NULL){return;}
	m_pSelectedMaterial->uCull = m_CheckboxCull.IsChecked();
}

void CDlgMaterial::OnNumTexanimX()
{
	if (m_pSelectedMaterial==NULL){return;}
	m_pSelectedMaterial->vTexAnim.x = m_NumTexanimX.getFloat();
}

void CDlgMaterial::OnNumTexanimY()
{
	if (m_pSelectedMaterial==NULL){return;}
	m_pSelectedMaterial->vTexAnim.y = m_NumTexanimY.getFloat();
}

void CDlgMaterial::OnNumOpacity()
{
	if (m_pSelectedMaterial==NULL){return;}
	m_pSelectedMaterial->m_fOpacity = m_NumOpacity.getFloat();
}

void CDlgMaterial::OnColorEmissive()
{
	if (m_pSelectedMaterial==NULL){return;}
	m_pSelectedMaterial->cEmissive = m_ColorEmissive.getColor();
}

void CDlgMaterial::OnNumUvscaleX()
{
	if (m_pSelectedMaterial==NULL){return;}
	m_pSelectedMaterial->vUVScale.x = m_NumUvscaleX.getFloat();
}

void CDlgMaterial::OnNumUvscaleY()
{
	if (m_pSelectedMaterial==NULL){return;}
	m_pSelectedMaterial->vUVScale.y = m_NumUvscaleY.getFloat();
}

void CDlgMaterial::OnBtnClose()
{
	SetVisible(false);
}