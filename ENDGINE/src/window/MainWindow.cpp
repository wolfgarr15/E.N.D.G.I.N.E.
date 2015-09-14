///////////////////////////////////
// Filename: MainWindow.cpp
///////////////////////////////////

////////////////////////////////////////////////////////////////
// Class Header
//-------------------------------------------------------------
#include "MainWindow.hpp"

///////////////////////////////////////////////////////////////////
// Class Definition
//-----------------------------------------------------------------
MainWindow::MainWindow(HINSTANCE hInstance, CONST PWNDCLASSEX pWcex)
	: BaseWindow(hInstance, pWcex)
{
	// DO NOTHING.
}

LRESULT MainWindow::HandleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		// If the window is closed, message the program to quit.
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
	}

	return (this->BaseWindow::HandleMessages(hWnd, uMsg, wParam, lParam));
}

