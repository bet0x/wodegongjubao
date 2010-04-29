#include "MyPlug.h"
#include "CSVFile.h"

BOOL WINAPI Material_Data_Plug_CreateObject(void ** pobj){
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

bool CMyPlug::importData(std::map<std::string, CMaterial>& mapItems, const std::string& strFilename)
{
	CCsvFile csv;
	if (!csv.Open(strFilename))
	{
		return false;
	}
	while (csv.SeekNextLine())
	{
		const std::string strMaterialName = csv.GetStr("Name");
		CMaterial& material = mapItems[strMaterialName];

		material.setDiffuse		(csv.GetStr("Diffuse"));
		material.setEmissive	(csv.GetStr("Emissive"));
		material.setSpecular	(csv.GetStr("Specular"));
		material.setNormal		(csv.GetStr("Normal"));
		material.setReflection	(csv.GetStr("Reflection"));
		material.setLightMap	(csv.GetStr("LightMap"));
		material.setShader		(csv.GetStr("Shader"));

		material.m_fOpacity		=csv.GetFloat("Opacity");
		material.uCull			=csv.GetInt("Cull");
		material.bDepthWrite	=csv.GetBool("IsDepthWrite");
		material.bBlend			=csv.GetBool("IsBlend");
		material.bAlphaTest		=csv.GetBool("IsAlphaTest");
		material.uAlphaTestValue=csv.GetInt("AlphaTestValue");

		material.vTexAnim.x		=csv.GetFloat("TexAnimX");
		material.vTexAnim.y		=csv.GetFloat("TexAnimY");
		material.vUVScale.x		=1.0f/csv.GetFloat("UScale");
		material.vUVScale.y		=1.0f/csv.GetFloat("VScale");
	}
	csv.Close();
	return true;
}

bool CMyPlug::exportData(std::map<std::string, CMaterial>& mapItems, const std::string& strFilename)
{
	return true;
}

void CMyPlug::Release()
{
	delete this;
}