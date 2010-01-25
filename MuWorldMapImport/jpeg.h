#pragma once
//#include "common.h"
#include <stdio.h>
#include <memory.h>
#include "math.h"

class CJpeg
{
public:
	CJpeg();
	~CJpeg();
	bool loadJpegFromBuffer(const char* buffer, size_t size);
	bool LoadJpegFile(const char* szFilename);
	char* getBuffer();
	bool saveToBmp(const char* szFilename);
	bool saveToTxt(const char* szFilename);
	int  InitTag();
	void InitTable();
	int  Decode();
	int  DecodeMCUBlock();
	int  HufBlock(unsigned char dchufindex,unsigned char achufindex);
	int  DecodeElement();
	void IQtIZzMCUComponent(short flag);
	void IQtIZzBlock(short  *s ,int * d,short flag);
	void GetYUV(short flag);
	void StoreBuffer();
	unsigned char ReadByte();
	void Initialize_Fast_IDCT();
	void Fast_IDCT(int * block);
	void idctrow(int * blk);
	void idctcol(int * blk);
protected:
private:
	//HPALETTE           hPalette=NULL;
	//HBITMAP            hBitmap=NULL;
	unsigned long              LineBytes;
	unsigned long              ImgWidth , ImgHeight;
	char*             lpPtr;
	//////////////////////////////////////////////////
	//variables used in jpeg function
	short   SampRate_Y_H,SampRate_Y_V;
	short   SampRate_U_H,SampRate_U_V;
	short   SampRate_V_H,SampRate_V_V;
	short   H_YtoU,V_YtoU,H_YtoV,V_YtoV;
	short   Y_in_MCU,U_in_MCU,V_in_MCU;
	const unsigned char* lpJpegBuf;
	const unsigned char   *lp;
	short   qt_table[3][64];
	short   comp_num;
	unsigned char   comp_index[3];
	unsigned char      YDcIndex,YAcIndex,UVDcIndex,UVAcIndex;
	unsigned char   HufTabIndex;
	short      *YQtTable,*UQtTable,*VQtTable;
	short      code_pos_table[4][16],code_len_table[4][16];
	unsigned short code_value_table[4][256];
	unsigned short huf_max_value[4][16],huf_min_value[4][16];
	short   BitPos,CurByte;
	short   rrun,vvalue;
	short   MCUBuffer[10*64];
	int    QtZzMCUBuffer[10*64];
	short   BlockBuffer[64];
	short   ycoef,ucoef,vcoef;
	bool   IntervalFlag;
	short   interval;
	int    Y[4*64],U[4*64],V[4*64];
	unsigned long      sizei,sizej;
	short    restart;
	/*static*/  long iclip[1024];
	/*static*/  long *iclp;
};
