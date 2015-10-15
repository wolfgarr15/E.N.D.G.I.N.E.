//////////////////////////////////////
// Filename: winmain.cpp
//------------------------------------

/////////////////////////////////////
// Preprocessor Directives
//-----------------------------------
#pragma once

/////////////////////////////////////
// Includes
//-----------------------------------
#include <fstream>
#include "globals\LeanWindows.h"
#include "config\EngineConfig.hpp"
#include "window\MainWindow.hpp"
#include "rareInput\RareInput.hpp"
#include "renderer\D3D11Renderer.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////
// Main Application Entry (NOTE: This is a test program!)
//---------------------------------------------------------------------------------------------
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	// Initialize the configuration object.
	EngineConfig config;
	config.Initialize("EngineDefault.ini", "Engine.ini");

	// Create the display window.
	MainWindow window(hInstance);
	window.Register();
	window.Create(&config);
	window.SetWindowCursorPos(0,0);

	// Create the input handler and set it to listen to the window.
	RareInput inputHandler;
	inputHandler.Initialize(window.GetHandle(), RAREDEVICE::KBMOUSE);
	window.SetListener(&inputHandler);

	// Create and initialize the DX11 renderer.
	D3D11Renderer DX11;
	DX11.Initialize(window.GetHandle(), &config);

	// Eveything else below is rando test code.
	//---------------------------------------------------------

	MSG msg;
	SecureZeroMemory(&msg, sizeof(MSG));
	bool done = false;

	std::ofstream fout("TEST.txt");

	while (!done)
	{
		DX11.BeginScene(1.0f, 0.0f, 0.0f, 0.0f);

		window.GetMessage(&msg);
		if (msg.message == WM_QUIT || inputHandler.IsKeyDown(VK_ESCAPE))
			done = true;

		DX11.EndScene();
	}

	std::string videoCardName;
	DX11.GetVideoCardName(videoCardName);
	fout << "Video Card Name : " << videoCardName << std::endl
		 << "Video Memory (MB) : " << DX11.GetVideoMemoryMB();

	fout.close();

	return 0;
}