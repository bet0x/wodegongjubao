#pragma once
#include <string>
#include <vector>
#include <atlstr.h>
class CModelConfig
{
public:
	CModelConfig();
	~CModelConfig();
public:
	std::string m_strTempPath;
	std::string m_strWOWPath;
	std::string m_strMUPath;
	std::string m_strFontFilename;
	std::string m_strThemeFilename;
	std::string m_strUIFilename;
	int			m_nFontSize;
};
CModelConfig& GetModelConfig();