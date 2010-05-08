#pragma once
#include "ShaderMgr.h"

class DLL_EXPORT CD3D9ShaderMgr : public CShaderMgr
{
public:
	void OnResetDevice();
	void OnLostDevice();
	void OnDestroyDevice();

	CShader* createItem(const std::string& strFilename);
protected:
	uint32	m_uShareShaderID;
};