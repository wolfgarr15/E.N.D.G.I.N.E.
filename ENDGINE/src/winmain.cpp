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
#include <Windows.h>
#include "config\EngineConfig.hpp"
#include "window\MainWindow.hpp"
#include "rareInput\RareInput.hpp"
#include "renderer\D3D11Renderer.hpp"
#include "shaders\TextureShader.hpp"
#include "camera\Camera.hpp"
#include "model\model.hpp"

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
	if (!DX11.Initialize(window.GetHandle(), &config))
		return 1;

	// Create and initialize the TextureShader.
	TextureShader textureShader(DX11.GetDevice(), DX11.GetDeviceContext());
	if(!textureShader.Initialize())
		return 2;

	// Create and initialize the camera.
	Camera camera(DirectX::XMFLOAT3(0.0f, 0.0f, -10.0f));

	// Create and initialize the model.
	Model model;
	if (!model.Initialize(DX11.GetDevice(), DX11.GetDeviceContext()))
		return 3;
	if(!model.Load("data/meshes/cube.obj", L"data/textures/stone01.tga"))
		return 4;

	// Eveything else below is rando test code.
	//---------------------------------------------------------

	MSG msg;
	SecureZeroMemory(&msg, sizeof(MSG));
	bool done = false;

	std::ofstream fout("TEST.txt");

	while (!done)
	{
		DirectX::XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

		DX11.BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
		camera.RenderView();
		model.Render();
		if (!textureShader.Render(model.GetVertexCount(),
								  DX11.GetWorldMatrix(),
								  DX11.GetViewMatrix(),
							      DX11.GetProjectionMatrix(),
							      model.GetTextureView()))
		{
			return 5;
		}
		DX11.EndScene();

		window.GetMessage(&msg);
		if (msg.message == WM_QUIT || inputHandler.IsKeyDown(VK_ESCAPE))
			done = true;
	}

	std::string videoCardName;
	DX11.GetVideoCardName(videoCardName);
	fout << "Video Card Name : " << videoCardName << std::endl
		 << "Video Memory (MB) : " << DX11.GetVideoMemoryMB();

	fout.close();

	return 0;
}