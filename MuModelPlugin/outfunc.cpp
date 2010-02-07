#include "myplug.h"

BOOL WINAPI Model_Plug_CreateObject(void ** pobj){
	*pobj = new CMyPlug;
	return *pobj != NULL;
}