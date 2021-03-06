///////////////////////////////////////////////////////////////////////////////////////
// Filename: IMessageListener.hpp
//
// Contains: IMessageListener interface. Classes that implement this interface
//           can access the messages of a BaseWindow object. Use the BaseWindow
//           member function BaseWindow::SetListener(...) to set an IMessageListener
//           object as the active message listener.
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////
// Preprocessor Directives
//---------------------------------
#pragma once

////////////////////////////////////
// Header Includes
//---------------------------------
#include <Windows.h>

////////////////////////////////////
// Interface Declaration
//----------------------------------
class IMessageListener
{
public:
	virtual VOID ListenToMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
};
