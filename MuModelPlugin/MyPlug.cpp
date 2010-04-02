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

		material.strDiffuse		=getRealFilename(strPath,csv.GetStr("Diffuse"));
		material.strEmissive	=getRealFilename(strPath,csv.GetStr("Emissive"));
		material.strSpecular	=getRealFilename(strPath,csv.GetStr("Specular"));
		material.strNormal		=getRealFilename(strPath,csv.GetStr("Normal"));
		material.strReflection	=getRealFilename(strPath,csv.GetStr("Reflection"));
		material.strLightMap	=getRealFilename(strPath,csv.GetStr("LightMap"));
		material.strShader		=getRealFilename(strPath,csv.GetStr("Shader"));

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
		for (size_t i=0; i<bmd.head.uAnimCount; ++i)
		{
			long timeStart = nFrameCount*MU_BMD_ANIM_FRAME_TIME;
			nFrameCount+=bmd.bmdSkeleton.setBmdAnim[i].uFrameCount+1;
			long timeEnd = (nFrameCount-1)*MU_BMD_ANIM_FRAME_TIME;

			std::string strAnimName;
			{
				char szID[256]={0};
				itoa(i,szID,10);
				strAnimName=szID;
			}
			switch(i)
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
			pModelData->setAnimation(strAnimName.c_str(),timeStart,timeEnd);
		}
	}

	for (size_t i=0;  i<bmd.setBmdSub.size(); ++i)
	{
		CMUBmd::BmdSub& bmdSub = bmd.setBmdSub[i];
		iLodMesh& mesh = pModelData->getMesh();
		FaceIndex faceIndex;
		for(std::vector<CMUBmd::BmdSub::BmdTriangle>::iterator it=bmdSub.setTriangle.begin(); it!=bmdSub.setTriangle.end(); it++)
		{
			for (size_t j=0; j<3; ++j)
			{
				faceIndex.v[j]	= mesh.getPosCount()+it->indexVertex[2-j];
				faceIndex.b[j]	= mesh.getBoneCount()+it->indexVertex[2-j];
				faceIndex.w[j]	= mesh.getWeightCount()+it->indexVertex[2-j];
				faceIndex.n[j]	= mesh.getNormalCount()+it->indexNormal[2-j];
				faceIndex.uv1[j]= mesh.getTexcoordCount()+it->indexUV[2-j];
			}
			mesh.addFaceIndex(i,faceIndex);
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
					mesh.addBone(0);
				}
				else
				{
					mesh.addBone(it->uBones);
				}
				//assert((it->uBones&0xFFFFFF00)==0);
				mesh.addWeight(0x000000FF);
			}
			mesh.addPos(vPos);
		}
		for(std::vector<CMUBmd::BmdSub::BmdNormal>::iterator it=bmdSub.setNormal.begin(); it!=bmdSub.setNormal.end(); it++)
		{
			Vec3D n = fixCoordSystemNormal(it->vNormal);
			n = bmd.bmdSkeleton.getRotateMatrix(it->uBones)*n;
			mesh.addNormal(n);
		}
		for(std::vector<Vec2D>::iterator it=bmdSub.setUV.begin(); it!=bmdSub.setUV.end(); it++)
		{
			mesh.addTexcoord(*it);
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
				material.strDiffuse=strTexFileName;
				material.uCull = 0;
				material.bAlphaTest=true;
				material.uAlphaTestValue = 0x80;
			}
			pModelData->setRenderPass(i,i,strMaterialName);
		}
	}

	iSkeleton& skeleton = pModelData->getSkeleton();
	skeleton.m_BoneAnims.resize(bmd.head.uBoneCount);

	std::vector<BoneAnim>::iterator itBoneAnim = skeleton.m_BoneAnims.begin();
	for (std::vector<CMUBmd::BmdSkeleton::BmdBone>::iterator itBmdBone=bmd.bmdSkeleton.setBmdBone.begin();itBmdBone!=bmd.bmdSkeleton.setBmdBone.end();itBmdBone++)
	{
		if (!itBmdBone->bEmpty)
		{
			itBoneAnim->strName = itBmdBone->szName;
			Matrix	mInvLocal = itBmdBone->mLocal;
			mInvLocal.Invert();
			itBoneAnim->mSkin = mInvLocal;
			unsigned int uTime =0;
			for (std::vector<Vec3D>::iterator it= itBmdBone->setTrans.begin();
				it!=itBmdBone->setTrans.end(); it++)
			{
				if (GetFilename(strFilename)=="player.bmd"&&itBmdBone==bmd.bmdSkeleton.setBmdBone.begin())
				{
					/*if (uTime>m_AnimList[15].timeStart&&uTime<m_AnimList[23].timeStart)
					{
					Vec3D vPos = fixCoordSystemPos(*it);
					vPos.x=0;vPos.z=0;
					itBoneAnim->trans.addValue(uTime,vPos);
					uTime += MU_BMD_ANIM_FRAME_TIME;
					continue;
					}*/
				}
				itBoneAnim->trans.addValue(uTime,fixCoordSystemPos(*it));
				uTime += MU_BMD_ANIM_FRAME_TIME;
			}
			uTime =0;
			for (std::vector<Vec3D>::iterator it= itBmdBone->setRotate.begin();
				it!=itBmdBone->setRotate.end(); it++)
			{
				itBoneAnim->rot.addValue(uTime,fixCoordSystemRotate(*it));
				uTime += MU_BMD_ANIM_FRAME_TIME;
			}
			int nParent = itBmdBone->nParent;
			if (nParent<0||nParent>255)
			{
				nParent = 255;
			}
			itBoneAnim->parent=nParent;	
		}
		itBoneAnim++;
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
	bmdHead.uBoneCount=pModelData->getSkeleton().m_BoneAnims.size();
	bmdHead.uAnimCount=pModelData->getSkeleton().m_BoneAnims.size();
	fwrite(&bmdHead,sizeof(CMUBmd::BmdHead),1,fp);
	// sub
	FaceIndex faceIndex;
	for (size_t i=0; i<bmdHead.uSubCount; ++i)
	{
		size_t uFaceIndexCount=getFaceIndexCount(i);
		// sub head
		CMUBmd::BmdSub::BmdSubHead subHead;
		subHead.uVertexCount=uFaceIndexCount*3;
		subHead.uNormal=uFaceIndexCount*3;
		subHead.uUVCount=uFaceIndexCount*3;
		subHead.uTriangleCount=uFaceIndexCount*3;

		fwrite(&bmdHead,sizeof(CMUBmd::BmdHead),1,fp);
		for (size_t n=0;n<uFaceIndexCount;++n)
		{
			if (getFaceIndex(i,n,faceIndex))
			{
			}
		}
		//setBmdSub[i].load(s);
	}

	return true;
}

void CMyPlug::Release()
{
	delete this;
}