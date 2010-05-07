#pragma once
#include "TextureMgr.h"

class DLL_EXPORT CD3D9TextureMgr : public CTextureMgr
{
	friend class CD3D9Texture;
public:
	CD3D9TextureMgr() {}
	~CD3D9TextureMgr() {}
public:
	//HRESULT OnCreateDevice(LPDIRECT3DDEVICE9 pd3dDevice);
	void OnResetDevice();
	void OnLostDevice();
	void OnDestroyDevice();
protected:
	CTexture* newTexture();
};