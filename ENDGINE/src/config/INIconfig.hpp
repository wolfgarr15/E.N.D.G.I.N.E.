////////////////////////////////
// Filename: INIconfig.hpp
////////////////////////////////

////////////////////////////////
// Pre-processor Directives
//------------------------------
#pragma once

///////////////////////////////
// Include Headers
//-----------------------------
#include "configObject\ConfigObject.hpp"
#include <boost\property_tree\ini_parser.hpp>

//////////////////////////////
// Class Declaration
//----------------------------
class INIconfig :
	public ConfigObject
{
public:
	INIconfig() = default;
	~INIconfig() = default;

private:
	void Read_(const std::string& fileToRead);
	void Write_(const std::string& fileToWrite);
};

