/////////////////////////////////////////////////////////
// Filename: BaseWindow.hpp
/////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
// Preprocessor Directives
//------------------------------------------------------
#pragma once

/////////////////////////////////////////////////////////
// Header Includes
//-------------------------------------------------------
#include <string>
#include "..\globals\LeanWindows.h"
#include "..\interfaces\IMessageListener.hpp"
#include "..\config\EngineConfig.hpp"

////////////////////////////////////////////////////////
// Class Declaration
//-----------------------------------------------------
class BaseWindow
{
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	BOOL m_bFullscreen;
	BOOL m_bWindowClosed;
	std::string m_sClassName;
	std::string m_sWndTitle;

	IMessageListener* m_pListener;

public:
	BaseWindow(HINSTANCE hInstance, CONST PWNDCLASSEX pWcex = NULL);
	~BaseWindow();

	BOOL Register();
	BOOL Register(UINT uStyle, HICON hIcon, HCURSOR hCursor, HBRUSH hBackgroundBrush,
		                       LPCTSTR lpszMenuName, LPCTSTR lpszClassName, HICON hIconSm);
	BOOL Register(CONST PWNDCLASSEX pWcex);

	BOOL Create(CONST EngineConfig* pConfig, DWORD dwStyles = WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME | WS_VISIBLE);
	BOOL Create(BOOL bFullscreen, INT iWindowWidth = 800, INT iWindowHeight = 600, 
		                          DWORD dwStyles = WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME | WS_VISIBLE);

	BOOL GetMessage(CONST PMSG pMessage) CONST;

	BOOL SetWindowCursorPos(CONST PPOINT position);
	BOOL SetWindowCursorPos(INT xPos, INT yPos);

	VOID SetWindowTitle(CONST LPCTSTR lpszTitle);

	VOID SetListener(IMessageListener* pListener);

	HWND GetHandle() CONST;
	BOOL IsFullscreen() CONST;

protected:
	virtual LRESULT HandleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	BaseWindow(CONST BaseWindow& other) = delete;
	BaseWindow& operator=(CONST BaseWindow& other) = delete;

	VOID SetFullscreen(DWORD& dwStyles, INT& iScreenWidth, INT& iScreenHeight);
	VOID SetWindowed(INT iWondowWidth, INT iWindowHeight, INT& iXposition, INT& iYpostion);

	LRESULT RouteMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	inline static BaseWindow* GetObjectPointer(HWND hWnd)
	{
		return (BaseWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	}
};

