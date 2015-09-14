////////////////////////////////
// Filename: RareInput.cpp
////////////////////////////////

////////////////////////////////////////////////////////////////
// Class Header
//-------------------------------------------------------------
#include "RareInput.hpp"

////////////////////////////////////////////////////////////////
// Class Definition
//--------------------------------------------------------------
RareInput::RareInput()
{
	// Initialize keyboard and mouse button states.
	for (int i = 0; i < NUMKEYBOARDKEYS; i++)
	{
		m_keys[i] = false;

		if (i < NUMMOUSEBUTTONS)
			m_mouseButtons[i] = false;
	}

	// Initialize mouse wheel states.
	m_wheelPrev = 0;
	m_wheelCurr = m_wheelPrev;

	// Initilialize mouse position states.
	m_mousePrev = { 0, 0 };
	m_mouseCurr = m_mousePrev;
}

BOOL RareInput::Initialize(HWND hWndTarget, UINT numDevices, CONST RAWINPUTDEVICE* deviceList)
{
	// Set initial mouse position with respect to the target window.
	GetCursorPos(&m_mousePrev);
	ScreenToClient(hWndTarget, &m_mousePrev);
	m_mouseCurr = m_mousePrev;

	// Register devices. Returns TRUE on success, FALSE on failure.
	return RegisterRawInputDevices(deviceList, numDevices, sizeof(RAWINPUTDEVICE));
}

BOOL RareInput::Initialize(HWND hWndTarget, CONST RareInputDevice& deviceList)
{
	return Initialize(hWndTarget, deviceList.GetRawCount(), deviceList.GetRawPointer());
}

VOID RareInput::GetMousePosition(POINT& returnValue) CONST
{
	returnValue = m_mouseCurr;
}

VOID RareInput::GetPrevMousePos(POINT& returnValue) CONST
{
	returnValue = m_mousePrev;
}

LONG RareInput::GetMouseX() CONST
{
	return m_mouseCurr.x;
}

LONG RareInput::GetPrevMouseX() CONST
{
	return m_mousePrev.x;
}
LONG RareInput::GetMouseY() CONST
{
	return m_mouseCurr.y;
}

LONG RareInput::GetPrevMouseY() CONST
{
	return m_mousePrev.y;
}

LONG RareInput::GetMouseWheelPosition() CONST
{
	return m_wheelCurr;
}

LONG RareInput::GetLastMouseWheelPos() CONST
{
	return m_wheelPrev;
}

BOOL RareInput::IsMouseButtonDown(UINT buttonId) CONST
{
	// Return 2, to denote an error if the input buttonId is out of bounds.
	if (buttonId >= NUMMOUSEBUTTONS)
		return 2;

	return m_mouseButtons[buttonId];
}

BOOL RareInput::IsKeyDown(USHORT keyCode) CONST
{
	// Return 2, to denote an error, if the keyCode is out of bounds.
	if (keyCode >= NUMKEYBOARDKEYS)
		return 2;

	return m_keys[keyCode];
}

VOID RareInput::ListenToMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_INPUT)
		ProcessRawInputMessages(lParam);
}

VOID RareInput::ProcessMouseInput(CONST PBYTE buffer)
{
	// Sanity check. Make sure the buffer exists.
	if (!buffer)
		return;

	// Get a reference to the RAWINPUT mouse from the buffer.
	RAWMOUSE& mouse = ((LPRAWINPUT)buffer)->data.mouse;

	// Update the mouse position.
	m_mousePrev = m_mouseCurr;
	m_mouseCurr.x += mouse.lLastX;
	m_mouseCurr.y += mouse.lLastY;

	// Update the mouse wheel state.
	if (mouse.usButtonFlags & RI_MOUSE_WHEEL)
	{
		m_wheelPrev = m_wheelCurr;
		m_wheelCurr += mouse.usButtonData;
	}

	// Update left mouse button state.
	if (mouse.ulButtons & RI_MOUSE_LEFT_BUTTON_DOWN)
		m_mouseButtons[RIMOUSE::LEFTBUTTON] = true;

	if (mouse.ulButtons & RI_MOUSE_LEFT_BUTTON_UP)
		m_mouseButtons[RIMOUSE::LEFTBUTTON] = false;

	// Update the right mouse button state.
	if (mouse.ulButtons & RI_MOUSE_RIGHT_BUTTON_DOWN)
		m_mouseButtons[RIMOUSE::RIGHTBUTTON] = true;

	if (mouse.ulButtons & RI_MOUSE_RIGHT_BUTTON_UP)
		m_mouseButtons[RIMOUSE::RIGHTBUTTON] = false;

	// Update the middle mouse button.
	if (mouse.ulButtons & RI_MOUSE_MIDDLE_BUTTON_DOWN)
		m_mouseButtons[RIMOUSE::MIDDLEBUTTON] = true;

	if (mouse.ulButtons & RI_MOUSE_MIDDLE_BUTTON_UP)
		m_mouseButtons[RIMOUSE::MIDDLEBUTTON] = false;

	// Note: RI_MOUSE_BUTTON1, 2, and 3 are the same as the 
	//       left, right, and middle buttons, respectively.

	// Update other mouse button 1.
	if (mouse.ulButtons & RI_MOUSE_BUTTON_4_DOWN)
		m_mouseButtons[RIMOUSE::OTHERBUTTON1] = true;

	if (mouse.ulButtons & RI_MOUSE_BUTTON_4_UP)
		m_mouseButtons[RIMOUSE::OTHERBUTTON1] = false;

	// Update other mouse button 2.
	if (mouse.ulButtons & RI_MOUSE_BUTTON_5_DOWN)
		m_mouseButtons[RIMOUSE::OTHERBUTTON2] = true;

	if (mouse.ulButtons & RI_MOUSE_BUTTON_5_UP)
		m_mouseButtons[RIMOUSE::OTHERBUTTON2] = false;
}

VOID RareInput::ProcessKeyboardInput(CONST PBYTE buffer)
{
	// Sanity check. Make sure the buffer exists.
	if (!buffer)
		return;

	// Get a reference to the RAWINPUT keyboard structure.
	RAWKEYBOARD& keyboard = ((PRAWINPUT)buffer)->data.keyboard;

	// Determine what key is being pressed/released by extracting the virtual key code.
	USHORT keyCode = keyboard.VKey;

	// Update the state of the corresponding key.
	//
	// NOTE: The RAWINPUT keyboard.flags structure can contain multiple
	//       bits of input data, include RI_KEY_MAKE (key is down), 
	//       RI_KEY_BREAK (key is down), RI_KEY_E0 (left identifier),
	//       and/or RI_KEY_E0 (right identifier). Thus, one must use
	//       bitwise comparison to check for specific flags!
	if ((keyboard.Flags & RI_KEY_MAKE) == 0)
		m_keys[keyCode] = true;

	if ((keyboard.Flags & RI_KEY_BREAK) == 1)
		m_keys[keyCode] = false;
}

VOID RareInput::ProcessRawInputMessages(LPARAM lParam)
{
	// Check that the captured input data is not larger than 40 bytes.
	UINT bufferSize = 0;
	GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &bufferSize, sizeof(RAWINPUTHEADER));
	if ((bufferSize <= 0) || (bufferSize > 40))
		return; // CONSIDER IMPLEMENTING AN ERROR HANDLER?

	// Use a static sized buffer, since memory allocation in each input cycle would impact performance.
	// Mouse RawInput data is 40 bytes, and Keyboard input data is 32 bytes.
	// Thus, we use the mouse buffer size to accommodate both devices.
	bufferSize = 40;
	std::unique_ptr<BYTE[]> inputBuffer = std::make_unique<BYTE[]>(bufferSize);

	// Write the captured Raw Input data to the buffer.
	GetRawInputData((HRAWINPUT)lParam, RID_INPUT, (LPVOID)inputBuffer.get(), &bufferSize, sizeof(RAWINPUTHEADER));

	// Get the input device type from the RawInput header in the buffer.
	DWORD inputDeviceType = ((PRAWINPUT)inputBuffer.get())->header.dwType;

	// Process Keyboard input...
	if (inputDeviceType == RIM_TYPEKEYBOARD)
		ProcessKeyboardInput(inputBuffer.get());

	if (inputDeviceType == RIM_TYPEMOUSE)
		ProcessMouseInput(inputBuffer.get());

	return;
}