///////////////////////////////////
// Filename: MainWindow.hpp
///////////////////////////////////

////////////////////////////////
// Pre-processor Directives
//------------------------------
#pragma once

///////////////////////////////
// Include Headers
//-----------------------------
#include "BaseWindow.hpp"

////////////////////////////////////
// Class Declaration
//----------------------------------
class MainWindow :
	public BaseWindow
{
public:
	MainWindow(HINSTANCE hInstance, CONST PWNDCLASSEX pWcex = NULL);
	~MainWindow() = default;

protected:
	LRESULT HandleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

