////////////////////////////////
// Filename: JSONconfig.hpp
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
#include <boost\property_tree\json_parser.hpp>

//////////////////////////////
// Class Declaration
//----------------------------
class JSONconfig :
	public ConfigObject
{
public:
	JSONconfig() = default;
	~JSONconfig() = default;

private:
	void Read_(const std::string& fileToRead) override;
	void Write_(const std::string& fileToWrite) override;
};

