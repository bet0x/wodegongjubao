#include "FrameRectMgr.h"


CFrameRectMgr::CFrameRectMgr()
{

}
CFrameRectMgr::~CFrameRectMgr()
{

}

void CFrameRectMgr::AddFrameRect(std::string strActionName,sFrameColRect &FrameRect)
{
	m_mapFrameColRect[strActionName].push_back(FrameRect);
}

void CFrameRectMgr::Save(std::string FileName)
{
	FILE *file = fopen(FileName.c_str(),"wb");
	
	if(file != NULL)
	{
		int nMaxAction = m_mapFrameColRect.size();
		fwrite(&nMaxAction,sizeof(int),1,file);
		fseek(file,sizeof(int),SEEK_CUR);

		mapFrameColRect::iterator iter;
		for(iter=m_mapFrameColRect.begin(); iter!=m_mapFrameColRect.end(); ++iter)
		{
			char strName[32];
			memcpy(strName,iter->first.c_str(),sizeof(strName));

			fwrite(strName,sizeof(strName),1,file);
			fseek(file,sizeof(strName),SEEK_CUR);

			vecFrameRect &FrameRect = iter->second;
			int nFrameSize = FrameRect.size();
			fwrite(&nFrameSize,sizeof(int),1,file);
			fseek(file,sizeof(int),SEEK_CUR);

			for(int i=0; i<FrameRect.size(); i++)
			{
				int nFrameID = FrameRect[i].nFrameID;
				fwrite(&nFrameID,sizeof(int),1,file);
				fseek(file,sizeof(int),SEEK_CUR);

				int nRectCout = FrameRect[i].vecRect.size();
				fwrite(&nRectCout,sizeof(int),1,file);
				fseek(file,sizeof(int),SEEK_CUR);

				if(nRectCout > 0)
				{
					fwrite(&FrameRect[i].vecRect[0],sizeof(sFrameRect)*nRectCout,1,file);
					fseek(file,sizeof(sFrameRect)*nRectCout,SEEK_CUR);
				}

			}
			
		}
	
	
		fclose(file);
	}
	
}

void CFrameRectMgr::Load(std::string FileName)
{
	FILE *file = fopen(FileName.c_str(),"rb");

	m_mapFrameColRect.clear();

	if(file != NULL)
	{
		int nMaxAction = 0;

		fread(&nMaxAction,sizeof(int),1,file);
		fseek(file,sizeof(int),SEEK_CUR);

		for(int i=0; i<nMaxAction; i++)
		{
			char strActionName[32];
			fread(strActionName,sizeof(strActionName),1,file);
			fseek(file,sizeof(strActionName),SEEK_CUR);

			
			int nFrameSize = 0;
			fread(&nFrameSize,sizeof(int),1,file);
			fseek(file,sizeof(int),SEEK_CUR);
			sFrameColRect FrameColRect;

			for(int j=0; j<nFrameSize; j++)
			{
				int nFrameID = 0;
				fread(&nFrameID,sizeof(int),1,file);
				fseek(file,sizeof(int),SEEK_CUR);

				int nRectCount = 0;
				fread(&nRectCount,sizeof(int),1,file);
				fseek(file,sizeof(int),SEEK_CUR);

				
				

				if(nRectCount)
				{
					sFrameRect *pRect = new sFrameRect[nRectCount];
					fread(pRect,sizeof(sFrameRect)*nRectCount,1,file);
					fseek(file,sizeof(sFrameRect)*nRectCount,SEEK_CUR);

					for(int k=0; k<nRectCount; k++)
						FrameColRect.vecRect.push_back(pRect[k]);

					delete[] pRect;
				}

				FrameColRect.nFrameID = nFrameID;
				
			}

			m_mapFrameColRect[strActionName].push_back(FrameColRect);
		}
	}


}