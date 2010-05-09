#include "MyPlug.h"
#include "IORead.h"
#include "FileSystem.h"
#include "MUBmd.h"
#include "Material.h"
#include "CSVFile.h"

CMyPlug::CMyPlug(void)
{
}

CMyPlug::~CMyPlug(void)
{
}

int CMyPlug::Execute(iModelData * pModelData, bool bShowDlg, bool bSpecifyFileName)
{
	return -1;
}

bool importMaterial(iModelData * pModelData, const std::string& strFilename, const std::string& strPath)
{
	CCsvFile csv;
	if (!csv.Open(strFilename))
	{
		return false;
	}
	while (csv.SeekNextLine())
	{
		const std::string strMaterialName = csv.GetStr("Name");
		CMaterial& material = pModelData->getMaterial(strMaterialName);

		material.setDiffuse		(getRealFilename(strPath,csv.GetStr("Diffuse")));
		material.setEmissive	(getRealFilename(strPath,csv.GetStr("Emissive")));
		material.setSpecular	(getRealFilename(strPath,csv.GetStr("Specular")));
		material.setNormal		(getRealFilename(strPath,csv.GetStr("Normal")));
		material.setReflection	(getRealFilename(strPath,csv.GetStr("Reflection")));
		material.setLightMap	(getRealFilename(strPath,csv.GetStr("LightMap")));
		material.setShader		(getRealFilename(strPath,csv.GetStr("Shader")));

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

bool CMyPlug::importData(iModelData * pModelData, const std::string& strFilename)
{
	assert(pModelData);
	static CMUBmd* pPlayerBmd;
	CMUBmd bmd;
	bool bIsPlayerPart = false;
	if (!bmd.LoadFile(strFilename))
	{
		return false;
	}
	if (GetFilename(strFilename)!="player.bmd")
	{
		std::string str = GetFilename(GetParentPath(strFilename));
		if (str=="player")
		{
			if (pPlayerBmd)
			{
				bIsPlayerPart= true;
			}
			else
			{
				pPlayerBmd = new CMUBmd;
				bIsPlayerPart = pPlayerBmd->LoadFile(GetParentPath(strFilename)+"player.bmd");
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	iLodMesh& mesh = pModelData->getMesh();
	//m_Mesh.m_Lods.resize(1);
	if (bmd.nFrameCount>1)// if there one frame only, free the animlist
	{
		int nFrameCount = 0;
		for (size_t uAnimID=0; uAnimID<bmd.head.uAnimCount; ++uAnimID)
		{
			long uTotalFrames = bmd.bmdSkeleton.setBmdAnim[uAnimID].uFrameCount;
			
			long timeEnd = nFrameCount;

			std::string strAnimName;
			{
				char szID[256]={0};
				itoa(uAnimID,szID,10);
				strAnimName=szID;
			}
			switch(uAnimID)
			{
			case 0:strAnimName+="Idle0";
				break;
			case 1:strAnimName+="Idle1";
				break;
			case 2:strAnimName+="Walk";
				break;
			case 3:strAnimName+="Attack0";
				break;
			case 4:strAnimName+="Attack1";
				break;
			case 5:strAnimName+="Hurt";
				break;
			case 6:strAnimName+="Dead";
				break;
			default:strAnimName+="Unknown";
				break;
			}
			
			std::vector<CMUBmd::BmdSkeleton::BmdBone>& setBmdBone = bmd.bmdSkeleton.setBmdBone;
			size_t uBoneSize = setBmdBone.size();
			SkeletonAnim& skeletonAnim=pModelData->getSkeleton().m_Anims[strAnimName];
			std::vector<BoneAnim>& setBonesAnim=skeletonAnim.setBonesAnim;
			setBonesAnim.resize(uBoneSize);
			for (size_t uBoneID = 0;uBoneID<uBoneSize;++uBoneID)
			{
				CMUBmd::BmdSkeleton::BmdBone& bmdBone=setBmdBone[uBoneID];
				if (!bmdBone.bEmpty)
				{
					BoneAnim& bonsAnim = setBonesAnim[uBoneID];
					for (size_t i=0;i<uTotalFrames;++i)
					{
						bonsAnim.trans.addValue(i*MU_BMD_ANIM_FRAME_TIME,fixCoordSystemPos(bmdBone.setTrans[i+nFrameCount]));// 可以设置关键帧播放速度来调控
						bonsAnim.rot.addValue(i*MU_BMD_ANIM_FRAME_TIME,fixCoordSystemRotate(bmdBone.setRotate[i+nFrameCount]));
					}
				}
			}
			nFrameCount+=uTotalFrames;
			skeletonAnim.uTotalFrames = (uTotalFrames-1)*MU_BMD_ANIM_FRAME_TIME;
		}
	}

	for (size_t i=0;  i<bmd.setBmdSub.size(); ++i)
	{
		CMUBmd::BmdSub& bmdSub = bmd.setBmdSub[i];
		iLodMesh& mesh = pModelData->getMesh();
		CSubMesh& subMesh=mesh.addSubMesh();
		VertexIndex vertexIndex;
		for(std::vector<CMUBmd::BmdSub::BmdTriangle>::iterator it=bmdSub.setTriangle.begin(); it!=bmdSub.setTriangle.end(); it++)
		{
			for (size_t j=0; j<3; ++j)
			{
				vertexIndex.p	= /*mesh.getPosCount()+*/it->indexVertex[2-j];
				vertexIndex.b	= /*mesh.getBoneCount()+*/it->indexVertex[2-j];
				vertexIndex.w	= /*mesh.getWeightCount()+*/it->indexVertex[2-j];
				vertexIndex.n	= /*mesh.getNormalCount()+*/it->indexNormal[2-j];
				vertexIndex.uv1	= /*mesh.getTexcoordCount()+*/it->indexUV[2-j];
				subMesh.m_setVertexIndex.push_back(vertexIndex);
			}
		}
		for(std::vector<CMUBmd::BmdSub::BmdPos>::iterator it=bmdSub.setVertex.begin(); it!=bmdSub.setVertex.end(); it++)
		{
			Vec3D vPos = fixCoordSystemPos(it->vPos);
			if (bIsPlayerPart)
			{
				vPos = pPlayerBmd->bmdSkeleton.getLocalMatrix(it->uBones)*vPos;
			}
			else
			{
				vPos = bmd.bmdSkeleton.getLocalMatrix(it->uBones)*vPos;
			}
			if (1<bmd.nFrameCount||bIsPlayerPart)
			{
				uint8 uBone = it->uBones&0xFF;
				if (bmd.bmdSkeleton.setBmdBone.size()<=uBone||bmd.bmdSkeleton.setBmdBone[uBone].bEmpty)
				{
					subMesh.addBone(0);
				}
				else
				{
					subMesh.addBone(it->uBones);
				}
				//assert((it->uBones&0xFFFFFF00)==0);
				subMesh.addWeight(0x000000FF);
			}
			subMesh.addPos(vPos);
		}
		for(std::vector<CMUBmd::BmdSub::BmdNormal>::iterator it=bmdSub.setNormal.begin(); it!=bmdSub.setNormal.end(); it++)
		{
			Vec3D n = fixCoordSystemNormal(it->vNormal);
			n = bmd.bmdSkeleton.getRotateMatrix(it->uBones)*n;
			subMesh.addNormal(n);
		}
		for(std::vector<Vec2D>::iterator it=bmdSub.setUV.begin(); it!=bmdSub.setUV.end(); it++)
		{
			subMesh.addTexcoord(*it);
		}
		{
			std::string strTexFileName = GetParentPath(strFilename) + bmdSub.szTexture;
			{
				std::string strExtension = GetExtension(bmdSub.szTexture); 
				if (".jpg"==strExtension)
					strTexFileName = ChangeExtension(strTexFileName,".ozj");
				else if (".tga"==strExtension)
					strTexFileName = ChangeExtension(strTexFileName,".ozt");
				else if (".bmp"==strExtension)
					strTexFileName = ChangeExtension(strTexFileName,".ozb");
			}
			std::string strMaterialName = Format("%s%d",ChangeExtension(GetFilename(strFilename),".sub").c_str(),i);
			{
				CMaterial& material = pModelData->getMaterial(strMaterialName);
				material.setDiffuse(strTexFileName);
				material.bLightingEnabled=true;
				material.uCull = 0;
				material.bAlphaTest=true;
				material.uAlphaTestValue = 0x80;
			}
			pModelData->setRenderPass(i,i,strMaterialName);
		}
	}

	std::vector<BoneInfo>& setBonesInfo = pModelData->getSkeleton().m_Bones;
	for (std::vector<CMUBmd::BmdSkeleton::BmdBone>::iterator itBmdBone=bmd.bmdSkeleton.setBmdBone.begin();itBmdBone!=bmd.bmdSkeleton.setBmdBone.end();itBmdBone++)
	{
		BoneInfo boneInfo;
		if (!itBmdBone->bEmpty)
		{
			boneInfo.strName = itBmdBone->szName;
			Matrix	mInvLocal = itBmdBone->mLocal;
			mInvLocal.Invert();
			boneInfo.mInvLocal = mInvLocal;

			int nParent = itBmdBone->nParent;
			if (nParent<0||nParent>255)
			{
				nParent = 255;
			}
			boneInfo.parent=nParent;
		}
		setBonesInfo.push_back(boneInfo);
	}

	mesh.update();

	std::string strParentDir = GetParentPath(strFilename);
	std::string strParentDirName = GetFilename(strParentDir);
	
	std::string strMyPath ="Data\\"+strParentDirName+"\\";
	std::string strMatFilename = strMyPath+GetFilename(ChangeExtension(strFilename,".mat.csv"));
	std::string strParFilename = strMyPath+GetFilename(ChangeExtension(strFilename,".par.csv"));
	if (!IOReadBase::Exists(strMatFilename))
	{
		strMatFilename=strMyPath+strParentDirName+".mat.csv";
	}
	if (!IOReadBase::Exists(strParFilename))
	{
		strParFilename=strMyPath+strParentDirName+".par.csv";
	}

	importMaterial(pModelData, strMatFilename, strParentDir);
	pModelData->loadParticleEmitters(strParFilename);
	return true;
}

bool CMyPlug::exportData(iModelData * pModelData, const std::string& strFilename)
{
	assert(pModelData);
	FILE* fp= fopen(strFilename.c_str(),"wb");
	if (fp==NULL)
	{
		return false;
	}
	// tag
	uint32 uTag=0x0a444d42;
	fwrite(&uTag,4,1,fp);
	// head
	CMUBmd::BmdHead bmdHead;
	strcpy(bmdHead.strFile,GetFilename(pModelData->getItemName()).c_str());
	bmdHead.uSubCount=pModelData->getMesh().getSubCount();
	bmdHead.uBoneCount=pModelData->getSkeleton().m_Bones.size();
	bmdHead.uAnimCount=pModelData->getSkeleton().m_Bones.size();
	fwrite(&bmdHead,sizeof(CMUBmd::BmdHead),1,fp);
	// sub
	for (size_t i=0; i<bmdHead.uSubCount; ++i)
	{
		//size_t uFaceIndexCount=getFaceIndexCount(i);
		//// sub head
		//CMUBmd::BmdSub::BmdSubHead subHead;
		//subHead.uVertexCount=uFaceIndexCount*3;
		//subHead.uNormal=uFaceIndexCount*3;
		//subHead.uUVCount=uFaceIndexCount*3;
		//subHead.uTriangleCount=uFaceIndexCount*3;

		//fwrite(&bmdHead,sizeof(CMUBmd::BmdHead),1,fp);
		//for (size_t n=0;n<uFaceIndexCount;++n)
		//{
		//	if (getFaceIndex(i,n,faceIndex))
		//	{
		//	}
		//}
		//setBmdSub[i].load(s);
	}

	return true;
}

void CMyPlug::Release()
{
	delete this;
}