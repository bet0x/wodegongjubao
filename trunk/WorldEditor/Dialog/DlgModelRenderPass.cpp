#include "DlgModelRenderPass.h"
#include "ModelDisplay.h"
#include "DlgMainEditor.h"
#include "FileSystem.h"

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
		size_t uCount = getModelDisplay().getModelData()->getRenderPassCount();
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
		int nSubID=0;
		std::string strMaterial;
		if (getModelDisplay().getModelData()->getRenderPass(nSelected,nSubID,strMaterial))
		{
			return strMaterial;
		}
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
		int nSubID=0;
		std::string strMaterial;
		if (getModelDisplay().getModelData()->getRenderPass(nSelected,nSubID,strMaterial))
		{
			m_CmbSubID.getListBox().selectByData((void*)nSubID);
		}
	}
}

void CDlgModelRenderPass::OnCmbSubIDSelected()
{
	int nSelected = m_ListboxRenderPass.GetSelectedIndex();
	if (getModelDisplay().getModelData())
	{
		//int nSubID=0;
		//std::string strMaterial;
		//if (getModelDisplay().getModelData()->getRenderPass(nSelected,nSubID,strMaterial))
		//{
		//	m_CmbSubID.getListBox().selectByData((void*)nSubID);
		//}

		//const ModelRenderPass& renderPass = getModelDisplay().getModelData()->m_mapPasses[nSelected];
		//renderPass.nSubID = m_CmbSubID.getListBox().GetSelectedIndex()-1;
	}
}

void CDlgModelRenderPass::OnEditBoxMaterialChanged()
{
	int nSelected = m_ListboxRenderPass.GetSelectedIndex();
	if (getModelDisplay().getModelData())
	{
		getModelDisplay().getModelData()->setRenderPass(nSelected,m_CmbSubID.getListBox().GetSelectedIndex()-1,ws2s(m_EditBoxMaterial.GetText()));
	}
}

void CDlgModelRenderPass::OnBtnAdd()
{
	if (getModelDisplay().getModelData())
	{
		int passID =getModelDisplay().getModelData()->getRenderPassCount();
		getModelDisplay().getModelData()->setRenderPass(passID,m_CmbSubID.getListBox().GetSelectedIndex()-1,ws2s(m_EditBoxMaterial.GetText()));
	}
}

void CDlgModelRenderPass::OnBtnDelete()
{
	int nSelected = m_ListboxRenderPass.GetSelectedIndex();
	if (getModelDisplay().getModelData())
	{
		if(getModelDisplay().getModelData()->delRenderPass(nSelected))
		{
			onReset();
		}
	}
}