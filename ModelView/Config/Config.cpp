#include "Config.h"
#include "tinyxml.h"

CModelConfig& GetModelConfig()
{
	static CModelConfig g_ModelConfig;
	return g_ModelConfig;
}
#include "IORead.h"
CModelConfig::CModelConfig()
{
	//创建一个XML的文档对象。
	TiXmlDocument *myDocument = new TiXmlDocument();
	{
		IOReadBase* pRead = IOReadBase::autoOpen("model.xml");
		if (pRead)
		{
			char* pBuf = new char[pRead->GetSize()+1];
			if (pBuf)
			{
				pBuf[pRead->GetSize()] = 0;
				pRead->Read(pBuf, pRead->GetSize());
				myDocument->LoadFormMemory(pBuf, pRead->GetSize(), TIXML_ENCODING_UTF8);
				delete[] pBuf;
			}
			IOReadBase::autoClose(pRead);
		}
	}
	if ( myDocument->Error() )
	{
		TiXmlElement path("path");
		path.SetAttribute("temp","tempPath");
		path.SetAttribute("wow","D:\\wow\\Data\\");
		path.SetAttribute("mu","D:\\mu\\Data\\");

		path.SetAttribute("font","Data\\font.ttf");
		path.SetAttribute("theme","Data\\Themes\\MU\\UIStyle.xml");
		path.SetAttribute("ui","Data\\UI\\Dialog_EN.xml");
		path.SetAttribute("font_size","15");
		TiXmlElement RootElement("root");
		RootElement.InsertEndChild(*((TiXmlNode*)&path));
		myDocument->InsertEndChild(*((TiXmlNode*)&RootElement));
		myDocument->SaveFile();
	}

	//获得根元素，即root。
	TiXmlElement *RootElement = myDocument->RootElement();
	//获得第一个window节点。
	TiXmlElement *PathElement = RootElement->FirstChildElement("path");

	//const char* u8 = PathElement->Attribute("temp");
	if (PathElement->Attribute("temp"))
	{
		m_strTempPath = PathElement->Attribute("temp");
	}
	if (PathElement->Attribute("wow"))
	{
		m_strWOWPath = PathElement->Attribute("wow");
	}
	if (PathElement->Attribute("mu"))
	{
		m_strMUPath = PathElement->Attribute("mu");
	}
	if (PathElement->Attribute("font"))
	{
		m_strFontFilename = PathElement->Attribute("font");
	}
	if (PathElement->Attribute("theme"))
	{
		m_strThemeFilename = PathElement->Attribute("theme");
	}
	if (PathElement->Attribute("ui"))
	{
		m_strUIFilename = PathElement->Attribute("ui");
	}
	if (PathElement->Attribute("font_size"))
	{
		m_nFontSize = atoi(PathElement->Attribute("font_size"));
	}
	//std::string m_strTempPath;
	//std::string m_strWOWPath;
	//strcpy(m_szTempPath,u8);
	//MultiByteToWideChar(CP_UTF8,0,u8,-1,m_szTempPath,MAX_PATH);
	//读入Wow
	//u8 = PathElement->Attribute("wow");
	//strcpy(m_szWOWPath,u8);
	//MultiByteToWideChar(CP_UTF8,0,u8,-1,m_szWOWPath,MAX_PATH);

	delete myDocument;
}

CModelConfig::~CModelConfig()
{
}