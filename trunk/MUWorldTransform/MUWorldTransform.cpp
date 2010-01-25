// MUWorldTransform.cpp : 定义控制台应用程序的入口点。
//

#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
#include <stdio.h>
#include <tchar.h>
#include "FileSystem.h"
#include "LumpFile.h"
#include "Color.h"

void fileOffset(const std::string& strSrcFilename, const std::string& strDestFilename,size_t offset)
{
	FILE* fp = fopen(strSrcFilename.c_str(), "rb");
	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		size_t size = ftell(fp);
		if (size>offset)
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

void ozj2jpg(const std::string& strSrcFilename)
{
	std::string strDestFilename = ChangeExtension(strSrcFilename,"jpg");
	fileOffset(strSrcFilename,strDestFilename,24);
}

void ozt2tga(const std::string& strSrcFilename)
{
	std::string strDestFilename = ChangeExtension(strSrcFilename,"tga");
	fileOffset(strSrcFilename,strDestFilename,4);
}
void ozb2bmp(const std::string& strSrcFilename)
{
	std::string strDestFilename = ChangeExtension(strSrcFilename,"bmp");
	fileOffset(strSrcFilename,strDestFilename,4);
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

void decryptMuBuffer2(char* buffer, size_t size)
{
	const char xorKeys[] = {0xFC, 0xCF, 0xAB};
	for (size_t i=0; i<size; ++i)
	{
		*buffer ^= xorKeys[i%3];
		buffer++;
	}
}

void decryptMuFile2(const std::string& strSrcFilename, const std::string& strDestFilename)
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
		decryptMuBuffer2(buffer,size);
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
		decryptMuBuffer2(buffer,size);
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
	{
		std::wstring wstrFindPath = getCurrentDirectory()+L"\\*.*";
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
					ozj2jpg(strFilename);
				}
				else if (wstrExt==L".ozt")
				{
					ozt2tga(strFilename);
				}
				else if (wstrExt==L".ozb")
				{
					ozb2bmp(strFilename);
				}
				else if (wstrExt==L".map"||wstrExt==L".obj")
				{
					decryptMuFile(strFilename,"dec"+strFilename+"d");
				}
				else if (wstrExt==L".att")
				{
					decryptMuATTFile(strFilename,"dec"+strFilename+"d");
				}
				else if (wstrExt==L".bmd")
				{
					if ("BuffEffect.bmd"==strFilename)
					{
						decryptBuffEffectFile(strFilename,"dec"+strFilename+"d");
					}
					else if (isEncBmd(strFilename))
					{
						decryptMuFile(strFilename,"dec"+strFilename+"d");
					}
					else
					{
						decryptMuFile2(strFilename,"dec"+strFilename+"d");
					}
				}
				//else if (wstrExt==L".raw")
			}
		}
	}
	return 1;
	{
		CLumpFile lump;
		lump.SetName("terrain");
		lump.SetInt(1);
		lump.SetInt("width", 253);
		lump.SetInt("height", 253);

		std::vector<float> setHeight;
		std::vector<uint8> setTile[2];
		std::vector<Color32> setColor;
		std::vector<uint8> setAttribute;

		CLumpNode* pCellsNode = lump.AddNode("cells");
		if (pCellsNode)
		{
			{
				FILE* fp = fopen("Terrain.map", "rb");
				if (fp)
				{
					fseek(fp, 0, SEEK_END);
					size_t size = ftell(fp);
					if (size==65536*3+2)
					{
						fseek(fp, 2, SEEK_SET);
						uint8 buffer[65536*3];
						fread(&buffer,65536*3,1,fp);

						for (int y=0; y<253; ++y)
						{
							for (int x=0; x<253; ++x)
							{
								setTile[0].push_back(buffer[y*256+x]);
								setTile[1].push_back(buffer[y*256+x+256*256]);
							}
						}

						for (int y=0; y<254; ++y)
						{
							for (int x=0; x<254; ++x)
							{
								setColor.push_back(Color32(buffer[y*256+x+256*256*2],255,255,255));
							}
						}
					}
					fclose(fp);
				}
			}
			{
				FILE* fp = fopen("TerrainLight.bmp", "rb");
				if (fp)
				{
					fseek(fp, 54, SEEK_SET);
					for (int y=0; y<254; ++y)
					{
						uint8 uVal;
						for (int x=0; x<254; ++x)
						{
							fread(&uVal,1,1,fp);
							setColor[y*254+x].b=uVal;
							fread(&uVal,1,1,fp);
							setColor[y*254+x].g=uVal;
							fread(&uVal,1,1,fp);
							setColor[y*254+x].r=uVal;
						}
						fread(&uVal,1,1,fp);
						fread(&uVal,1,1,fp);
						fread(&uVal,1,1,fp);
						fread(&uVal,1,1,fp);
						fread(&uVal,1,1,fp);
						fread(&uVal,1,1,fp);
					}
					fclose(fp);
				}
			}
			{
				FILE* fp = fopen("TerrainHeight.bmp", "rb");
				if (fp)
				{
					fseek(fp, 1078, SEEK_SET);
					for (int y=0; y<254; ++y)
					{
						uint8 uVal;
						fread(&uVal,1,1,fp);
						fread(&uVal,1,1,fp);
						for (int x=0; x<254; ++x)
						{
							fread(&uVal,1,1,fp);
							setHeight.push_back(uVal*0.015f);
						}
					}
					fclose(fp);
				}


				fp = fopen("Terrain.att", "rb");
				if (fp)
				{
					fseek(fp, 3, SEEK_SET);
					for (int y=0; y<253; ++y)
					{
						for (int x=0; x<256; ++x)
						{
							uint8 uVal;
							fread(&uVal,1,1,fp);
							if (x<253)
							{
								setAttribute.push_back(uVal);

								if (uVal==0x08||uVal==0x0C)
								{
									setTile[0][y*253+x]=255;
									setTile[1][y*253+x]=255;
								}
								/*switch(uVal)
								{
								case 0:
									setColor[y*254+x].r=255;
									setColor[y*254+x].g=255;
									setColor[y*254+x].b=255;
									break;
								case 1:
									setColor[y*254+x].r=0;
									setColor[y*254+x].g=255;
									setColor[y*254+x].b=0;
									break;
								case 4:
									setColor[y*254+x].r=255;
									setColor[y*254+x].g=0;
									setColor[y*254+x].b=0;
								    break;
								case 5:
									setColor[y*254+x].r=255;
									setColor[y*254+x].g=255;
									setColor[y*254+x].b=0;
								    break;
								case 0xFF:
									setColor[y*254+x].r=0;
									setColor[y*254+x].g=0;
									setColor[y*254+x].b=0;
									break;
								default:
									setColor[y*254+x].r=0;
									setColor[y*254+x].g=0;
									setColor[y*254+x].b=255;
								    break;
								}*/
							}
						}
					}
					fclose(fp);
				}
				pCellsNode->SetVector("height", setHeight);
				pCellsNode->SetVector("tile0",	setTile[0]);
				pCellsNode->SetVector("tile1",	setTile[1]);
				pCellsNode->SetVector("color",	setColor);
				pCellsNode->SetVector("attribute",	setAttribute);
			}
		}
		lump.SaveFile("test.map");
	}
	return 0;
}