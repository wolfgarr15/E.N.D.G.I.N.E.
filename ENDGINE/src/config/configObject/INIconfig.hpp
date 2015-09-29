////////////////////////////////
// Filename: INIconfig.hpp
//
// Author: Wolfe S. Greene
////////////////////////////////

////////////////////////////////
// Pre-processor Directives
//------------------------------
#pragma once

///////////////////////////////
// Include Headers
//-----------------------------
#include "ConfigObject.hpp"
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
	void Read_(const std::string& fileToRead) override;
	void Write_(const std::string& fileToWrite) override;
};

