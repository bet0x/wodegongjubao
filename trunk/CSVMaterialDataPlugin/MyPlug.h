#pragma once
#include "Material.h"

class CMyPlug : public CMaterialDataPlugBase  
{
public:
	CMyPlug(void);
	~CMyPlug(void);
	virtual const char * GetTitle(){return "CSV Materila Data File";}
	virtual const char * GetFormat() {return ".csv";}
	virtual int Execute(std::map<std::string, CMaterial>& mapItems, bool bShowDlg, bool bSpecifyFileName);
	virtual bool importData(std::map<std::string, CMaterial>& mapItems, const char* szFilename);
	virtual bool exportData(std::map<std::string, CMaterial>& mapItems, const char* szFilename);

	virtual DWORD GetExportDataType(){return -1;}
	virtual DWORD GetImportDataType(){return -1;}
	virtual void Release();
private:
};