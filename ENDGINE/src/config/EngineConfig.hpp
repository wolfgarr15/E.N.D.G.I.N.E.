////////////////////////////////
// Filename: EngineConfig.hpp
////////////////////////////////

////////////////////////////////
// Pre-processor Directives
//------------------------------
#pragma once

///////////////////////////////
// Include Headers
//-----------------------------
#include "..\globals\LeanWindows.h"
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

