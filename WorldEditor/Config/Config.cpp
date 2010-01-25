#include "Config.h"
#include "tinyxml.h"

CConfig& GetConfig()
{
	static CConfig g_ModelConfig;
	return g_ModelConfig;
}
#include "IORead.h"
CConfig::CConfig()
{
	//创建一个XML的文档对象。
	TiXmlDocument *myDocument = new TiXmlDocument();
	{
		IOReadBase* pRead = IOReadBase::autoOpen("config.xml");
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
		m_strTempPath="tempPath";
		m_strWOWPath="D:\\wow\\Data\\";
		m_strMUPath="D:\\mu\\Data\\";
		m_strLastPath="";
		m_strFontFilename="Data\\font.ttf";
		m_strThemeFilename="Data\\Themes\\CSS\\UIStyle.xml";
		m_strUIFilename="Data\\UI\\Dialog_EN.xml";
		m_nFontSize=15;
	}
	else
	{
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
		if (PathElement->Attribute("lastpath"))
		{
			m_strLastPath = PathElement->Attribute("lastpath");
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
	}
	delete myDocument;
}

CConfig::~CConfig()
{
	//创建一个XML的文档对象。
	TiXmlDocument myDocument;

	TiXmlElement path("path");
	path.SetAttribute("temp",m_strTempPath.c_str());
	path.SetAttribute("wow",m_strWOWPath.c_str());
	path.SetAttribute("mu",m_strMUPath.c_str());
	path.SetAttribute("lastpath",m_strLastPath.c_str());
	path.SetAttribute("font",m_strFontFilename.c_str());
	path.SetAttribute("theme",m_strThemeFilename.c_str());
	path.SetAttribute("ui",m_strUIFilename.c_str());
	path.SetAttribute("font_size",m_nFontSize);
	TiXmlElement RootElement("root");
	RootElement.InsertEndChild(*((TiXmlNode*)&path));
	myDocument.InsertEndChild(*((TiXmlNode*)&RootElement));
	myDocument.SaveFile("config.xml");
}