#include "DlgModelRenderPass.h"
#include "ModelDisplay.h"
#include "DlgMainEditor.h"
#include "FileSystem.h"
#include "..\MainRoot.h"

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
	CUIDialog::SetVisible(bVisible);
	if (bVisible==false)
	{
		CMainRoot::getInstance().getMainDialog().getMaterialDialog().SetVisible(false);
	}
}

void CDlgModelRenderPass::onReset()
{
	int nSelected = m_ListboxRenderPass.GetSelectedIndex();
	m_ListboxRenderPass.RemoveAllItems();
	if (CMainRoot::getInstance().getMainDialog().getModelDisplay().getModelData())
	{
		size_t uCount = CMainRoot::getInstance().getMainDialog().getModelDisplay().getModelData()->getRenderPassCount();
		if (uCount>0)
		{
			for (size_t i=0; i<uCount; ++i)
			{
				m_ListboxRenderPass.AddItem(i2ws(i).c_str());
			}
			if (nSelected>=uCount)
			{
				nSelected = uCount-1;
			}
			m_ListboxRenderPass.SelectItem(nSelected);
		}
		if (CMainRoot::getInstance().getMainDialog().getMaterialDialog().IsVisible())
		{
			OnListboxRenderPass();
		}
		size_t uSubCount = CMainRoot::getInstance().getMainDialog().getModelDisplay().getModelData()->getMesh().getSubCount();
		{
			m_CmbSubID.AddItem(L"Full",(void*)(-1));
			for (size_t i=0; i<uSubCount; ++i)
			{
				m_CmbSubID.AddItem(i2ws(i).c_str(),(void*)i);
			}
		}
	}
}

std::string CDlgModelRenderPass::getSelectedRenderPass()
{
	int nSelected = m_ListboxRenderPass.GetSelectedIndex();
	if (CMainRoot::getInstance().getMainDialog().getModelDisplay().getModelData())
	{
		int nSubID=0;
		std::string strMaterial;
		if (CMainRoot::getInstance().getMainDialog().getModelDisplay().getModelData()->getRenderPass(nSelected,nSubID,strMaterial))
		{
			return strMaterial;
		}
	}
	return "";
}

void CDlgModelRenderPass::OnListboxRenderPass()
{
	CMainRoot::getInstance().getMainDialog().getMaterialDialog().setMaterial(getSelectedRenderPass(),GetParentPath(CMainRoot::getInstance().getMainDialog().getModelDisplay().getModelObject()->getModelFilename()));
	CMainRoot::getInstance().getMainDialog().getMaterialDialog().SetVisible(true);
	m_EditBoxMaterial.SetText(s2ws(getSelectedRenderPass()).c_str());

	int nSelected = m_ListboxRenderPass.GetSelectedIndex();
	if (CMainRoot::getInstance().getMainDialog().getModelDisplay().getModelData())
	{
		int nSubID=0;
		std::string strMaterial;
		if (CMainRoot::getInstance().getMainDialog().getModelDisplay().getModelData()->getRenderPass(nSelected,nSubID,strMaterial))
		{
			m_CmbSubID.getListBox().selectByData((void*)nSubID);
		}
	}
}

void CDlgModelRenderPass::OnCmbSubIDSelected()
{
	int nSelected = m_ListboxRenderPass.GetSelectedIndex();
	if (CMainRoot::getInstance().getMainDialog().getModelDisplay().getModelData())
	{
		//int nSubID=0;
		//std::string strMaterial;
		//if (CMainRoot::getInstance().getMainDialog().getModelDisplay().getModelData()->getRenderPass(nSelected,nSubID,strMaterial))
		//{
		//	m_CmbSubID.getListBox().selectByData((void*)nSubID);
		//}

		//const ModelRenderPass& renderPass = CMainRoot::getInstance().getMainDialog().getModelDisplay().getModelData()->m_mapPasses[nSelected];
		//renderPass.nSubID = m_CmbSubID.getListBox().GetSelectedIndex()-1;
	}
}

void CDlgModelRenderPass::OnEditBoxMaterialChanged()
{
	int nSelected = m_ListboxRenderPass.GetSelectedIndex();
	if (CMainRoot::getInstance().getMainDialog().getModelDisplay().getModelData())
	{
		CMainRoot::getInstance().getMainDialog().getModelDisplay().getModelData()->setRenderPass(nSelected,m_CmbSubID.getListBox().GetSelectedIndex()-1,ws2s(m_EditBoxMaterial.GetText()));
	}
}

void CDlgModelRenderPass::OnBtnAdd()
{
	if (CMainRoot::getInstance().getMainDialog().getModelDisplay().getModelData())
	{
		int passID =CMainRoot::getInstance().getMainDialog().getModelDisplay().getModelData()->getRenderPassCount();
		CMainRoot::getInstance().getMainDialog().getModelDisplay().getModelData()->setRenderPass(passID,m_CmbSubID.getListBox().GetSelectedIndex()-1,ws2s(m_EditBoxMaterial.GetText()));
	}
}

void CDlgModelRenderPass::OnBtnDelete()
{
	int nSelected = m_ListboxRenderPass.GetSelectedIndex();
	if (CMainRoot::getInstance().getMainDialog().getModelDisplay().getModelData())
	{
		if(CMainRoot::getInstance().getMainDialog().getModelDisplay().getModelData()->delRenderPass(nSelected))
		{
			onReset();
		}
	}
}