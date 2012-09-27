//Are we on the windows platform?
//#ifdef WIN32// anything included between ifdef and endif will be included if win32 is 
//if so include the header for windows applications
#include <Windows.h>
#include "GameApplication.h"


//Check to see if we are on windows
#ifdef WIN32
int WINAPI WinMain(HINSTANCE hInstance, 
					HINSTANCE hPrevInstance, 
					LPSTR lpCmdLine,
					int nCmdShow)
#else
//we are on some other platform
int main(int argc, char **argv)
#endif
{
	CGameApplication*pApp=new CGameApplication();

	if(!pApp->init())
	{
		if(pApp)
		{
			delete pApp;
			pApp=NULL;
			return 1;
		}
	}
	pApp->run();
	if(pApp)
	{
		delete pApp;
		pApp=NULL;
	}

	return 0;
}






