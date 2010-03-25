#include "DlgModelRenderPass.h"
#include "ModelDisplay.h"

CDlgModelRenderPass::CDlgModelRenderPass()
{
}

void CDlgModelRenderPass::OnControlRegister()
{
	RegisterControl("IDC_LISTBOX_RENDERPASS",	m_ListboxRenderPass);
	RegisterControlEvent("IDC_LISTBOX_RENDERPASS",(PEVENT)&CDlgModelRenderPass::OnListboxRenderPass);
}

void CDlgModelRenderPass::SetVisible(bool bVisible)
{
	CDlgModelBaseDialog::SetVisible(bVisible);
	
	if (bVisible==false)
	{
		getMainDialog().getMaterialDialog().SetVisible(false);
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
		if (getMainDialog().getMaterialDialog().IsVisible())
		{
			OnListboxRenderPass();
		}
	}
}

std::string CDlgModelRenderPass::getSelectedRenderPass()
{
	int nSelected = m_ListboxRenderPass.GetSelectedIndex();
	if (getModelDisplay().getModelData())
	{
		ModelRenderPass& renderPass = getModelDisplay().getModelData()->m_mapPasses[nSelected];
		return renderPass.strMaterialName;
	}
	return "";
}

void CDlgModelRenderPass::OnListboxRenderPass()
{
	getMainDialog().getMaterialDialog().setMaterial(getSelectedRenderPass());
	getMainDialog().getMaterialDialog().SetVisible(true);
}