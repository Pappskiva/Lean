////////////////////////////////////////////////////////////////////////////////
// Filename: main.cpp
////////////////////////////////////////////////////////////////////////////////
#include "SystemClass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemClass* System;
	bool result;


	// Skapa systemobjektet
	System = new SystemClass;
	if (!System)
	{
		return 0;
	}

	// Initialisera och k�r systemobjektet
	result = System->Initialize();
	if (result)
	{
		System->Run();
	}

	// St�ng ner och release systemobjektet
	System->Shutdown();
	delete System;
	System = 0;

	return 0;
}