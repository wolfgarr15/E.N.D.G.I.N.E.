///////////////////////////////////
// Filename: RareInputDevices.hpp
///////////////////////////////////

////////////////////////////////
// Pre-processor Directives
//------------------------------
#pragma once
#define WIN32_LEAN_AND_MEAN

///////////////////////////////
// Include Headers
//-----------------------------
#include <Windows.h>
#include <vector>

///////////////////////////////
// Static Enumerator
//-----------------------------
static const enum RIUSEDEFAULT
{
	KEYBOARD,
	MOUSE,
	DEVICES,
	NONE
};

////////////////////////////////////
// Class Declaration
//----------------------------------
class RareInputDevice
{
	std::vector<RAWINPUTDEVICE> m_rawInputDevices;

public:
	RareInputDevice(CONST RIUSEDEFAULT defaultDevices = NONE);
	~RareInputDevice() = default;

	VOID AddDevice(USHORT usUsagePage, USHORT usUsage, DWORD dwFlags, HWND hWndTarget);
	VOID AddDevice(CONST RAWINPUTDEVICE& rawInputdevice);

	VOID SetTarget(HWND hWnd);

	SIZE_T GetRawCount() CONST;
	CONST RAWINPUTDEVICE* GetRawPointer() CONST;
};
