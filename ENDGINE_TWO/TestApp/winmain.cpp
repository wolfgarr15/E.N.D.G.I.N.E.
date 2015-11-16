#pragma once

#include <Windows.h>

#include "ECore.h"

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	ECore coreTest;

	coreTest.Initialize();
	coreTest.Run();
	coreTest.Shutdown();

	return 0;
}