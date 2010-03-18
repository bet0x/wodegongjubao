#include "DlgModelRenderPass.h"
#include "ModelDisplay.h"

CDlgModelRenderPass::CDlgModelRenderPass()
{
}

void CDlgModelRenderPass::OnControlRegister()
{
	RegisterControl("IDC_LISTBOX_RENDERPASS",	m_ListboxRenderPass);
	m_DlgModelMaterial.Create("IDD_MODEL_MATERIAL", this->GetParentDialog()->GetParentDialog());
	RegisterControlEvent("IDC_LISTBOX_RENDERPASS",(PEVENT)&CDlgModelRenderPass::OnListboxRenderPass);
}

void CDlgModelRenderPass::SetVisible(bool bVisible)
{
	CDlgModelBaseDialog::SetVisible(bVisible);
	if (bVisible==false)
	{
		m_DlgModelMaterial.SetVisible(false);
	}
}

void CDlgModelRenderPass::onReset()
{
	int nSelected = m_ListboxRenderPass.GetSelectedIndex();
	m_ListboxRenderPass.RemoveAllItems();
	if (getModelDisplay().getModelData())
	{
		size_t uCount = getModelDisplay().getModelData()->m_mapPasses.size();
		if (uCount>0)
		{
			for (size_t i=0; i<uCount; ++i)
			{
				m_ListboxRenderPass.AddItem(FormatW(L"[%d]",i));
			}
			if (nSelected>=uCount)
			{
				nSelected = uCount-1;
			}
			m_ListboxRenderPass.SelectItem(nSelected);
		}
		if (m_DlgModelMaterial.IsVisible())
		{
			OnListboxRenderPass();
		}
	}
}

CMaterial* CDlgModelRenderPass::getSelectedRenderPass()
{
	int nSelected = m_ListboxRenderPass.GetSelectedIndex();
	if (getModelDisplay().getModelData())
	{
		ModelRenderPass& renderPass = getModelDisplay().getModelData()->m_mapPasses[nSelected];
		CMaterial& material = GetRenderSystem().getMaterialMgr().getItem(renderPass.strMaterialName);
		return &material;
	}
	return NULL;
}

void CDlgModelRenderPass::OnListboxRenderPass()
{
	CMaterial* pMaterial=getSelectedRenderPass();
	m_DlgModelMaterial.setMaterial(pMaterial);
	m_DlgModelMaterial.SetVisible(true);
}