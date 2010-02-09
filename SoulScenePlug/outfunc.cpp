#include "myplug.h"

BOOL WINAPI Scene_Plug_CreateObject(void ** pobj){
	*pobj = new CMyPlug;
	return *pobj != NULL;
}