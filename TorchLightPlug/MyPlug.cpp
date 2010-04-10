#include "MyPlug.h"
#include "IORead.h"
#include "FileSystem.h"
#include "LumpFile.h"
#include "3DMapSceneObj.h"
#include "StringNodeFile.h"

BOOL WINAPI Scene_Plug_CreateObject(void ** pobj)
{
	*pobj = new CMyPlug;
	return *pobj != NULL;
}

CMyPlug::CMyPlug(void)
{

}

CMyPlug::~CMyPlug(void)
{

}

int CMyPlug::Execute(iScene * pScene, bool bShowDlg, bool bSpecifyFileName)
{
	return -1;
}

bool CMyPlug::importTerrainData(iTerrainData * pTerrainData, const std::string& strFilename)
{
	return true;
}

bool CMyPlug::importTiles(iTerrain * pTerrain, const std::string& strFilename, const std::string& strPath)
{
	return true;
}

bool CMyPlug::importTileSet(iScene * pScene, const std::string& strFilename, const std::string& strPath)
{
	CStringNodeFile file;
	if (file.LoadFile(strFilename))
	{
		const CNodeData* pCellsNode = file.firstChild("PIECE");
		while (pCellsNode)
		{
			uint64 uID;
			std::string strMeshFile;
			std::string strName;
			pCellsNode->GetVal("GUID",uID);
			pCellsNode->GetVal("NAME",strName);
			pCellsNode->GetString("FILE",strMeshFile);
			pScene->setObjectResources(uID,,strPath+strMeshFile);
			pCellsNode = file.nextSibling("PIECE");
		}
	}
}

bool CMyPlug::importObjectResources(iScene * pScene, const std::string& strFilename, const std::string& strPath)
{
	pScene->clearObjectResources();
	CCsvFile csvObject;
	if (csvObject.Open(strFilename))
	{
		const CNodeData* pCellsNode = lump.firstChild("PIECE");
		while (pCellsNode)
		{
			pScene->setObjectResources(
				csvObject.GetInt("ID"),
				csvObject.GetStr("Name"),
				getRealFilename(strPath,csvObject.GetStr("Filename")));
				//Info.bbox				= 
				//Info.bIsGround			= csvObject.GetBool("IsGround");
				//Info.bHasShadow			= csvObject.GetBool("HasShadow");
				//Info.strFilename	= csvObject.GetStr("ModelFilename");
			pCellsNode = lump.nextSibling("PIECE");
		}
		csvObject.Close();
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

int CMyPlug::importData(iScene * pScene, const std::string& strFilename)
{

	importObjectResources(pScene,GetParentPath(strFilename)+"object.csv",GetParentPath(strFilename)); 
	BBox bboxObject;
	bboxObject.vMin = Vec3D(-20.0f,-100.0f,-20.0f);
	bboxObject.vMax = Vec3D(pScene->getTerrain()->GetData().GetWidth()+20.0f,100.0f,pScene->getTerrain()->GetData().GetHeight()+20.0f);
	pScene->createObjectTree(bboxObject,16);
	importObject(pScene,ChangeExtension(strFilename,".obj"));
	return true;
}

bool CMyPlug::exportTerrainData(iTerrainData * pTerrainData, const std::string& strFilename)
{
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
	exportObject(pScene,ChangeExtension(strFilename,".obj"));
	return true;
}

void CMyPlug::Release()
{
	delete this;
}