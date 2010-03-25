#include "DlgModelMaterial.h"
#include "ModelDisplay.h"

CDlgModelMaterial::CDlgModelMaterial():m_pSelectedMaterial(NULL)
{
}

void CDlgModelMaterial::OnControlRegister()
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
	RegisterControl("IDC_LISTBOX_MATERIAL",	m_ListboxMaterial);

	RegisterControlEvent("IDC_EDITBOX_DIFFUSE",		(PEVENT)&CDlgModelMaterial::OnEditboxDiffuse);
	RegisterControlEvent("IDC_EDITBOX_EMISSIVE",	(PEVENT)&CDlgModelMaterial::OnEditboxEmissive);
	RegisterControlEvent("IDC_EDITBOX_SPECULAR",	(PEVENT)&CDlgModelMaterial::OnEditboxSpecular);
	RegisterControlEvent("IDC_EDITBOX_BUMP",		(PEVENT)&CDlgModelMaterial::OnEditboxBump);
	RegisterControlEvent("IDC_EDITBOX_REFLECTION",	(PEVENT)&CDlgModelMaterial::OnEditboxReflection);
	RegisterControlEvent("IDC_EDITBOX_LIGHTMAP",	(PEVENT)&CDlgModelMaterial::OnEditboxLightmap);
	RegisterControlEvent("IDC_EDITBOX_EFFECT",		(PEVENT)&CDlgModelMaterial::OnEditboxEffect);
	RegisterControlEvent("IDC_CHECKBOX_ALPHATEST",	(PEVENT)&CDlgModelMaterial::OnCheckboxAlphatest);
	RegisterControlEvent("IDC_NUM_ALPHATESTVALUE",	(PEVENT)&CDlgModelMaterial::OnNumAlphatestvalue);
	RegisterControlEvent("IDC_CHECKBOX_BLEND",	(PEVENT)&CDlgModelMaterial::OnCheckboxBlend);
	RegisterControlEvent("IDC_CHECKBOX_CULL",	(PEVENT)&CDlgModelMaterial::OnCheckboxCull);
	RegisterControlEvent("IDC_NUM_TEXANIM_X",	(PEVENT)&CDlgModelMaterial::OnNumTexanimX);
	RegisterControlEvent("IDC_NUM_TEXANIM_Y",	(PEVENT)&CDlgModelMaterial::OnNumTexanimY);
	RegisterControlEvent("IDC_NUM_OPACITY",		(PEVENT)&CDlgModelMaterial::OnNumOpacity);
	RegisterControlEvent("IDC_COLOR_EMISSIVE",	(PEVENT)&CDlgModelMaterial::OnColorEmissive);
	RegisterControlEvent("IDC_NUM_UVSCALE_X",	(PEVENT)&CDlgModelMaterial::OnNumUvscaleX);
	RegisterControlEvent("IDC_NUM_UVSCALE_Y",	(PEVENT)&CDlgModelMaterial::OnNumUvscaleY);
	RegisterControlEvent("IDC_BTN_CLOSE",	(PEVENT)&CDlgModelMaterial::OnBtnClose);
}

void CDlgModelMaterial::setMaterial(const std::string& strMaterial)
{
	m_pSelectedMaterial = &GetRenderSystem().getMaterialMgr().getItem(strMaterial);
	if (m_pSelectedMaterial)
	{
		CTextureMgr& TM=GetRenderSystem().GetTextureMgr();
		CShaderMgr& SM=GetRenderSystem().GetShaderMgr();
		std::string strFilename = GetParentPath(getModelDisplay().getModelObject()->getModelFilename());

		m_EditboxDiffuse.	SetText(s2ws(getSimpleFilename(strFilename,TM.getItemName(m_pSelectedMaterial->uDiffuse))));
		m_EditboxEmissive.	SetText(s2ws(getSimpleFilename(strFilename,TM.getItemName(m_pSelectedMaterial->uEmissive))));
		m_EditboxSpecular.	SetText(s2ws(getSimpleFilename(strFilename,TM.getItemName(m_pSelectedMaterial->uSpecular))));
		m_EditboxBump.		SetText(s2ws(getSimpleFilename(strFilename,TM.getItemName(m_pSelectedMaterial->uNormal))));
		m_EditboxReflection.SetText(s2ws(getSimpleFilename(strFilename,TM.getItemName(m_pSelectedMaterial->uReflection))));
		m_EditboxLightmap.	SetText(s2ws(getSimpleFilename(strFilename,TM.getItemName(m_pSelectedMaterial->uLightMap))));
		m_EditboxEffect.	SetText(s2ws(getSimpleFilename(strFilename,SM.getItemName(m_pSelectedMaterial->uShader))));
		m_CheckboxAlphatest.SetChecked(m_pSelectedMaterial->bAlphaTest);
		m_NumAlphatestvalue.setFloat(m_pSelectedMaterial->uAlphaTestValue);
		m_CheckboxBlend.SetChecked(m_pSelectedMaterial->bBlend);
		m_CheckboxCull.SetChecked(m_pSelectedMaterial->uCull);
		m_NumTexanimX.setFloat(m_pSelectedMaterial->vTexAnim.x);
		m_NumTexanimY.setFloat(m_pSelectedMaterial->vTexAnim.y);
		m_NumOpacity.setFloat(m_pSelectedMaterial->m_fOpacity);
		m_ColorEmissive.setColor(m_pSelectedMaterial->cEmissive);
		m_NumUvscaleX.setFloat(m_pSelectedMaterial->vUVScale.x);
		m_NumUvscaleY.setFloat(m_pSelectedMaterial->vUVScale.y);
	}
}

std::string CDlgModelMaterial::getEditBoxFilename(const CUIEditBox& editBox)
{
	return getRealFilename(GetParentPath(getModelDisplay().getModelObject()->getModelFilename()),ws2s(editBox.GetText()));
}

unsigned int CDlgModelMaterial::getTextureID(const CUIEditBox& editBox)
{
	return GetRenderSystem().GetTextureMgr().RegisterTexture(getEditBoxFilename(editBox));
}

void CDlgModelMaterial::OnEditboxDiffuse()
{
	if (m_pSelectedMaterial==NULL){return;}
	m_pSelectedMaterial->uDiffuse = getTextureID(m_EditboxDiffuse);
}

void CDlgModelMaterial::OnEditboxEmissive()
{
	if (m_pSelectedMaterial==NULL){return;}
	m_pSelectedMaterial->uEmissive = getTextureID(m_EditboxEmissive);
}

void CDlgModelMaterial::OnEditboxSpecular()
{
	if (m_pSelectedMaterial==NULL){return;}
	m_pSelectedMaterial->uSpecular = getTextureID(m_EditboxSpecular);
}

void CDlgModelMaterial::OnEditboxBump()
{
	if (m_pSelectedMaterial==NULL){return;}
	m_pSelectedMaterial->uNormal = getTextureID(m_EditboxBump);
}

void CDlgModelMaterial::OnEditboxReflection()
{
	if (m_pSelectedMaterial==NULL){return;}
	m_pSelectedMaterial->uReflection = getTextureID(m_EditboxReflection);
}

void CDlgModelMaterial::OnEditboxLightmap()
{
	if (m_pSelectedMaterial==NULL){return;}
	m_pSelectedMaterial->uLightMap = getTextureID(m_EditboxLightmap);
}

void CDlgModelMaterial::OnEditboxEffect()
{
	if (m_pSelectedMaterial==NULL){return;}
	CShaderMgr& SM=GetRenderSystem().GetShaderMgr();
	SM.del(m_pSelectedMaterial->uShader);
	std::string strFilename = getEditBoxFilename(m_EditboxEffect);
	m_pSelectedMaterial->uShader = SM.registerItem(strFilename);
}

void CDlgModelMaterial::OnCheckboxAlphatest()
{
	if (m_pSelectedMaterial==NULL){return;}
	m_pSelectedMaterial->bAlphaTest = m_CheckboxAlphatest.IsChecked();
}

void CDlgModelMaterial::OnNumAlphatestvalue()
{
	if (m_pSelectedMaterial==NULL){return;}
	m_pSelectedMaterial->uAlphaTestValue = m_NumAlphatestvalue.getFloat();
}

void CDlgModelMaterial::OnCheckboxBlend()
{
	if (m_pSelectedMaterial==NULL){return;}
	m_pSelectedMaterial->bBlend = m_CheckboxBlend.IsChecked();
}

void CDlgModelMaterial::OnCheckboxCull()
{
	if (m_pSelectedMaterial==NULL){return;}
	m_pSelectedMaterial->uCull = m_CheckboxCull.IsChecked();
}

void CDlgModelMaterial::OnNumTexanimX()
{
	if (m_pSelectedMaterial==NULL){return;}
	m_pSelectedMaterial->vTexAnim.x = m_NumTexanimX.getFloat();
}

void CDlgModelMaterial::OnNumTexanimY()
{
	if (m_pSelectedMaterial==NULL){return;}
	m_pSelectedMaterial->vTexAnim.y = m_NumTexanimY.getFloat();
}

void CDlgModelMaterial::OnNumOpacity()
{
	if (m_pSelectedMaterial==NULL){return;}
	m_pSelectedMaterial->m_fOpacity = m_NumOpacity.getFloat();
}

void CDlgModelMaterial::OnColorEmissive()
{
	if (m_pSelectedMaterial==NULL){return;}
	m_pSelectedMaterial->cEmissive = m_ColorEmissive.getColor();
}

void CDlgModelMaterial::OnNumUvscaleX()
{
	if (m_pSelectedMaterial==NULL){return;}
	m_pSelectedMaterial->vUVScale.x = m_NumUvscaleX.getFloat();
}

void CDlgModelMaterial::OnNumUvscaleY()
{
	if (m_pSelectedMaterial==NULL){return;}
	m_pSelectedMaterial->vUVScale.y = m_NumUvscaleY.getFloat();
}

void CDlgModelMaterial::OnBtnClose()
{
	SetVisible(false);
}