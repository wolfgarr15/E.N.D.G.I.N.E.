///////////////////////////////////
// Filename: EngineConfig.cpp
//
// Author: Wolfe S. Greene
///////////////////////////////////

////////////////////////////////////////////////////////////////
// Class Header
//-------------------------------------------------------------
#include "EngineConfig.hpp"

///////////////////////////////////////////////////////////////////
// Class Definition
//-----------------------------------------------------------------
BOOL EngineConfig::IsFullscreen() CONST
{
	return GetValue<BOOL>("WINDOW.bFullscreen");
}

BOOL EngineConfig::UseVsync() CONST
{
	return GetValue<BOOL>("RENDERER.bVsync");
}

INT EngineConfig::GetWindowWidth() CONST
{
	return GetValue<INT>("WINDOW.iWidth");
}

INT EngineConfig::GetWindowHeight() CONST
{
	return GetValue<INT>("WINDOW.iHeight");
}

FLOAT EngineConfig::GetScreenFar() CONST
{
	return GetValue<FLOAT>("RENDERER.fScreenFar");
}

FLOAT EngineConfig::GetScreenNear() CONST
{
	return GetValue<FLOAT>("RENDERER.fScreenNear");
}