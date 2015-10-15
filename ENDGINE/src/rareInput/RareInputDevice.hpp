///////////////////////////////////
// Filename: RareInputDevices.hpp
///////////////////////////////////

////////////////////////////////
// Pre-processor Directives
//------------------------------
#pragma once

///////////////////////////////
// Include Headers
//-----------------------------
#include <vector>
#include "..\globals\LeanWindows.h"

///////////////////////////////
// Static Enumerator
//-----------------------------
static const enum RAREDEVICE
{
	KEYBOARDONLY,
	MOUSEONLY,
	KBMOUSE,
	NONE
};

////////////////////////////////////
// Class Declaration
//----------------------------------
class RareInputDevice
{
	std::vector<RAWINPUTDEVICE> m_rawInputDevices;

public:
	RareInputDevice(RAREDEVICE defaultDevices = NONE);
	~RareInputDevice() = default;

	VOID AddDevice(USHORT usUsagePage, USHORT usUsage, DWORD dwFlags, HWND hWndTarget);
	VOID AddDevice(CONST RAWINPUTDEVICE& rawInputdevice);

	VOID SetTarget(HWND hWnd);

	SIZE_T GetRawCount() CONST;
	CONST RAWINPUTDEVICE* GetRawPointer() CONST;
};
