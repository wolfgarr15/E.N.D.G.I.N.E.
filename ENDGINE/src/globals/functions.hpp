///////////////////////////////////////////////////////////////////////
// Filenam: functions.hpp
//
// Purpose: Includes global utility function definitions used 
//          throughout the E.N.D.G.I.N.E. project.
///////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Preprocessor Compile Guard
//--------------------------------------------------------------------
#pragma once

///////////////////////////////////////////////////////////////////////
// Required Header Includes
//---------------------------------------------------------------------
#include <memory>
#include <string>

///////////////////////////////////////////////////////////////////////
// Function Definitions
//---------------------------------------------------------------------
namespace Convert
{
	inline std::string Wc_strToString(const wchar_t* wc_str)
	{
		std::wstring tmp(wc_str);
		return std::string(tmp.begin(), tmp.end());
	}
}