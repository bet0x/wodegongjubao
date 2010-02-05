#include "MyPlug.h"
#include "IORead.h"
#include "FileSystem.h"
#include "Jpeg.h"
#include "3DMapSceneObj.h"
//标记开始处.
#define   VMBEGIN __asm\
{\
_emit 0xEB\
_emit 0x10\
_emit 0x56\
_emit 0x4D\
_emit 0x50\
_emit 0x72\
_emit 0x6F\
_emit 0x74\
_emit 0x65\
_emit 0x63\
_emit 0x74\
_emit 0x20\
_emit 0x62\
_emit 0x65\
_emit 0x67\
_emit 0x69\
_emit 0x6E\
_emit 0x00\
}
//标记结束处.
#define   VMEND __asm\
{\
_emit 0xEB\
_emit 0x0E\
_emit 0x56\
_emit 0x4D\
_emit 0x50\
_emit 0x72\
_emit 0x6F\
_emit 0x74\
_emit 0x65\
_emit 0x63\
_emit 0x74\
_emit 0x20\
_emit 0x65\
_emit 0x6E\
_emit 0x64\
_emit 0x00\
}
CMyPlug::CMyPlug(void)
{

}

CMyPlug::~CMyPlug(void)
{

}

#define MAP_FILE_SIZE 65536*3+2
#define HEIGHT_HEAD_SIZE 1082
#define HEIGHT_BUFFER_SIZE 65536
#define ATT_FILE_129KB_SIZE 65536*2+4
#define ATT_FILE_65KB_SIZE 65536+4
#define ATT_FILE_SERVER_SIZE 65536+3

inline void decrypt2(char* buffer, size_t size)
{
	const char xorKeys[] = {0xFC, 0xCF, 0xAB};
	for (size_t i=0; i<size; ++i)
	{
		*buffer ^= xorKeys[i%3];
		buffer++;
	}
}

inline void decrypt(char* buffer, size_t size)
{
	const char xorKeys[] = {
		0xd1, 0x73, 0x52, 0xf6,
		0xd2, 0x9a, 0xcb, 0x27,
		0x3e, 0xaf, 0x59, 0x31,
		0x37, 0xb3, 0xe7, 0xa2
	};
	char key = 0x5E;
	for (size_t i=0; i<size; ++i)
	{
		char encode = *buffer;
		*buffer ^= xorKeys[i%16];
		*buffer -= key;
		key = encode+0x3D;
		buffer++;
	}
}

inline void encrypt(char* buffer, size_t size)
{
	const char xorKeys[] = {
		0xd1, 0x73, 0x52, 0xf6,
		0xd2, 0x9a, 0xcb, 0x27,
		0x3e, 0xaf, 0x59, 0x31,
		0x37, 0xb3, 0xe7, 0xa2
	};
	char key = 0x5E;
	for (size_t i=0; i<size; ++i)
	{
		*buffer += key;
		*buffer ^= xorKeys[i%16];
		key = *buffer+0x3D;
		buffer++;
	}
}

int CMyPlug::Execute(iScene * pScene, bool bShowDlg, bool bSpecifyFileName)
{
	return -1;
}

bool CMyPlug::importTerrainData(iTerrainData * pTerrainData, const std::string& strFilename)
{
	pTerrainData->clear();
	if (pTerrainData->resize(253,253,11))
	{
		// EncTerrain
		IOReadBase* pRead = IOReadBase::autoOpen(strFilename);
		if (pRead)
		{
			if (MAP_FILE_SIZE==pRead->GetSize())
			{
				char buffer[MAP_FILE_SIZE];
				pRead->Read(buffer,MAP_FILE_SIZE);
				decrypt(buffer,MAP_FILE_SIZE);
				char* p = buffer;
				m_uMuFlgMap = *((uint16*)p);
				p+=2;
				for (int y=0; y<253; ++y)
				{
					for (int x=0; x<253; ++x)
					{
						pTerrainData->SetCellTileID(Pos2D(x,y),*p,0);
						p++;
					}
					p+=3;
				}
				p+=256*3;
				for (int y=0; y<253; ++y)
				{
					for (int x=0; x<253; ++x)
					{
						pTerrainData->SetCellTileID(Pos2D(x,y),*p,1);
						p++;
					}
					p+=3;
				}
				p+=256*3;
				for (int y=0; y<254; ++y)
				{
					for (int x=0; x<254; ++x)
					{
						pTerrainData->setVertexColor(Pos2D(x,y),Color32(*p,255,255,255));
						p++;
					}
					p+=2;
				}
			}
			IOReadBase::autoClose(pRead);
		}
		// EncTerrain.att
		pRead = IOReadBase::autoOpen(ChangeExtension(strFilename,".att"));
		if (pRead)
		{
			if (ATT_FILE_129KB_SIZE==pRead->GetSize())
			{
				char buffer[ATT_FILE_129KB_SIZE];
				pRead->Read(buffer,ATT_FILE_129KB_SIZE);
				decrypt(buffer,ATT_FILE_129KB_SIZE);
				decrypt2(buffer,ATT_FILE_129KB_SIZE);
				char* p = buffer;
				m_uMuFlgAtt = *((uint32*)p);
				p+=4;
				for (int y=0; y<253; ++y)
				{
					for (int x=0; x<253; ++x)
					{
						pTerrainData->setCellAttribute(Pos2D(x,y),*p);
						p+=2;
					}
					p+=6;
				}
			}
			else if (ATT_FILE_65KB_SIZE==pRead->GetSize())
			{
				char buffer[ATT_FILE_65KB_SIZE];
				pRead->Read(buffer,ATT_FILE_65KB_SIZE);
				decrypt(buffer,ATT_FILE_65KB_SIZE);
				decrypt2(buffer,ATT_FILE_65KB_SIZE);
				char* p = buffer;
				m_uMuFlgAtt = *((uint32*)p);
				p+=4;
				for (int y=0; y<253; ++y)
				{
					for (int x=0; x<253; ++x)
					{
						pTerrainData->setCellAttribute(Pos2D(x,y),*p);
						p++;
					}
					p+=3;
				}
			}
			IOReadBase::autoClose(pRead);
		}
		// TerrainLight
		std::string strTerrainLight = GetParentPath(strFilename)+"TerrainLight.ozj";
		pRead = IOReadBase::autoOpen(strTerrainLight);
		if (pRead)
		{
			size_t uFileSize = pRead->GetSize();
			char* buffer = new char[uFileSize];
			pRead->Read(buffer,uFileSize);
			CJpeg myJpg;
			if (myJpg.loadJpegFromBuffer(buffer+24,uFileSize))
			{
				uint8* pImg = (uint8*)myJpg.getBuffer();
				for (int y=0; y<254; ++y)
				{
					for (int x=0; x<254; ++x)
					{

						Color32 c = pTerrainData->getVertexColor(Pos2D(x,y));
						c.b = *pImg;
						pImg++;
						c.g = *pImg;
						pImg++;
						c.r = *pImg;
						pImg++;
						pTerrainData->setVertexColor(Pos2D(x,y),c);
					}
					pImg+=2*3;
				}
			}
			delete[] buffer;
		}

		// TerrainHeight
		std::string strHeightFilename = GetParentPath(strFilename)+"TerrainHeight.ozb";
		pRead = IOReadBase::autoOpen(strHeightFilename);
		if (pRead)
		{
			if (HEIGHT_HEAD_SIZE+HEIGHT_BUFFER_SIZE<=pRead->GetSize())
			{
				pRead->Move(HEIGHT_HEAD_SIZE);
				for (int y=0; y<254; ++y)
				{
					pRead->Move(2);
					for (int x=0; x<254; ++x)
					{
						uint8 uVal;
						pRead->Read(&uVal,1);
						pTerrainData->setVertexHeight(Pos2D(x,y),uVal*0.015f);
					}
				}
			}
			IOReadBase::autoClose(pRead);
		}
	}
	return true;
}
#include "CsvFile.h"
bool CMyPlug::importTiles(iTerrain * pTerrain, const std::string& strFilename, const std::string& strPath)
{
	pTerrain->clearAllTiles();
	CCsvFile csv;
	if (csv.Open(strFilename))
	{
		while (csv.SeekNextLine())
		{
			pTerrain->setTile(csv.GetInt("ID"), csv.GetStr("Name"),
				getRealFilename(strPath,csv.GetStr("Diffuse")),
				getRealFilename(strPath,csv.GetStr("Emissive")),
				getRealFilename(strPath,csv.GetStr("Specular")),
				getRealFilename(strPath,csv.GetStr("Normal")),
				getRealFilename(strPath,csv.GetStr("Environment")),
				csv.GetStr("Effect"),
				csv.GetInt("Channel"),
				csv.GetBool("AlphaBlend"),
				csv.GetBool("AlphaTest"),
				1.0f/(float)csv.GetInt("usize"),
				1.0f/(float)csv.GetInt("vsize"));
		}
		csv.Close();
	}
	return true;
}

bool CMyPlug::importObjectResources(iScene * pScene, const std::string& strFilename, const std::string& strPath)
{
	pScene->clearObjectResources();
	CCsvFile csvObject;
	if (csvObject.Open(strFilename))
	{
		while (csvObject.SeekNextLine())
		{
			pScene->setObjectResources(
				csvObject.GetInt("ID"),
				csvObject.GetStr("Name"),
				getRealFilename(strPath,csvObject.GetStr("Filename")));
				//Info.bbox				= 
				//Info.bIsGround			= csvObject.GetBool("IsGround");
				//Info.bHasShadow			= csvObject.GetBool("HasShadow");
				//Info.strFilename	= csvObject.GetStr("ModelFilename");
		}
		csvObject.Close();
	}
	return true;
}

bool CMyPlug::importObjectResourcesFormDir(iScene * pScene,const std::string& strPath)
{
	pScene->clearObjectResources();
	for (size_t i=0; i<256; i++)
	{
		std::string strFilename = "Object";
		if (i<9)
		{
			strFilename+="0";
		}
		strFilename+=ws2s(i2ws(i+1))+".bmd";
		if (IOReadBase::Exists(strPath+strFilename))
		{
			pScene->setObjectResources(i,strFilename,strPath+strFilename);
		}
	}
	return true;
}

#pragma pack(push,1)
struct ObjInfo
{
	int16 id;
	Vec3D p;
	Vec3D rotate;
	float fScale;
};
#pragma pack(pop)

bool CMyPlug::importObject(iScene * pScene, const std::string& strFilename)
{
	pScene->removeAllObjects();
	IOReadBase* pRead = IOReadBase::autoOpen(strFilename);
	if (pRead)
	{
		size_t fileSize = pRead->GetSize();
		char* buffer = new char[fileSize];
		pRead->Read(buffer, fileSize);
		decrypt(buffer,fileSize);

		int m_uUnknow = *((uint16*)(buffer));
		uint16 uObjCount = *((uint16*)(buffer+2));
		ObjInfo* pObjInfo = (ObjInfo*)(buffer+4);
		for (int i=0; i<uObjCount;++i)
		{
			Vec3D vPos = Vec3D(pObjInfo->p.x,pObjInfo->p.z,pObjInfo->p.y)*0.01f;
			Vec3D vRotate = Vec3D(pObjInfo->rotate.x,pObjInfo->rotate.z,pObjInfo->rotate.y)*PI/180.0f;

			if (false==pScene->add3DMapSceneObj(pObjInfo->id,vPos,vRotate,pObjInfo->fScale))
			{
				//MessageBoxA(NULL,"cannot find ID!","Error",0);
			}
			pObjInfo++;
		}
		delete buffer;
		IOReadBase::autoClose(pRead);
	}
	return true;
}

int getMapIDFromFilename(const std::string& strFilename)
{
	int nMapID = -1;
	{
		std::string strPath = GetFilename(GetParentPath(strFilename));
		size_t i = strPath.find("world");
		if (i!=std::string::npos)
		{
			nMapID=atoi(strPath.substr(i+5).c_str());
		}
	}
	return nMapID;
}

int CMyPlug::importData(iScene * pScene, const std::string& strFilename)
{
	importTerrainData(&pScene->getTerrain()->GetData(),strFilename);
	// tiles
	std::string strTileFile = GetParentPath(strFilename)+"Tile.csv";
	if (!IOReadBase::Exists(strTileFile))
	{
		strTileFile="Data\\default\\Tile.csv";
	}
	importTiles(pScene->getTerrain(),strTileFile,GetParentPath(strFilename));
	//
	//pScene->getTerrain()->setLightMapTexture(strFilename+"TerrainLight.OZJ");
	pScene->getTerrain()->setGrassTexture(GetParentPath(strFilename)+"TileGrass01.OZT");
	pScene->getTerrain()->setGrassShader("data\\fx\\TerrainGrass.fx");
	pScene->getTerrain()->create();

	// calc MU's filename
	int nMapID = getMapIDFromFilename(strFilename);
	std::string strObjectPath = Format("%s%s%d\\",GetParentPath(GetParentPath(strFilename)).c_str(),"Object",nMapID);
	if (nMapID==1)
	{
		importObjectResources(pScene,"Data\\World1\\Object.csv",strObjectPath); 
	}
	else
	{
		importObjectResourcesFormDir(pScene,strObjectPath);
	}
	BBox bboxObject;
	bboxObject.vMin = Vec3D(-10.0f,-10.0f,-10.0f);
	bboxObject.vMax = Vec3D(pScene->getTerrain()->GetData().GetWidth()+10.0f,10.0f,pScene->getTerrain()->GetData().GetHeight()+10.0f);
	pScene->createObjectTree(bboxObject,16);
	importObject(pScene,ChangeExtension(strFilename,".obj"));
	return true;
}


bool CMyPlug::exportTerrainData(iTerrainData * pTerrainData, const std::string& strFilename)
{
	// Calc MU's map id from filename.
	int nMapID = getMapIDFromFilename(strFilename);
	// map
	FILE* f=fopen(strFilename.c_str(),"wb");
	if (f)
	{
		char buffer[MAP_FILE_SIZE];
		char* p = buffer;
		*((unsigned char*)p)=0;++p;
		*((unsigned char*)p)=nMapID;++p;
		// tile
		for (size_t layer=0;layer<2;++layer)
		{
			for (int y=0; y<253; ++y)
			{
				for (int x=0; x<253; ++x)
				{
					*p = pTerrainData->GetCellTileID(Pos2D(x,y),layer);
					p++;
				}
				*p =0;++p;
				*p =0;++p;
				*p =0;++p;
			}
			for (int x=0; x<256*3; ++x)
			{
				*p =0;++p;
			}
		}
		// alpha
		{
			for (int y=0; y<254; ++y)
			{
				for (int x=0; x<254; ++x)
				{
					*p = pTerrainData->getVertexColor(Pos2D(x,y)).a;
					p++;
				}
				*p =0;++p;
				*p =0;++p;
			}
			for (int x=0; x<256*2; ++x)
			{
				*p =0;++p;
			}
		}
		encrypt(buffer,MAP_FILE_SIZE);
		fwrite(buffer,MAP_FILE_SIZE,1,f);
		fclose(f);
	}
	// att
	// for server.att
	f=fopen(ChangeExtension(strFilename,"ForServer.att").c_str(),"wb+");
	if (f)
	{
		char buffer[ATT_FILE_SERVER_SIZE];
		char* p = buffer;
		*((unsigned char*)p)=0x0;++p;
		*((unsigned char*)p)=0xFF;++p;
		*((unsigned char*)p)=0xFF;++p;
		for (int y=0; y<253; ++y)
		{
			for (int x=0; x<253; ++x)
			{
				*p = pTerrainData->getCellAttribute(Pos2D(x,y));
				p++;
			}
			for (int x=253; x<256; ++x)
			{
				*p =0;++p;
			}
		}
		for (int x=0; x<256*3; ++x)
		{
			*p =0;++p;
		}
		fwrite(buffer,ATT_FILE_SERVER_SIZE,1,f);
		fclose(f);
	}
	// EncTerrain.att
	f=fopen(ChangeExtension(strFilename,".att").c_str(),"wb+");
	if (f)
	{
		switch(nMapID)
		{
		case 2:
		case 4:
		case 5:
		case 6:
		case 7:
		case 10:
		case 11:
		case 12:
		case 19:

			{
				char buffer[ATT_FILE_65KB_SIZE];
				char* p = buffer;
				*((unsigned char*)p)=0x0;++p;
				*((unsigned char*)p)=nMapID;++p;
				*((unsigned char*)p)=0xFF;++p;
				*((unsigned char*)p)=0xFF;++p;
				{
					for (int y=0; y<253; ++y)
					{
						for (int x=0; x<253; ++x)
						{
							*p = pTerrainData->getCellAttribute(Pos2D(x,y));
							p++;
						}
						for (int x=253; x<256; ++x)
						{
							*p =0;++p;
						}
					}
					for (int x=0; x<256*3; ++x)
					{
						*p =0;++p;
					}
				}
				decrypt2(buffer,ATT_FILE_65KB_SIZE);
				encrypt(buffer,ATT_FILE_65KB_SIZE);
				fwrite(buffer,ATT_FILE_65KB_SIZE,1,f);
			}
			break;
		default:
			{
				char buffer[ATT_FILE_129KB_SIZE];
				char* p = buffer;
				*((unsigned char*)p)=0x0;++p;
				*((unsigned char*)p)=nMapID;++p;
				*((unsigned char*)p)=0xFF;++p;
				*((unsigned char*)p)=0xFF;++p;
				{
					for (int y=0; y<253; ++y)
					{
						for (int x=0; x<253; ++x)
						{
							*p = pTerrainData->getCellAttribute(Pos2D(x,y));
							p++;
							*p =0;++p;
						}
						for (int x=253; x<256; ++x)
						{
							*p =0;++p;
							*p =0;++p;
						}
					}
					for (int x=0; x<256*6; ++x)
					{
						*p =0;++p;
					}
				}
				decrypt2(buffer,ATT_FILE_129KB_SIZE);
				encrypt(buffer,ATT_FILE_129KB_SIZE);
				fwrite(buffer,ATT_FILE_129KB_SIZE,1,f);
			}
			break;
		}
		fclose(f);
	}
	// Height
	std::string strHeightFilename = GetParentPath(strFilename)+"TerrainHeight.ozb";
	f=fopen(strHeightFilename.c_str(),"wb+");
	if (f)
	{
		fseek(f,HEIGHT_HEAD_SIZE,SEEK_SET);
		char buffer[HEIGHT_BUFFER_SIZE];
		char* p = buffer;
		int i = 0;
		for (int y=0; y<254; ++y)
		{
			*p =0;++p;
			*p =0;++p;
			for (int x=0; x<254; ++x)
			{
				*p = max(min(pTerrainData->getVertexHeight(Pos2D(x,y))/0.015f,255),0);
				p++;
				i++;
			}
		}
		for (int x=0; x<256*2; ++x)
		{
			*p =0;++p;
		}
		fwrite(buffer,HEIGHT_BUFFER_SIZE,1,f);
		fclose(f);
	}
	return true;
}

bool CMyPlug::exportTiles(iTerrain * pTerrain, const std::string& strFilename, const std::string& strPath)
{
	return true;
}

bool CMyPlug::exportObjectResources(iScene * pScene, const std::string& strFilename, const std::string& strPath)
{
	return true;
}

bool CMyPlug::exportObjectResourcesFormDir(iScene * pScene,const std::string& strPath)
{
	return true;
}

bool CMyPlug::exportObject(iScene * pScene, const std::string& strFilename)
{
	FILE* f=fopen(strFilename.c_str(),"wb");
	if (f)
	{
		std::vector<ObjInfo> setObjInfo;
		DEQUE_MAPOBJ setObject;
		pScene->getAllObjects(setObject);
		for (DEQUE_MAPOBJ::iterator it=setObject.begin();it!=setObject.end();it++)
		{
			if ((*it)->GetObjType()==MAP_3DSIMPLE)
			{
				ObjInfo objInfo;
				C3DMapSceneObj* pObj = (C3DMapSceneObj*)(*it);
				Vec3D vPos = pObj->getPos();
				vPos = Vec3D(vPos.x,vPos.z,vPos.y)*100.0f;
				Vec3D vRotate = pObj->getRotate();
				vRotate = Vec3D(vRotate.x,vRotate.z,vRotate.y)*180.0f/PI;

				objInfo.id = pObj->getObjectID();
				objInfo.p = vPos;
				objInfo.rotate = vRotate;
				objInfo.fScale = pObj->getScale();
				setObjInfo.push_back(objInfo);
			}
		}
		size_t fileSize = setObjInfo.size()*sizeof(ObjInfo)+4;
		char* buffer = new char[fileSize];
		*((uint16*)buffer) = 1;//m_uUnknow;
		*((uint16*)(buffer+2)) = setObjInfo.size();
		if (setObjInfo.size()>0)
		{
			memcpy(buffer+4,&setObjInfo[0],setObjInfo.size()*sizeof(ObjInfo));
		}
		encrypt(buffer,fileSize);
		fwrite(buffer,fileSize,1,f);
		fclose(f);
		delete buffer;
	}
	return true;
}

int CMyPlug::exportData(iScene * pScene, const std::string& strFilename)
{
	exportTerrainData(&pScene->getTerrain()->GetData(),strFilename);
	exportObject(pScene,ChangeExtension(strFilename,".obj"));
	return true;
}

void CMyPlug::Release()
{
	delete this;
}