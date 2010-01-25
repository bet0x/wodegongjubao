#include "MainRoot.h"

#include <windows.h>
#include <wininet.h>
#define MAXBLOCKSIZE 1024
#pragma comment(lib, "wininet.lib")
void download(const char *szUrl,const char *szDest)
{
	HINTERNET hSession = InternetOpenA("RookIE/1.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hSession != NULL)
	{
		HINTERNET handle2 = InternetOpenUrlA(hSession, szUrl, NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);
		if (handle2 != NULL)
		{
			byte Temp[MAXBLOCKSIZE];
			ULONG Number = 1;
			FILE *stream;
			if( (stream = fopen(szDest, "wb")) != NULL )
			{
				while (Number > 0)
				{
					InternetReadFile(handle2, Temp, MAXBLOCKSIZE - 1, &Number);
					fwrite(Temp, sizeof(char), Number, stream);
				}
				fclose(stream);
			}
			InternetCloseHandle(handle2);
			handle2 = NULL;
		}
		InternetCloseHandle(hSession);
		hSession = NULL;
	}
}

//--------------------------------------------------------------------------------------
// Initialize everything and go into a render loop
//--------------------------------------------------------------------------------------
INT WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
	// Create application object
	CMainRoot app;
	try
	{
	//	download("http://www.rpgsky.com/muworldeditor/update.inf","update.inf");
		app.Run();
	}
	catch( std::exception& e )
	{
		// #if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		// 		MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		// #else
		// 		std::cerr << "An exception has occured: " << 
		// 			e.getFullDescription().c_str() << std::endl;
		// #endif
	}

	return 0;
	// 通过DXUT来处理消息循环并分派渲染调用。当在空闲时间和处理窗口消息的
	// 时间间隔时，框架将调用OnFrameMove和OnFrameRender回调函数。
	//GetVideo()->Play(L"Data/yso_logo.avi");
	// 	if ( GetScript()->DoFile( GetConfig().GetScriptFilename() ) )
	// 	{
	// 		LOG("LoadScript Error!")
	// 	}
}


