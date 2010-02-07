#include "MUBmd.h"

void decryptMuBuffer(unsigned char* buffer, size_t size)
{
	static const uint8 xorKeys[16] = {
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

bool CMUBmd::LoadFile(const std::string& strFilename)
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

	for (size_t i=0;i<bmdSkeleton.setBmdBone.size();++i)
	{
		bmdSkeleton.calcLocalMatrix(i);
	}

	nFrameCount = 0;
	for (size_t i=0; i<head.uAnimCount; ++i)
	{
		nFrameCount+=bmdSkeleton.setBmdAnim[i].uFrameCount;
	}
	return true;
}