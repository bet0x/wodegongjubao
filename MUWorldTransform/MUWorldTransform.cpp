// MUWorldTransform.cpp : 定义控制台应用程序的入口点。
//

#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
#include <stdio.h>
#include <tchar.h>
#include "FileSystem.h"
#include "LumpFile.h"
#include "Color.h"
#include "CSVFile.h"
#include "Direct.h"

void fileOffset(const std::string& strSrcFilename, const std::string& strDestFilename,int offset)
{
	FILE* fp = fopen(strSrcFilename.c_str(), "rb");
	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		size_t size = ftell(fp);
		if (offset<0)
		{
			size_t addSize=abs(offset);
			fseek(fp, 0, SEEK_SET);
			size+=addSize;
			char* buffer= new char[size];
			fread(buffer,addSize,1,fp);
			fseek(fp, 0, SEEK_SET);
			fread(&buffer[addSize],size-addSize,1,fp);
			{

				FILE* fpw = fopen(strDestFilename.c_str(), "wb");
				if (fpw)
				{
					fwrite(buffer,size,1,fpw);
					fclose(fpw);
				}
			}
			delete buffer;
		}
		else if (size>offset)
		{
			fseek(fp, offset, SEEK_SET);
			size-=offset;
			char* buffer= new char[size];
			fread(buffer,size,1,fp);
			{

				FILE* fpw = fopen(strDestFilename.c_str(), "wb");
				if (fpw)
				{
					fwrite(buffer,size,1,fpw);
					fclose(fpw);
				}
			}
			delete buffer;
		}
		fclose(fp);
	}
}

void decryptMuBuffer(char* buffer, size_t size)
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

void decryptBuffEffectFile(const std::string& strSrcFilename, const std::string& strDestFilename)
{
	FILE* fp = fopen(strSrcFilename.c_str(), "rb");
	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		size_t size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		char* buffer = new char[size];
		fread(buffer,size,1,fp);
		fclose(fp);
		fp = NULL;
		decryptMuBuffer(buffer+4,size-4);
		fp = fopen(strDestFilename.c_str(), "wb");
		if (fp)
		{
			fwrite(buffer,size,1,fp);
			fclose(fp);
		}
		delete buffer;
	}
}

void decryptMuFile(const std::string& strSrcFilename, const std::string& strDestFilename)
{
	FILE* fp = fopen(strSrcFilename.c_str(), "rb");
	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		size_t size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		char* buffer = new char[size];
		fread(buffer,size,1,fp);
		fclose(fp);
		fp = NULL;
		decryptMuBuffer(buffer,size);
		fp = fopen(strDestFilename.c_str(), "wb");
		if (fp)
		{
			fwrite(buffer,size,1,fp);
			fclose(fp);
		}
		delete buffer;
	}
}

void decryptMuBufferXOR3(char* buffer, size_t size)
{
	const char xorKeys[] = {0xFC, 0xCF, 0xAB};
	for (size_t i=0; i<size; ++i)
	{
		*buffer ^= xorKeys[i%3];
		buffer++;
	}
}

struct itemdata97
{
	char cName[30];
	unsigned char ucInt[34];
};

struct itemdatastruct97 //96y,97,98c
{
	char cName[30];	//1物品名称
	unsigned char ucHand;		//2 0单手，1双手
	unsigned char ucDroplv;	//3 所掉物品的怪物等级Level
	unsigned char ucX;		//4 X,Y 所占格子
	unsigned char ucY;        //5
	unsigned char ucDamMin;	//6 最小攻击力
	unsigned char ucDamMax;	//7 最大攻击力
	unsigned char ucDefxx;	//8 防御率
	unsigned char ucDef;		//9 防御力Def
	unsigned char ucMagDef;	//10 00 MagDef 魔法防御?
	unsigned char ucSpeed;	//11 攻击速度/护手等属性
	unsigned char ucWalkSeed;	//12 WalkSpeed 鞋子属性
	unsigned char ucDur;		//13 持久度
	unsigned char ucUnknown1;	//14 00
	unsigned short usiReqStr; //15 力量,此力量非所佩带需要力量,是有个力量参数*10 得来 /除了12项物品,该项是直接按值计算
	unsigned short usiReqDex; //16 敏捷,此力量非所佩带需要敏捷,是有个敏捷参数*10 得来 /除了12项物品
	//unsigned char ucUnknown2;  //17
	//unsigned char ucUnknown3;      //18
	unsigned char ucReqLev;	//19 最小佩带物品等级
	unsigned char ucValue;		//20 14项物品计算价格参数
	unsigned char ucUnknown4;	//21 ?书 12项非零
	unsigned char ucUnknown5;        //22
	unsigned char ucUnknown6;	//23 ? 书 极光以上非零
	unsigned char ucUnknown7;	//24 00
	unsigned char ucType1; //25 ?盾04 ..风头04//装备类都为04 普通武器刀剑类1 天雷3,玛雅武器0,弓弩2 法师杖03
	unsigned char ucDs;		//26 法师
	unsigned char ucDk;		//27 战士
	unsigned char ucElf;		//28 精灵
	unsigned char ucMg;		//29 魔剑士
	unsigned char ucBing;		//30 防冰属性
	unsigned char ucDu;		//31 防毒属性
	unsigned char ucLei;		//32 防雷属性
	unsigned char ucHuo;		//33 防火属性
}; //64byte
#include<iostream>
#include<iomanip>
#include<fstream>

void encryptItemBMD(const std::string& strSrcFilename, const std::string& strDestFilename)
{
	CCsvFile csvFile;
	if (csvFile.Open(strSrcFilename))
	{
		char buffer[512*64];
		memset(buffer,0,512*64);
		while (csvFile.SeekNextLine())
		{
			int id = csvFile.GetInt(0);
			if (id>=512)
			{
				continue;
			}
			itemdata97* pItem=(itemdata97*)(buffer+id*64);
			std::string strName = csvFile.GetStr(1);
			strcpy(pItem->cName,strName.c_str());
			for (size_t n=0;n<34;++n)
			{
				pItem->ucInt[n]=csvFile.GetInt(n+2);
			}
		}
		csvFile.Close();
		//
		for (size_t i=0;i<512;++i)
		{
			decryptMuBufferXOR3(buffer+i*64,64);
		}
		DWORD dwCheck;
		//
		FILE* fp = fopen(strDestFilename.c_str(), "wb");
		if (fp)
		{
			fwrite(buffer,512*64,1,fp);
			fwrite(&dwCheck,4,1,fp);
			fclose(fp);
		}
	}
}

void decryptItemBMD(const std::string& strSrcFilename, const std::string& strDestFilename)
{
	FILE* fp = fopen(strSrcFilename.c_str(), "rb");
	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		size_t size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		char* buffer = new char[size];
		fread(buffer,size,1,fp);
		fclose(fp);
		fp = NULL;
		for (size_t i=0;i<size/64;++i)
		{
			decryptMuBufferXOR3(buffer+i*64,64);
		}

		std::ofstream file;
		file.open(strDestFilename.c_str(), std::ios::out);
		if ( file.is_open() )
		{
			file << "ID,物品名称,双手武器,掉物品的怪物等级,所占格子X,所占格子Y,最小攻击力,最大攻击力,防御率,防御力,魔法防御,攻击速度,鞋子属性,持久度,Unknown,力量需求,敏捷需求,等级需求,价格参数,书,22,书极光以上非零,00,类型,法师,战士,精灵,魔剑士,防冰属性,防毒属性,防雷属性,防火属性,9,9,9,9"<< std::endl;
			for (size_t i=0;i<size/64;++i)
			{
				itemdata97* pItem = (itemdata97*)(buffer+i*64);
				if (strlen(pItem->cName)!=0)
				{
					file <<i<<","<<pItem->cName;
					for (size_t n=0;n<34;++n)
					{
						file<<","<<(int)pItem->ucInt[n];
					}
					file<<std::endl;
				}
			}
		}
		file.close();

		delete buffer;
	}
}

void decryptMuFileXOR3(const std::string& strSrcFilename, const std::string& strDestFilename)
{
	FILE* fp = fopen(strSrcFilename.c_str(), "rb");
	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		size_t size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		char* buffer = new char[size];
		fread(buffer,size,1,fp);
		fclose(fp);
		fp = NULL;
		decryptMuBufferXOR3(buffer,size);
		fp = fopen(strDestFilename.c_str(), "wb");
		if (fp)
		{
			fwrite(buffer,size,1,fp);
			fclose(fp);
		}
		delete buffer;
	}
}

void decryptMuATTFile(const std::string& strSrcFilename, const std::string& strDestFilename)
{
	FILE* fp = fopen(strSrcFilename.c_str(), "rb");
	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		size_t size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		char* buffer = new char[size];
		fread(buffer,size,1,fp);
		fclose(fp);
		fp = NULL;
		decryptMuBuffer(buffer,size);
		decryptMuBufferXOR3(buffer,size);
		fp = fopen(strDestFilename.c_str(), "wb");
		if (fp)
		{
			fwrite(buffer,size,1,fp);
			fclose(fp);
		}
		delete buffer;
	}
}

bool isEncBmd(const std::string& strSrcFilename)
{
	//FILE* fp = fopen(strSrcFilename.c_str(), "rb");
	//if (fp)
	//{
	//}
	return false;
}

int main(int argc, _TCHAR* argv[])
{
	mkdir("Dec");mkdir("Enc");
	std::wstring wstrFindPath = getCurrentDirectory();
	CDir dir;
	dir.ReadDir(wstrFindPath);
	for (size_t i=0; i<dir.m_FileInfo.size(); i++)
	{
		if (!dir.m_FileInfo[i].IsDirectory())
		{
			std::wstring wstrExt = GetExtension(dir.m_FileInfo[i].wstrFilename);
			std::string strFilename = ws2s(dir.m_FileInfo[i].wstrFilename);
			if (wstrExt==L".ozj")
			{
				std::string strDestFilename = "Dec\\"+ChangeExtension(strFilename,".jpg");
				fileOffset(strFilename,strDestFilename,24);
			}
			else if (wstrExt==L".ozt")
			{
				std::string strDestFilename = "Dec\\"+ChangeExtension(strFilename,".tga");
				fileOffset(strFilename,strDestFilename,4);
			}
			else if (wstrExt==L".ozb")
			{
				std::string strDestFilename = "Dec\\"+ChangeExtension(strFilename,".bmp");
				fileOffset(strFilename,strDestFilename,4);
			}
			else if (wstrExt==L".jpg")
			{
				std::string strDestFilename = "Enc\\"+ChangeExtension(strFilename,".ozj");
				fileOffset(strFilename,strDestFilename,-24);
			}
			else if (wstrExt==L".tga")
			{
				std::string strDestFilename = "Enc\\"+ChangeExtension(strFilename,".ozt");
				fileOffset(strFilename,strDestFilename,-4);
			}
			else if (wstrExt==L".bmp")
			{
				std::string strDestFilename = "Enc\\"+ChangeExtension(strFilename,".ozb");
				fileOffset(strFilename,strDestFilename,-4);
			}
			else if (wstrExt==L".map"||wstrExt==L".obj")
			{
				decryptMuFile(strFilename,"Dec\\"+strFilename+"d");
			}
			else if (wstrExt==L".att")
			{
				decryptMuATTFile(strFilename,"Dec\\"+strFilename+"d");
			}
			else if (wstrExt==L".bmd")
			{
				if ("item.bmd"==strFilename)
				{
					decryptItemBMD(strFilename,"Dec\\"+ChangeExtension(strFilename,".csv"));
				}
				else if ("BuffEffect.bmd"==strFilename)
				{
					decryptBuffEffectFile(strFilename,"Dec\\"+strFilename+"d");
				}
				else if (isEncBmd(strFilename))
				{
					decryptMuFile(strFilename,"Dec\\"+strFilename+"d");
				}
				else
				{
					decryptMuFileXOR3(strFilename,"Dec\\"+strFilename+"d");
				}
			}
			else if (wstrExt==L".csv")
			{
				if ("item.csv"==strFilename)
				{
					encryptItemBMD(strFilename,"Enc\\"+ChangeExtension(strFilename,".bmd"));
				}
			}
		}
	}

	return 0;
}