#pragma once
#include "InterfaceModel.h"

class CMyPlug : public CModelPlugBase  
{
public:
	CMyPlug(void);
	~CMyPlug(void);
	virtual const char * GetTitle(){return "Mu Model File";}
	virtual const char * GetFormat() {return ".bmd";}
	virtual int Execute(iModelData * pModelData, bool bShowDlg, bool bSpecifyFileName);
	virtual int importData(iModelData * pModelData, const std::string& strFilename);
	virtual int exportData(iModelData * pModelData, const std::string& strFilename);

	virtual DWORD GetExportDataType(){return -1;}
	virtual DWORD GetImportDataType(){return -1;}
	virtual void Release();
private:
};