#pragma once

#include <Vec3D.h>
#include <vector>
#include <map>
#include <string>

enum ColRectType
{
	COLRECT_DEF=0,
	COLRECT_ATTACK,
	COLRECT_COL,
};

struct sFrameRect
{
	int nType;
	int nHitID;
	Vec3D vPos;
	Vec3D vSize;
};

struct sFrameColRect
{
	int nFrameID;
	std::vector<sFrameRect> vecRect;
};
typedef std::vector<sFrameColRect> vecFrameRect;
typedef std::map< std::string,vecFrameRect > mapFrameColRect;

class CFrameRectMgr
{
public:
	CFrameRectMgr();
	~CFrameRectMgr();

	void AddFrameRect(std::string strActionName,sFrameColRect &FrameRect);
	void Save(std::string FileName);
	void Load(std::string FileName);
private:
	mapFrameColRect m_mapFrameColRect;
};