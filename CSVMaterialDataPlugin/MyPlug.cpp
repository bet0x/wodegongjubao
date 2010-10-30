#include "MyPlug.h"
#include "CSVFile.h"

BOOL WINAPI Data_Plug_CreateObject(void ** pobj){
	*pobj = new CMyPlug;
	return *pobj != NULL;
}

CMyPlug::CMyPlug(void)
{

}

CMyPlug::~CMyPlug(void)
{

}

int CMyPlug::Execute(std::map<std::string, CMaterial>& mapItems, bool bShowDlg, bool bSpecifyFileName)
{
	return -1;
}

#include "FileSystem.h"
bool CMyPlug::importData(std::map<std::string, CMaterial>& mapItems, const char* szFilename, const char* szParentDir)
{
	CCsvFile csv;
	if (!csv.open(szFilename))
	{
		return false;
	}
	while (csv.seekNextLine())
	{
		const char* szMaterial	= csv.getStr("Name","");
		CMaterial& material		= mapItems[szMaterial];

		material.setDiffuse		(getRealFilename(szParentDir,csv.getStr("Diffuse","")));
		material.setEmissive	(getRealFilename(szParentDir,csv.getStr("Emissive","")));
		material.setSpecular	(getRealFilename(szParentDir,csv.getStr("Specular","")));
		material.setNormal		(getRealFilename(szParentDir,csv.getStr("Normal","")));
		material.setReflection	(getRealFilename(szParentDir,csv.getStr("Reflection","")));
		material.setLightMap	(getRealFilename(szParentDir,csv.getStr("LightMap","")));
		material.setShader		(getRealFilename(szParentDir,csv.getStr("Shader","")));

		material.m_fOpacity		=csv.getFloat("Opacity");
		material.uCull			=csv.getInt("Cull");
		material.bDepthWrite	=csv.getBool("IsDepthWrite");
		material.bBlend			=csv.getBool("IsBlend");
		material.bAlphaTest		=csv.getBool("IsAlphaTest");
		material.uAlphaTestValue=csv.getInt("AlphaTestValue");

		material.vTexAnim.x		=csv.getFloat("TexAnimX");
		material.vTexAnim.y		=csv.getFloat("TexAnimY");
		material.vUVScale.x		=1.0f/csv.getFloat("UScale");
		material.vUVScale.y		=1.0f/csv.getFloat("VScale");
	}
	csv.close();
	return true;
}

bool CMyPlug::exportData(std::map<std::string, CMaterial>& mapItems, const char* szFilename, const char* szParentDir)
{
	return true;
}

void CMyPlug::release()
{
	delete this;
}