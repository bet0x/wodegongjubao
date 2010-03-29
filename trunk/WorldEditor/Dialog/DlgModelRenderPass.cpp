#include "DlgModelRenderPass.h"
#include "ModelDisplay.h"
#include "DlgMainEditor.h"

CDlgModelRenderPass::CDlgModelRenderPass()
{
}

void CDlgModelRenderPass::OnControlRegister()
{
	RegisterControl("IDC_LISTBOX_RENDERPASS",	m_ListboxRenderPass);
	RegisterControl("IDC_CMB_SUB_ID",			m_CmbSubID);
	RegisterControl("IDC_EDITBOX_MATERIAL",		m_EditBoxMaterial);

	RegisterControlEvent("IDC_LISTBOX_RENDERPASS",(PEVENT)&CDlgModelRenderPass::OnListboxRenderPass);
	RegisterControlEvent("IDC_CMB_SUB_ID",(PEVENT)&CDlgModelRenderPass::OnCmbSubIDSelected);
	RegisterControlEvent("IDC_EDITBOX_MATERIAL",(PEVENT)&CDlgModelRenderPass::OnEditBoxMaterialChanged);

	RegisterControlEvent("IDC_BTN_ADD",(PEVENT)&CDlgModelRenderPass::OnBtnAdd);
	RegisterControlEvent("IDC_BTN_DELETE",(PEVENT)&CDlgModelRenderPass::OnBtnDelete);
}

void CDlgModelRenderPass::SetVisible(bool bVisible)
{
	CDlgBaseEditor::SetVisible(bVisible);
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
		size_t uSubCount = getModelDisplay().getModelData()->getMesh().getSubCount();
		{
			m_CmbSubID.AddItem(L"Full",(void*)(-1));
			for (size_t i=0; i<uSubCount; ++i)
			{
				m_CmbSubID.AddItem(FormatW(L"[%d]",i),(void*)i);
			}
		}
	}
}

std::string CDlgModelRenderPass::getSelectedRenderPass()
{
	int nSelected = m_ListboxRenderPass.GetSelectedIndex();
	if (getModelDisplay().getModelData())
	{
		const ModelRenderPass& renderPass = getModelDisplay().getModelData()->m_mapPasses[nSelected];
		return renderPass.strMaterialName;
	}
	return "";
}

void CDlgModelRenderPass::OnListboxRenderPass()
{
	getMainDialog().getMaterialDialog().setMaterial(getSelectedRenderPass(),GetParentPath(getModelDisplay().getModelObject()->getModelFilename()));
	getMainDialog().getMaterialDialog().SetVisible(true);
	m_EditBoxMaterial.SetText(s2ws(getSelectedRenderPass()));

	int nSelected = m_ListboxRenderPass.GetSelectedIndex();
	if (getModelDisplay().getModelData())
	{
		const ModelRenderPass& renderPass = getModelDisplay().getModelData()->m_mapPasses[nSelected];
		m_CmbSubID.getListBox().selectByData((void*)renderPass.nSubID);
	}
}

void CDlgModelRenderPass::OnCmbSubIDSelected()
{
	int nSelected = m_ListboxRenderPass.GetSelectedIndex();
	if (getModelDisplay().getModelData())
	{
		const ModelRenderPass& renderPass = getModelDisplay().getModelData()->m_mapPasses[nSelected];
		//renderPass.nSubID = m_CmbSubID.getListBox().GetSelectedIndex()-1;
	}
}

void CDlgModelRenderPass::OnEditBoxMaterialChanged()
{
	int nSelected = m_ListboxRenderPass.GetSelectedIndex();
	if (getModelDisplay().getModelData())
	{
		const ModelRenderPass& renderPass = getModelDisplay().getModelData()->m_mapPasses[nSelected];
		renderPass.strMaterialName = ws2s(m_EditBoxMaterial.GetText());
	}
}

void CDlgModelRenderPass::OnBtnAdd()
{
	if (getModelDisplay().getModelData())
	{
		std::map<int,ModelRenderPass>& mapPasses=getModelDisplay().getModelData()->m_mapPasses;
		int passID = mapPasses.size();
		ModelRenderPass& renderPass = mapPasses[passID];
		renderPass.nSubID = m_CmbSubID.getListBox().GetSelectedIndex()-1;
		renderPass.strMaterialName = ws2s(m_EditBoxMaterial.GetText());
	}
}

void CDlgModelRenderPass::OnBtnDelete()
{
	int nSelected = m_ListboxRenderPass.GetSelectedIndex();
	if (getModelDisplay().getModelData())
	{
		std::map<int,ModelRenderPass>& mapPasses=getModelDisplay().getModelData()->m_mapPasses;
		std::map<int,ModelRenderPass>::iterator it=mapPasses.find(nSelected);
		if(it!=mapPasses.end())
		{
			mapPasses.erase(it);
			onReset();
		}
	}
}