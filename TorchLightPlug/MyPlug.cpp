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
	CStringNodeFile file;
	if (file.LoadFile(strFilename))
	{
		const CNodeData* pCellsNode = file.firstChild("BASEOBJECT");
		while (pCellsNode)
		{
			uint64 uID;
			std::string strMeshFile;
			std::string strName;
			pCellsNode->GetVal("GUID",uID);
			pCellsNode->GetVal("NAME",strName);
			pCellsNode->GetString("FILE",strMeshFile);
			pScene->setObjectResources(uID,,strPath+strMeshFile);
			if (false==pScene->add3DMapSceneObj(pObjInfo->id,vPos,vRotate,pObjInfo->fScale))
			{
				//MessageBoxA(NULL,"cannot find ID!","Error",0);
			}
			pCellsNode = file.nextSibling("BASEOBJECT");
		}
	}
	return true;
}

int CMyPlug::importData(iScene * pScene, const std::string& strFilename)
{
	importTileSet(pScene,"D:\\Program Files\\Runic Games\\TorchED\\media\\levelSets\\Cave.dat","D:\\Program Files\\Runic Games\\TorchED\\");
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