///////////////////////////////////
// Filename: RareInputDevice.cpp
///////////////////////////////////

////////////////////////////////////////////////////////////////
// Class Header
//-------------------------------------------------------------
#include "RareInputDevice.hpp"

///////////////////////////////////////////////////////////////////
// Class Definition
//-----------------------------------------------------------------
RareInputDevice::RareInputDevice(RAREDEVICE defaultDevices)
{
	RAWINPUTDEVICE defaultKeyboard = { 1, 6, 0, NULL };
	RAWINPUTDEVICE defaultMouse = { 1, 2, 0, NULL };

	switch (defaultDevices)
	{
		case NONE:
			break;

		case KEYBOARDONLY:
		{
			AddDevice(defaultKeyboard);
			break;
		}

		case MOUSEONLY:
		{
			AddDevice(defaultMouse);
			break;
		}

		case KBMOUSE:
		{
			AddDevice(defaultKeyboard);
			AddDevice(defaultMouse);
			break;
		}
	}
}

VOID RareInputDevice::AddDevice(USHORT usUsagePage, USHORT usUsage, DWORD dwFlags, HWND hWndTarget)
{
	RAWINPUTDEVICE rawDevice = { usUsagePage, usUsage, dwFlags, hWndTarget };
	AddDevice(rawDevice);
}

VOID RareInputDevice::AddDevice(CONST RAWINPUTDEVICE& rawInputdevice)
{
	m_rawInputDevices.push_back(rawInputdevice);
}

VOID RareInputDevice::SetTarget(HWND hWnd)
{
	for (SIZE_T i = 0; i < m_rawInputDevices.size(); i++)
		m_rawInputDevices[i].hwndTarget = hWnd;
}

SIZE_T RareInputDevice::GetRawCount() CONST
{
	return m_rawInputDevices.size();
}

CONST RAWINPUTDEVICE* RareInputDevice::GetRawPointer() CONST
{
	return m_rawInputDevices.data();
}