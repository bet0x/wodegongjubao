#include "MUBmd.h"
#include "DecryptFuncs.h"


void CMUBmd::BmdSkeleton::BmdAnim::load(CMemoryStream& s)
{
	s.getBuffer(uFrameCount);
	s.getBuffer(bOffset);
	if (bOffset)
	{
		s.getVector(vOffset,uFrameCount);
	}
}

void CMUBmd::BmdSkeleton::BmdBone::load(CMemoryStream& s, const std::vector<BmdAnim>& setBmdAnim)
{
	s.getBuffer(bEmpty);
	if (bEmpty)
	{
		return;
	}
	s.getBuffer((unsigned char*)szName,32);
	s.getBuffer(nParent);
	for (size_t i=0; i<setBmdAnim.size();++i)
	{
		for (size_t j=0; j<setBmdAnim[i].uFrameCount;++j)
		{
			Vec3D vTrans;
			s.getBuffer(vTrans);
			setTrans.push_back(vTrans);
		}
		for (size_t j=0; j<setBmdAnim[i].uFrameCount;++j)
		{
			Vec3D vRotate;
			s.getBuffer(vRotate);
			setRotate.push_back(vRotate);
		}

		if (setBmdAnim[i].uFrameCount>1)
		{
			setTrans.push_back(setTrans[setTrans.size()-setBmdAnim[i].uFrameCount]);
			setRotate.push_back(setRotate[setRotate.size()-setBmdAnim[i].uFrameCount]);
		}
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
	s.getBuffer(head);

	s.getVector(setVertex,head.uVertexCount);
	s.getVector(setNormal,head.uNormal);
	s.getVector(setUV,head.uUVCount);
	s.getVector(setTriangle,head.uTriangleCount);

	s.getBuffer((unsigned char*)szTexture,32);
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
	s.getBuffer(uTag);
	if (0x0a444d42==uTag)//BMD.
	{
	}
	else if (0x0c444d42==uTag)//BMD.
	{
		uint32 uEncodeSize;
		s.getBuffer(uEncodeSize);
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

	s.getBuffer(head);
	assert(head.uSubCount<50);
	setBmdSub.resize(head.uSubCount);
	for (size_t i=0; i<setBmdSub.size(); ++i)
	{
		setBmdSub[i].load(s);
	}
	bmdSkeleton.load(s, head.uBoneCount,head.uAnimCount);

	nFrameCount = 0;
	for (size_t i=0; i<head.uAnimCount; ++i)
	{
		nFrameCount+=bmdSkeleton.setBmdAnim[i].uFrameCount;
	}
	return true;
}

bool CMUBmd::loadFormSmd(const std::string& strFilename)
{
	return false;
}

void CMUBmd::saveToBmd(const std::string& strFilename)
{

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
		file << "end"<< std::endl;
		file << "skeleton"<< std::endl;
		file << "end"<< std::endl;

		file << "triangles"<< std::endl;
		for (size_t i=0;i<setBmdSub.size();++i)
		{
			for (size_t j=0;j<setBmdSub[i].setTriangle.size();++j)
			{
				file << setBmdSub[i].szTexture << std::endl;
				for (size_t n=0;n<3;++n)
				{
					int nPos = setBmdSub[i].setTriangle[j].indexVertex[n];
					int nNormal = setBmdSub[i].setTriangle[j].indexNormal[n];
					int nUV = setBmdSub[i].setTriangle[j].indexUV[n];

					int nBone = setBmdSub[i].setVertex[nPos].uBones;
					Vec3D vPos = setBmdSub[i].setVertex[nPos].vPos;
					Vec3D vNormal = setBmdSub[i].setNormal[nNormal].vNormal;
					Vec2D vUV = setBmdSub[i].setUV[nUV];

					file<<nBone<<" "<<vPos.x<<" "<<vPos.y<<" "<<vPos.z<<" "<<vUV.x<<" "<<vUV.y<<" "<<vNormal.x<<" "<<vNormal.y<<" "<<vNormal.z<<std::endl;
				}
			}
		}
		file << "end"<< std::endl;
	}
	file.close();
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