////////////////////////////////
// Filename: RareInput.hpp
//
// Author: Wolfe S. Greene
////////////////////////////////

////////////////////////////////
// Pre-processor Directives
//------------------------------
#pragma once

///////////////////////////////
// Include Headers
//-----------------------------
#include <memory>
#include "..\globals\LeanWindows.h"
#include "..\interfaces\IMessageListener.hpp"
#include "RareInputDevice.hpp"

///////////////////////////////////////////
// Global Statics
//-----------------------------------------
static const UINT NUMMOUSEBUTTONS = 5;
static const UINT NUMKEYBOARDKEYS = 256;

static const enum RAREMBUTTON
{
	LEFTBUTTON,
	RIGHTBUTTON,
	MIDDLEBUTTON,
	OTHERBUTTON1,
	OTHERBUTTON2,
};

////////////////////////////////////
// Class Declaration
//----------------------------------
class RareInput 
	: public IMessageListener // Implements IMessageListerner to access window messages.
{
	POINT m_mouseCurr;
	POINT m_mousePrev;

	LONG m_wheelCurr;
	LONG m_wheelPrev;

	bool m_mouseButtons[NUMMOUSEBUTTONS];
	bool m_keys[NUMKEYBOARDKEYS];

public:
	RareInput();
	~RareInput() = default;

	BOOL Initialize(HWND hWndTarget, UINT numDevices, CONST RAWINPUTDEVICE* deviceList);
	BOOL Initialize(HWND hWndTarget, CONST RareInputDevice& deviceList);

	VOID GetMousePosition(POINT& returnValue) CONST;
	VOID GetPrevMousePos(POINT& returnValue) CONST;

	LONG GetMouseX() CONST;
	LONG GetPrevMouseX() CONST;
	LONG GetMouseY() CONST;
	LONG GetPrevMouseY() CONST;

	LONG GetMouseWheelPosition() CONST;
	LONG GetLastMouseWheelPos() CONST;

	BOOL IsMouseButtonDown(UINT buttonId) CONST;
	BOOL IsKeyDown(USHORT keyCode) CONST;

	VOID ListenToMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	// RareInput instances are uncopyable and unassignable.
	RareInput(CONST RareInput& source) = delete;
	RareInput& operator=(CONST RareInput& other) = delete;

	VOID ProcessMouseInput(CONST PBYTE buffer);
	VOID ProcessKeyboardInput(CONST PBYTE buffer);
	VOID ProcessRawInputMessages(LPARAM lParam);
};



