/////////////////////////////////////////////////////////
// Filename: BaseWindow.hpp
//
// Author: Wolfe S. Greene
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
// Class Header
//-------------------------------------------------------
#include "BaseWindow.hpp"

//////////////////////////////////////////////////////////////////////////////////////
// Class Definition
//------------------------------------------------------------------------------------
BaseWindow::BaseWindow(HINSTANCE hInstance, CONST PWNDCLASSEX pWcex)
	: m_hInstance(hInstance),
	  m_hWnd(NULL),
	  m_bWindowClosed(FALSE)
{
	if (pWcex)
		RegisterClassEx(pWcex);
}

BaseWindow::~BaseWindow()
{
	if (m_hWnd)
		DestroyWindow(m_hWnd);
}

BOOL BaseWindow::Register()
{
	WNDCLASSEX wcex;
	SecureZeroMemory(&wcex, sizeof(WNDCLASSEX));

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = BaseWindow::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = NULL; 
	wcex.lpszClassName = "BaseWindow";
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	return Register(&wcex);
}

BOOL BaseWindow::Register(UINT uStyle, HICON hIcon, HCURSOR hCursor, HBRUSH hBackgroundBrush,
	                                LPCTSTR lpszMenuName, LPCTSTR lpszClassName, HICON hIconSm)
{
	WNDCLASSEX wcex;
	SecureZeroMemory(&wcex, sizeof(WNDCLASSEX));

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = uStyle; 
	wcex.lpfnWndProc = BaseWindow::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInstance;
	wcex.hIcon = hIcon;
	wcex.hCursor = hCursor;
	wcex.hbrBackground = hBackgroundBrush;
	wcex.lpszMenuName = lpszMenuName;
	wcex.lpszClassName = lpszClassName;	
	wcex.hIconSm = hIconSm;				             

	return Register(&wcex);
}

BOOL BaseWindow::Register(CONST PWNDCLASSEX pWcex)
{
	m_sClassName = pWcex->lpszClassName;
	return RegisterClassEx(pWcex);
}

BOOL BaseWindow::Create(CONST EngineConfig* pConfig, DWORD dwStyles)
{
	return Create(pConfig->IsFullscreen(),
		          pConfig->GetWindowWidth(),
		          pConfig->GetWindowHeight(),
				  dwStyles);
}

BOOL BaseWindow::Create(BOOL bFullscreen, INT iWindowWidth, INT iWindowHeight, DWORD dwStyles)
{
	m_bFullscreen = bFullscreen;

	INT iXposition = 0;
	INT iYposition = 0;

	if (m_bFullscreen)
		SetFullscreen(dwStyles, iWindowWidth, iWindowHeight);
	else
		SetWindowed(iWindowWidth, iWindowHeight, iXposition, iYposition);

	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, m_sClassName.c_str(), m_sWndTitle.c_str(), dwStyles, iXposition, iYposition,
								iWindowWidth, iWindowHeight, NULL, NULL, m_hInstance, (PVOID)this);

	return (m_hWnd != NULL);
}

BOOL BaseWindow::GetMessage(CONST PMSG pMessage) CONST
{
	if (m_hWnd)
	{
		if (PeekMessage(pMessage, m_hWnd, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(pMessage);
			DispatchMessage(pMessage);
		}

		return TRUE;
	}
	else
		return FALSE;
}

BOOL BaseWindow::SetWindowCursorPos(CONST PPOINT position)
{
	return SetWindowCursorPos(position->x, position->y);
}

BOOL BaseWindow::SetWindowCursorPos(INT xPos, INT yPos)
{
	if (m_hWnd)
	{
		POINT p = { xPos, yPos };
		ClientToScreen(m_hWnd, &p);
		return SetCursorPos(p.x, p.y);
	}
	else
		return FALSE;
}

VOID BaseWindow::SetWindowTitle(CONST LPCTSTR lpszTitle)
{
	m_sWndTitle = lpszTitle;
}

VOID BaseWindow::SetListener(IMessageListener* pListener)
{
	m_pListener = pListener;
}

HWND BaseWindow::GetHandle() CONST
{
	return m_hWnd;
}

BOOL BaseWindow::IsFullscreen() CONST
{
	return m_bFullscreen;
}

LRESULT BaseWindow::HandleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

VOID BaseWindow::SetFullscreen(DWORD& dwStyles, INT& iScreenWidth, INT& iScreenHeight)
{
	// Set the style of the window to be borderless.
	dwStyles = WS_POPUP | WS_VISIBLE;

	// Get the screen width and height (in pixels).
	iScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	iScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Modify the screen settings for fullscreen display.
	DEVMODE dmScreenSettings;
	SecureZeroMemory(&dmScreenSettings, sizeof(DEVMODE));

	dmScreenSettings.dmSize = sizeof(DEVMODE);
	dmScreenSettings.dmPelsWidth = (ULONG)iScreenWidth;
	dmScreenSettings.dmPelsHeight = (ULONG)iScreenHeight;
	dmScreenSettings.dmBitsPerPel = 32;
	dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

	ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
}

VOID BaseWindow::SetWindowed(INT iWindowWidth, INT iWindowHeight, INT& iXposition, INT& iYposition)
{
	// Set the window position in the center of the screen.
	iXposition = (GetSystemMetrics(SM_CXSCREEN) - iWindowWidth) / 2;
	iYposition = (GetSystemMetrics(SM_CYSCREEN) - iWindowHeight) / 2;
}

LRESULT BaseWindow::RouteMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (m_pListener)
		m_pListener->ListenToMessages(hWnd, uMsg, wParam, lParam);

	return HandleMessages(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK BaseWindow::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BaseWindow* pWnd = NULL;

	if (uMsg == WM_NCCREATE)
	{
		LPCREATESTRUCT lpCs = (LPCREATESTRUCT)lParam;
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)(lpCs->lpCreateParams));
	}

	pWnd = GetObjectPointer(hWnd);

	if (pWnd)
		return pWnd->RouteMessages(hWnd, uMsg, wParam, lParam);
	else
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
}