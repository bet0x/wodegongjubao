#pragma once
#include "IORead.h"
#include "Vec2D.h"
#include "Vec3D.h"
#include "Vec4D.h"
#include "Matrix.h"
#include "MemoryStream.h"

class CMUBmd
{
public:
	CMUBmd()
	{
		nFrameCount=0;
	}
	struct BmdHead
	{
		BmdHead()
		{
			memset(this,0,sizeof(*this));
		}
		char strFile[32];
		uint16 uSubCount;
		uint16 uBoneCount;
		uint16 uAnimCount;
	};

	struct BmdSkeleton
	{
		struct BmdAnim
		{
			BmdAnim()
			{
				uFrameCount=0;
				bOffset=false;
			}
			uint16 uFrameCount;
			bool bOffset;
			std::vector<Vec3D> vOffset;
			void load(CMemoryStream& s);
		};
		struct BmdBone
		{
			BmdBone()
			{
				bEmpty=0;
				szName[0]=0;
				nParent=-1;
			}
			bool bEmpty;
			char szName[32];
			int16 nParent;
			std::vector<Vec3D> setTrans;
			std::vector<Vec3D> setRotate;
			Matrix	mLocal;

			void load(CMemoryStream& s, const std::vector<BmdAnim>& setBmdAnim);
		};
		Matrix	getLocalMatrix(uint8 uBoneID);
		Matrix	getRotateMatrix(uint8 uBoneID);
		void	calcLocalMatrix(uint32 uBoneID);
		void	getLocalMatrix(std::vector<Matrix>& setLocalMatrix);

		std::vector<BmdAnim> setBmdAnim;
		std::vector<BmdBone> setBmdBone;

		void load(CMemoryStream& s, uint16 uBoneCount, uint16 uAnimCount);
	};

	struct BmdSub
	{
		struct BmdSubHead
		{
			uint16 uVertexCount;
			uint16 uNormal;
			uint16 uUVCount;
			uint16 uTriangleCount;
			uint16 uID;//?
		};
		struct BmdPos
		{
			uint32 uBones;
			Vec3D vPos;
		};
		struct BmdNormal
		{
			uint32 uBones;//?
			Vec3D vNormal;
			uint32 uUnknown2;
		};
		struct BmdTriangle
		{
			uint16 uUnknown1;//03
			uint16 indexVertex[3];
			uint16 uUnknown2;//CD
			uint16 indexNormal[3];
			uint16 uUnknown3;//CD
			uint16 indexUV[3];
			uint16 uUnknown[20];//CDCDCDCD
		};

		BmdSubHead head;
		std::vector<BmdPos> setVertex;
		std::vector<BmdNormal> setNormal;
		std::vector<Vec2D> setUV;
		std::vector<BmdTriangle> setTriangle;

		char szTexture[32];// Œ∆¿Ì
		void load(CMemoryStream& s);
		void skinMesh(std::vector<Matrix>& setBoneMatrix);
	};

	bool loadFormBmd(const std::string& strFilename);
	bool loadFormSmd(const std::string& strFilename);

	void saveToBmd(const std::string& strFilename);
	void saveToSmd(const std::string& strFilename);

	BmdHead head;
	std::vector<BmdSub> setBmdSub;
	BmdSkeleton bmdSkeleton;
	int nFrameCount;
};

void Bmd2Smd(const std::string& strSrcFilename, const std::string& strDestFilename);
void Smd2Bmd(const std::string& strSrcFilename, const std::string& strDestFilename);