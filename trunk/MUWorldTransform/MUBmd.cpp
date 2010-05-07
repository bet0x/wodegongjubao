#include "MUBmd.h"
#include "DecryptFuncs.h"
#include "FileSystem.h"

void CMUBmd::BmdSkeleton::BmdAnim::load(CMemoryStream& s)
{
	s.read(uFrameCount);
	s.read(bOffset);
	if (bOffset)
	{
		s.readVector(vOffset,uFrameCount);
	}
}

void CMUBmd::BmdSkeleton::BmdBone::load(CMemoryStream& s, const std::vector<BmdAnim>& setBmdAnim)
{
	s.read(bEmpty);
	if (bEmpty)
	{
		return;
	}
	s.read((unsigned char*)szName,32);
	s.read(nParent);
	for (size_t i=0; i<setBmdAnim.size();++i)
	{
		for (size_t j=0; j<setBmdAnim[i].uFrameCount;++j)
		{
			Vec3D vTrans;
			s.read(vTrans);
			setTrans.push_back(vTrans);
		}
		for (size_t j=0; j<setBmdAnim[i].uFrameCount;++j)
		{
			Vec3D vRotate;
			s.read(vRotate);
			setRotate.push_back(vRotate);
		}
	}
}

Matrix CMUBmd::BmdSkeleton::getLocalMatrix(uint8 uBoneID)
{
	if (setBmdBone.size()>uBoneID)
	{
		return setBmdBone[uBoneID].mLocal;
	}
	return Matrix::UNIT;
}

Matrix CMUBmd::BmdSkeleton::getRotateMatrix(uint8 uBoneID)
{
	Matrix mRotate=getLocalMatrix(uBoneID);
	mRotate._14=0;mRotate._24=0;mRotate._34=0;
	return mRotate;
}

//左右手转换就是 z轴负一下
//Vec3D(v.x, v.y, -v.z)
//q.rotate(Vec3D(v.x,v.y,-v.z));
//
//然后剩下的就是 向上的问题
//mu是z向上， 换到我那里 就得y向上
//Vec3D(v.x, -v.z, v.y)
//Quaternion(q.x, -q.z, q.y, q.w)
//
//俩一合 就是
//Vec3D(v.x, v.z, v.y)
//
//Quaternion q;
//q.rotate(Vec3D(v.x,v.y,-v.z));
//return Quaternion(q.x, -q.z, q.y, q.w);

inline Vec3D fixCoordSystemPos(Vec3D v)
{
	return Vec3D(v.x, v.y, -v.z);
}
inline Vec3D fixCoordSystemNormal(Vec3D v)
{
	return Vec3D(v.x, v.y, -v.z);
}

inline Quaternion fixCoordSystemRotate(Vec3D v)
{
	Quaternion q;
	q.rotate(Vec3D(v.x,v.y,-v.z));
	return Quaternion(q);
}

void CMUBmd::BmdSkeleton::calcLocalMatrix(uint32 uBoneID)
{
	//m_bCalc
	if (setBmdBone.size()<=uBoneID)
	{
		return;
	}
	BmdBone& b = setBmdBone[uBoneID];
	Matrix m = Matrix::UNIT;
	if (b.setTrans.size()>0)
	{
		m *= Matrix::newTranslation(fixCoordSystemPos(b.setTrans[0]));
	}
	if (b.setRotate.size()>0)
	{
		m *= Matrix::newQuatRotate(fixCoordSystemRotate(b.setRotate[0]));
	}
	if (b.nParent==-1)
	{
		b.mLocal = m;
	}
	else
	{
		calcLocalMatrix(b.nParent);
		b.mLocal = getLocalMatrix(b.nParent)*m;
	}
}

void CMUBmd::BmdSkeleton::getLocalMatrix(std::vector<Matrix>& setLocalMatrix)
{
	setLocalMatrix.resize(setBmdBone.size());
	for (size_t i=0;i<setBmdBone.size();++i)
	{
		calcLocalMatrix(i);
		setLocalMatrix[i]=setBmdBone[i].mLocal;
	}
}

void CMUBmd::BmdSkeleton:: load(CMemoryStream& s, uint16 uBoneCount, uint16 uAnimCount)
{
	setBmdAnim.resize(uAnimCount);
	setBmdBone.resize(uBoneCount);
	for (size_t i=0; i<setBmdAnim.size();++i)
	{
		setBmdAnim[i].load(s);
	}
	for (size_t i=0; i<setBmdBone.size();++i)
	{
		setBmdBone[i].load(s,setBmdAnim);
	}
}

void CMUBmd::BmdSub::load(CMemoryStream& s)
{
	s.read(head);

	s.readVector(setVertex,head.uVertexCount);
	s.readVector(setNormal,head.uNormal);
	s.readVector(setUV,head.uUVCount);
	s.readVector(setTriangle,head.uTriangleCount);

	s.read((unsigned char*)szTexture,32);
}

void CMUBmd::BmdSub::skinMesh(std::vector<Matrix>& setBoneMatrix)
{
	for (size_t i=0;i<setVertex.size();++i)
	{
		setVertex[i].vPos=fixCoordSystemPos(setVertex[i].vPos);
		setVertex[i].vPos=setBoneMatrix[setVertex[i].uBones]*setVertex[i].vPos;
		setVertex[i].vPos=fixCoordSystemPos(setVertex[i].vPos);
		//setVertex[i].vPos.z=-setVertex[i].vPos.z;
	}
	for (size_t i=0;i<setNormal.size();++i)
	{
		setNormal[i].vNormal=fixCoordSystemNormal(setNormal[i].vNormal);
		Matrix mRotate=setBoneMatrix[setNormal[i].uBones];
		mRotate._14=0;mRotate._24=0;mRotate._34=0;
		setNormal[i].vNormal=mRotate*setNormal[i].vNormal;
		setNormal[i].vNormal=fixCoordSystemNormal(setNormal[i].vNormal);// 导出时 再把它纠正成openGl坐标系
		//setNormal[i].vNormal.z=-setNormal[i].vNormal.z;

	}
}

bool CMUBmd::loadFormBmd(const std::string& strFilename)
{
	IOReadBase* pRead = IOReadBase::autoOpen(strFilename);
	if (!pRead)
	{
		return false;
	}
	size_t uFileSize = pRead->GetSize();
	CMemoryStream s;
	s.resize(uFileSize);

	pRead->Read(s.getBuffer(),uFileSize);
	IOReadBase::autoClose(pRead);

	uint32 uTag;
	s.read(uTag);
	if (0x0a444d42==uTag)//BMD.
	{
	}
	else if (0x0c444d42==uTag)//BMD.
	{
		uint32 uEncodeSize;
		s.read(uEncodeSize);
		if (uEncodeSize!=uFileSize-8)
		{
			return false;
		}
		decryptMuBuffer(s.getPt(), uFileSize-8);
	}
	else 
	{
		return false;
	}

	s.read(head);
	assert(head.uSubCount<50);
	setBmdSub.resize(head.uSubCount);
	for (size_t i=0; i<setBmdSub.size(); ++i)
	{
		setBmdSub[i].load(s);
	}
	bmdSkeleton.load(s, head.uBoneCount,head.uAnimCount);
	// LocalMatrix
	std::vector<Matrix> setLocalMatrix;
	bmdSkeleton.getLocalMatrix(setLocalMatrix);
	for (size_t i=0;i<setBmdSub.size();++i)
	{
		setBmdSub[i].skinMesh(setLocalMatrix);
	}

	return true;
}

bool CMUBmd::loadFormSmd(const std::string& strFilename)
{
	return false;
}

void CMUBmd::saveToBmd(const std::string& strFilename)
{
	FILE* f=fopen(strFilename.c_str(),"wb+");
	if (NULL==f)
	{
		return;
	}
	// Tag
	uint32 uTag=0x0a444d42;//BMD.
	fwrite(&uTag,4,1,f);
	// Head
	fwrite(&head,sizeof(head),1,f);
	// Sub Mesh
	for (size_t i=0; i<setBmdSub.size(); ++i)
	{
		//setBmdSub[i].load(s);
	}
}
#include<iostream>
#include<iomanip>
#include<fstream>
void CMUBmd::saveToSmd(const std::string& strFilename)
{
	std::ofstream file;
	file.open(strFilename.c_str(), std::ios::out);
	if ( file.is_open() )
	{
		file << "version 1"<< std::endl;
		file << "nodes"<< std::endl;
		for (size_t i=0;i<bmdSkeleton.setBmdBone.size();++i)
		{
			BmdSkeleton::BmdBone& bmdBone = bmdSkeleton.setBmdBone[i];
			if (!bmdBone.bEmpty)
			{
				file<<i<<" \""<<bmdBone.szName<<"\" "<<bmdBone.nParent<<std::endl;
			}
		}
		file << "end"<< std::endl;
		file << "skeleton"<< std::endl;
		file << "time 0"<< std::endl;
		for (size_t i=0;i<bmdSkeleton.setBmdBone.size();++i)
		{
			BmdSkeleton::BmdBone& bmdBone = bmdSkeleton.setBmdBone[i];
			if (!bmdBone.bEmpty)
			{
				Vec3D& vTrans = bmdBone.setTrans[0];
				Vec3D& vRotate = bmdBone.setRotate[0];
				file<<i<<" "<<vTrans.x<<" "<<vTrans.y<<" "<<vTrans.z<<" "<<vRotate.x<<" "<<vRotate.y<<" "<<vRotate.z<<std::endl;
			}
		}
		file << "end"<< std::endl;
		// triangles
		file << "triangles"<< std::endl;
		for (size_t i=0;i<setBmdSub.size();++i)
		{
			BmdSub& bmdSub = setBmdSub[i];
			size_t uBmdTriangleSize = bmdSub.setTriangle.size();
			for (size_t j=0;j<uBmdTriangleSize;++j)
			{
				BmdSub::BmdTriangle& bmdTriangle = bmdSub.setTriangle[j];
				file << bmdSub.szTexture << std::endl;
				for (size_t n=0;n<3;++n)
				{
					int nPos = bmdTriangle.indexVertex[n];
					int nNormal = bmdTriangle.indexNormal[n];
					int nUV = bmdTriangle.indexUV[n];

					int nBone = bmdSub.setVertex[nPos].uBones;
					Vec3D vPos = bmdSub.setVertex[nPos].vPos;
					Vec3D vNormal = bmdSub.setNormal[nNormal].vNormal;
					Vec2D vUV = bmdSub.setUV[nUV];
					// fix
					vUV.y = 1.0f-vUV.y;

					file<<nBone<<" "<<vPos.x<<" "<<vPos.y<<" "<<vPos.z<<" "<<vNormal.x<<" "<<vNormal.y<<" "<<vNormal.z<<" "<<vUV.x<<" "<<vUV.y<<std::endl;
				}
			}
		}
		file << "end";
	}
	file.close();
	//////////////////////////////////////////////////////////////////////////
	int nFrameIndex=0;
	for (size_t animID=0;animID<bmdSkeleton.setBmdAnim.size();++animID)
	{
		char szAnimName [256];
		std::string strAnimFilename;
		sprintf (szAnimName, "_%03d.smd", animID+1);
		strAnimFilename = ChangeExtension(strFilename,szAnimName);
		file.open(strAnimFilename.c_str(), std::ios::out);
		if ( file.is_open() )
		{
			file << "version 1"<< std::endl;
			file << "nodes"<< std::endl;
			for (size_t boneID=0;boneID<bmdSkeleton.setBmdBone.size();++boneID)
			{
				BmdSkeleton::BmdBone& bmdBone = bmdSkeleton.setBmdBone[boneID];
				if (!bmdBone.bEmpty)
				{
					file<<boneID<<" \""<<bmdBone.szName<<"\" "<<bmdBone.nParent<<std::endl;
				}
			}
			file << "end"<< std::endl;
			file << "skeleton"<< std::endl;

			size_t uFrameCount = bmdSkeleton.setBmdAnim[animID].uFrameCount;
			for (size_t frameID=0;frameID<uFrameCount;++frameID)
			{
				file<<"time "<<frameID<<std::endl;
				for (size_t boneID=0;boneID<bmdSkeleton.setBmdBone.size();++boneID)
				{
					BmdSkeleton::BmdBone& bmdBone = bmdSkeleton.setBmdBone[boneID];
					if (!bmdBone.bEmpty)
					{
						Vec3D& vTrans = bmdBone.setTrans[nFrameIndex];
						Vec3D& vRotate = bmdBone.setRotate[nFrameIndex];
						file<<boneID<<" "<<vTrans.x<<" "<<vTrans.y<<" "<<vTrans.z<<" "<<vRotate.x<<" "<<vRotate.y<<" "<<vRotate.z<<std::endl;
					}
				}
				++nFrameIndex;
			}
			file << "end";
		}
		file.close();
	}
}

void Bmd2Smd(const std::string& strSrcFilename, const std::string& strDestFilename)
{
	CMUBmd bmd;
	bmd.loadFormBmd(strSrcFilename);
	bmd.saveToSmd(strDestFilename);
}

void Smd2Bmd(const std::string& strSrcFilename, const std::string& strDestFilename)
{
	CMUBmd bmd;
	bmd.loadFormSmd(strSrcFilename);
	bmd.saveToBmd(strDestFilename);
}