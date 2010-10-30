#pragma once
#include "Material.h"

class CMyPlug : public CMaterialDataPlugBase  
{
public:
	CMyPlug(void);
	~CMyPlug(void);
	virtual const char * getTitle(){return "CSV Materila Data File";}
	virtual const char * getFormat() {return ".csv";}
	virtual int Execute(std::map<std::string, CMaterial>& mapItems, bool bShowDlg, bool bSpecifyFileName);
	virtual bool importData(std::map<std::string, CMaterial>& mapItems, const char* szFilename, const char* szParentDir);
	virtual bool exportData(std::map<std::string, CMaterial>& mapItems, const char* szFilename, const char* szParentDir);

	virtual void release();
private:
};