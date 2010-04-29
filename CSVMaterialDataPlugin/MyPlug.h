#pragma once
#include "Material.h"

class CMyPlug : public CMaterialDataPlugBase  
{
public:
	CMyPlug(void);
	~CMyPlug(void);
	virtual const char * GetTitle(){return "OGRE Model File";}
	virtual const char * GetFormat() {return ".mesh";}
	virtual int Execute(std::map<std::string, CMaterial>& mapItems, bool bShowDlg, bool bSpecifyFileName);
	virtual bool importData(std::map<std::string, CMaterial>& mapItems, const std::string& strFilename);
	virtual bool exportData(std::map<std::string, CMaterial>& mapItems, const std::string& strFilename);

	virtual DWORD GetExportDataType(){return -1;}
	virtual DWORD GetImportDataType(){return -1;}
	virtual void Release();
private:
};