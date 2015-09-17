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
// Global Function Definitions
//---------------------------------------------------------------------
namespace Convert
{
	static inline bool StringToWstring(const std::string& inString, std::wstring& outWstring)
	{
		// Determine the size of the intermediate wchar_t array.
		//
		// Note: Add 1 to the inString length to account 
		//       for the C-string termination character.
		size_t wc_strSize = inString.size() + 1;
		size_t garbage;

		// Create a wchar_t array to hold the intermediate wchar_t C-string.
		std::unique_ptr<wchar_t[]> wc_str = std::make_unique<wchar_t[]>(wc_strSize);

		// Convert the multibyte string to a wide character string.
		// Returns false if the conversion fails.
		if(mbstowcs_s(&garbage, wc_str.get(), wc_strSize, inString.c_str(), _TRUNCATE))
			return false;

		// Return the converted string.
		outWstring = std::wstring(wc_str.get());
		return true;
	}

	static inline bool WstringToString(const std::wstring& inWstring, std::string& outString)
	{
		// Determine the size of the intermediate char array.
		//
		// Note: Add 1 to the inString length to account 
		//       for the C-string termination character.
		size_t c_strSize = inWstring.size() + 1;

		// Create a char array to hold the intermediate char C-string string.
		std::unique_ptr<char[]> c_str = std::make_unique<char[]>(c_strSize);

		// Convert the multibyte string to a wide character string.
		// Returns false if the conversion fails.
		size_t garbage;
		if(wcstombs_s(&garbage, c_str.get(), c_strSize, inWstring.c_str(), _TRUNCATE))
			return false;

		// Return the converted string.
		outString = std::string(c_str.get());
		return true;
	}
}