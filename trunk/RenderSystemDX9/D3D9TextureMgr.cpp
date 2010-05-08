#include "D3D9TextureMgr.h"
#include "D3D9Texture.h"

void CD3D9TextureMgr::OnResetDevice()
{
	for (std::set<CTexture*>::iterator it=m_setTextureList.begin();
		it!=m_setTextureList.end();it++)
	{
		((CD3D9Texture*)*it)->OnResetDevice();
	}
}

void CD3D9TextureMgr::OnLostDevice()
{
	for (std::set<CTexture*>::iterator it=m_setTextureList.begin();
		it!=m_setTextureList.end();it++)
	{
		((CD3D9Texture*)*it)->OnLostDevice();
	}
}

void CD3D9TextureMgr::OnDestroyDevice()
{
	for (std::set<CTexture*>::iterator it=m_setTextureList.begin();
		it!=m_setTextureList.end();it++)
	{
		((CD3D9Texture*)*it)->OnDestroyDevice();
	}
	for (std::map<uint32, CManagedItem>::iterator it=m_Items.begin(); it!=m_Items.end(); ++it)
	{
		((CD3D9Texture*)it->second.pItem)->OnDestroyDevice();
	}
}

CTexture* CD3D9TextureMgr::newTexture()
{
	CD3D9Texture* pD3D9Texture = new CD3D9Texture;
	return pD3D9Texture;
}