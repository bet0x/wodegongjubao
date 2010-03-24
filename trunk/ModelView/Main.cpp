#include "MainRoot.h"

INT WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
	// Create application object
	try
	{
		CMainRoot::getInstance().Run();
	}
	catch( std::exception& e )
	{
		assert(0);
//		MessageBoxA( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		// #if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		// 		MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		// #else
		// 		std::cerr << "An exception has occured: " << 
		// 			e.getFullDescription().c_str() << std::endl;
		// #endif
	}
	return 0;
}