#pragma once
#include "ShaderMgr.h"

class CD3D9ShaderMgr : public CShaderMgr
{
public:
	void OnResetDevice();
	void OnLostDevice();
	void OnDestroyDevice();

	CShader* createItem(const std::string& strFilename);
protected:
	unsigned long	m_uShareShaderID;
};