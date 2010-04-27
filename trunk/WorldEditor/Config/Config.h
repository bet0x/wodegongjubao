#pragma once
#include <string>
#include <vector>

class CConfig
{
public:
	CConfig();
	~CConfig();

	std::wstring getRecentPath();
	void setRecentPath(const std::wstring& wstrRecentPath);
public:
	std::string m_strTempPath;
	std::string m_strWOWPath;
	std::string m_strMUPath;

	std::string m_strFontFilename;
	int			m_nFontSize;
private:
};
CConfig& GetConfig();
