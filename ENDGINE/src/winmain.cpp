//////////////////////////////////////
// Filename: winmain.cpp
//------------------------------------

/////////////////////////////////////
// Preprocessor Directives
//-----------------------------------
#pragma once
#define WIN32_LEAN_AND_MEAN

/////////////////////////////////////
// Includes
//-----------------------------------
#include <fstream>
#include <Windows.h>
#include "config/iniconfig.hpp"
#include "window/MainWindow.hpp"
#include "rareInput/RareInput.hpp"
#include "renderer/D3D11Renderer.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////
// Main Application Entry
//---------------------------------------------------------------------------------------------
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	INIconfig config;
	config.Initialize("EngineDefault.ini", "Engine.ini");

	MainWindow window(hInstance);
	window.Register();
	window.Create(&config);
	window.SetWindowCursorPos(0,0);

	RareInput inputHandler;
	inputHandler.Initialize(window.GetHandle(), RIUSEDEFAULT::DEVICES);
	window.SetListener(&inputHandler);

	D3D11Renderer DX11;
	if(!DX11.Initialize(config.GetValue<INT>("Window.iWidth"),  //Window width
					config.GetValue<INT>("Window.iHeight"),     //Window height
					TRUE,
					window.GetHandle(),
					config.GetValue<BOOL>("Window.bFullscreen"),
					1000.0f,
					0.1f))
					return 1;


	MSG msg;
	SecureZeroMemory(&msg, sizeof(MSG));
	bool done = false;

	std::ofstream fout("TEST.txt");

	while (!done)
	{
		window.GetMessage(&msg);
		if (msg.message == WM_QUIT || inputHandler.IsKeyDown(VK_ESCAPE))
			done = true;

		fout << "Mouse X: " << inputHandler.GetMouseX() << std::endl
			 << "Mouse Y: " << inputHandler.GetMouseY() << std::endl;

		DX11.BeginScene(1.0f, 0.0f, 0.0f, 1.0f);
		DX11.EndScene();
	}

	std::string videoCardName;
	DX11.GetVideoCardName(videoCardName);

	fout << videoCardName << std::endl;

	fout.close();

	return 0;
}