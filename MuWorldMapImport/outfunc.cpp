#include "myplug.h"

BOOL WINAPI Plug_CreateObject(void ** pobj){
	*pobj = new CMyPlug;
	return *pobj != NULL;
}