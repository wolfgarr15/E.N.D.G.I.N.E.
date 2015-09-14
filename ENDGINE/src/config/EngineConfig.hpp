////////////////////////////////
// Filename: EngineConfig.hpp
////////////////////////////////

////////////////////////////////
// Pre-processor Directives
//------------------------------
#pragma once
#define WIN32_LEAN_AND_MEAN

///////////////////////////////
// Include Headers
//-----------------------------
#include <Windows.h>
#include "configObject\INIconfig.hpp"

////////////////////////////////////
// Class Declaration
//----------------------------------
class EngineConfig : 
	public INIconfig // Engine-specific wrapper of INIconfig.
					 //
	                 // NOTE: If we decide to change configuration
					 //       file format, we may need to modify
					 //       EngineConfig's member functions.
{
public:
	EngineConfig() = default;
	~EngineConfig() = default;

	BOOL IsFullscreen() CONST;
	BOOL UseVsync() CONST;

	INT GetWindowWidth() CONST;
	INT GetWindowHeight() CONST;

	FLOAT GetScreenFar() CONST;
	FLOAT GetScreenNear() CONST;
};

