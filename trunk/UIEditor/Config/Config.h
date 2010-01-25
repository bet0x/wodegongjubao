#pragma once
#include <string>
#include <vector>

class CConfig
{
public:
	CConfig();
	~CConfig();
public:
	std::string m_strLastPath;
	std::string m_strFontFilename;
	std::string m_strThemeFilename;
	std::string m_strUIFilename;
	int			m_nFontSize;
};
CConfig& GetConfig();
