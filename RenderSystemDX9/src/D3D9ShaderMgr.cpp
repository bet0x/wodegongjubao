#include "D3D9ShaderMgr.h"
#include "D3D9Shader.h"
#include "D3D9RenderSystem.h"

void CD3D9ShaderMgr::OnResetDevice()
{
	for (std::map<uint32, CManagedItem>::iterator it=m_Items.begin(); it!=m_Items.end(); ++it)
	{
		((CD3D9Shader*)it->second.pItem)->OnResetDevice();
	}
}

void CD3D9ShaderMgr::OnLostDevice()
{
	for (std::map<uint32, CManagedItem>::iterator it=m_Items.begin(); it!=m_Items.end(); ++it)
	{
		((CD3D9Shader*)it->second.pItem)->OnLostDevice();
	}
}

void CD3D9ShaderMgr::OnDestroyDevice()
{
	for (std::map<uint32, CManagedItem>::iterator it=m_Items.begin(); it!=m_Items.end(); ++it)
	{
		((CD3D9Shader*)it->second.pItem)->OnDestroyDevice();
	}
}

CShader* CD3D9ShaderMgr::createItem(const std::string& strFilename)
{
	IDirect3DDevice9* m_pD3D9Device = ((CD3D9RenderSystem*)&GetRenderSystem())->GetD3D9Device();
	CD3D9Shader* pD3D9Shader = new CD3D9Shader;
	if (pD3D9Shader->create(m_pD3D9Device, strFilename))
	{
		return pD3D9Shader;
	}
	S_DEL(pD3D9Shader);
	return NULL;
}