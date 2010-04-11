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
		const CNodeData* pPieceNode = file.firstChild("PIECE");
		while (pPieceNode)
		{
			uint64 uGUID;
			std::string strMeshFile;
			std::string strName;
			pPieceNode->GetVal("GUID",uGUID);
			pPieceNode->GetString("NAME",strName);
			pPieceNode->GetString("FILE",strMeshFile);
			pScene->setObjectResources(uGUID,strName,strPath+strMeshFile);
			pPieceNode = pPieceNode->nextSibling("PIECE");
		}
	}
	return true;
}



// Converts a Quaternion to Euler angles (X = Yaw, Y = Pitch, Z = Roll)
Vec3D QuaternionToEulerAngle(const Vec3D& vForward, const Vec3D& vRight, const Vec3D& vUp)
{
	Vec3D rot;
	float _11=vRight.x;
	float _21=vUp.x;
	float _31=vForward.x;
	float _32=vForward.y;
	float _33=vForward.z;

	rot.x = atan2( _21,_11 );
	rot.y = atan2(-_31,sqrt(_32*_32+_33*_33));
	rot.z = atan2( _32,_33 );
	return rot;
}

void importObjectByNodeData(iScene * pScene,const CNodeData* pParentNode)
{
	const CNodeData* pBaseObjectNode = pParentNode->firstChild("BASEOBJECT");
	while (pBaseObjectNode)
	{
		const CNodeData* pPropertiesNode = pBaseObjectNode->firstChild("PROPERTIES");
		if (pPropertiesNode)
		{
			uint64 uGUID;
			std::string strGUID;
			Vec3D vPos(0,0,0);
			Vec3D vRotate(0,0,0);
			float fScale = 1;
			pPropertiesNode->GetString("GUID",strGUID);
			if (strGUID.length()>0)
			{
				uGUID = _atoi64(strGUID.c_str());
				pPropertiesNode->GetVal("POSITIONX",vPos.x);
				pPropertiesNode->GetVal("POSITIONY",vPos.y);
				pPropertiesNode->GetVal("POSITIONZ",vPos.z);
				Vec3D vForward,vRight,vUp;
				pPropertiesNode->GetVal("FORWARDX",vForward.x);
				pPropertiesNode->GetVal("FORWARDY",vForward.y);
				pPropertiesNode->GetVal("FORWARDZ",vForward.z);
				pPropertiesNode->GetVal("RIGHTX",vRight.x);
				pPropertiesNode->GetVal("RIGHTY",vRight.y);
				pPropertiesNode->GetVal("RIGHTZ",vRight.z);
				pPropertiesNode->GetVal("UPX",vUp.x);
				pPropertiesNode->GetVal("UPY",vUp.y);
				pPropertiesNode->GetVal("UPZ",vUp.z);
				vForward.normalize();
				vRotate=QuaternionToEulerAngle(vForward,vRight,vUp);
				if (false==pScene->add3DMapSceneObj(uGUID,vPos,vRotate,fScale))
				{
					MessageBoxA(NULL,"cannot find ID!","Error",0);
				}
			}
		}
		const CNodeData* pChildrenNode = pBaseObjectNode->firstChild("CHILDREN");
		if (pChildrenNode)
		{
			importObjectByNodeData(pScene,pChildrenNode);
		}
		pBaseObjectNode = pBaseObjectNode->nextSibling("BASEOBJECT");
	}
}

bool CMyPlug::importObject(iScene * pScene, const std::string& strFilename)
{
	pScene->removeAllObjects();
	CStringNodeFile file;
	if (!file.LoadFile(strFilename))
	{
		return false;
	}
	if (file.GetName()!="Layout")
	{
		return false;
	}
	// OBJECTS
	const CNodeData* pObjectsNode = file.firstChild("OBJECTS");
	if (pObjectsNode==NULL)
	{
		return false;
	}
	// BASEOBJECT
	importObjectByNodeData(pScene, pObjectsNode);
	return true;
}

int CMyPlug::importData(iScene * pScene, const std::string& strFilename)
{
	importTileSet(pScene,"D:\\Program Files\\Runic Games\\TorchED\\media\\levelSets\\Cave.dat","D:\\Program Files\\Runic Games\\TorchED\\");
	importTileSet(pScene,"D:\\Program Files\\Runic Games\\TorchED\\media\\levelSets\\CaveProps.dat","D:\\Program Files\\Runic Games\\TorchED\\");
	importTileSet(pScene,"D:\\Program Files\\Runic Games\\TorchED\\media\\levelSets\\Crypt.dat","D:\\Program Files\\Runic Games\\TorchED\\");
	importTileSet(pScene,"D:\\Program Files\\Runic Games\\TorchED\\media\\levelSets\\CryptProps.dat","D:\\Program Files\\Runic Games\\TorchED\\");
	importTileSet(pScene,"D:\\Program Files\\Runic Games\\TorchED\\media\\levelSets\\ERROR.dat","D:\\Program Files\\Runic Games\\TorchED\\");
	importTileSet(pScene,"D:\\Program Files\\Runic Games\\TorchED\\media\\levelSets\\Fortress.dat","D:\\Program Files\\Runic Games\\TorchED\\");
	importTileSet(pScene,"D:\\Program Files\\Runic Games\\TorchED\\media\\levelSets\\FortressProps.dat","D:\\Program Files\\Runic Games\\TorchED\\");
	importTileSet(pScene,"D:\\Program Files\\Runic Games\\TorchED\\media\\levelSets\\Lava.dat","D:\\Program Files\\Runic Games\\TorchED\\");
	importTileSet(pScene,"D:\\Program Files\\Runic Games\\TorchED\\media\\levelSets\\LavaProps.dat","D:\\Program Files\\Runic Games\\TorchED\\");
	importTileSet(pScene,"D:\\Program Files\\Runic Games\\TorchED\\media\\levelSets\\Mine.dat","D:\\Program Files\\Runic Games\\TorchED\\");
	importTileSet(pScene,"D:\\Program Files\\Runic Games\\TorchED\\media\\levelSets\\MineProps.dat","D:\\Program Files\\Runic Games\\TorchED\\");
	importTileSet(pScene,"D:\\Program Files\\Runic Games\\TorchED\\media\\levelSets\\Palace.dat","D:\\Program Files\\Runic Games\\TorchED\\");
	importTileSet(pScene,"D:\\Program Files\\Runic Games\\TorchED\\media\\levelSets\\PalaceProps.dat","D:\\Program Files\\Runic Games\\TorchED\\");
	importTileSet(pScene,"D:\\Program Files\\Runic Games\\TorchED\\media\\levelSets\\Props.dat","D:\\Program Files\\Runic Games\\TorchED\\");
	importTileSet(pScene,"D:\\Program Files\\Runic Games\\TorchED\\media\\levelSets\\STemple.dat","D:\\Program Files\\Runic Games\\TorchED\\");
	importTileSet(pScene,"D:\\Program Files\\Runic Games\\TorchED\\media\\levelSets\\STempleProps.dat","D:\\Program Files\\Runic Games\\TorchED\\");
	importTileSet(pScene,"D:\\Program Files\\Runic Games\\TorchED\\media\\levelSets\\town1.dat","D:\\Program Files\\Runic Games\\TorchED\\");
	importTileSet(pScene,"D:\\Program Files\\Runic Games\\TorchED\\media\\levelSets\\town.dat","D:\\Program Files\\Runic Games\\TorchED\\");
	importTileSet(pScene,"D:\\Program Files\\Runic Games\\TorchED\\media\\levelSets\\town_02.dat","D:\\Program Files\\Runic Games\\TorchED\\");


	BBox bboxObject;
	bboxObject.vMin = Vec3D(-20.0f,-100.0f,-20.0f);
	bboxObject.vMax = Vec3D(pScene->getTerrain()->GetData().GetWidth()+20.0f,100.0f,pScene->getTerrain()->GetData().GetHeight()+20.0f);
	pScene->createObjectTree(bboxObject,16);
	importObject(pScene,strFilename);
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