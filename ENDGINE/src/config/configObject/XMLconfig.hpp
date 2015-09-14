////////////////////////////////
// Filename: XMLconfig.hpp
////////////////////////////////

////////////////////////////////
// Pre-processor Directives
//------------------------------
#pragma once

///////////////////////////////
// Include Headers
//-----------------------------
#include "ConfigObject.hpp"
#include "boost\property_tree\xml_parser.hpp"

//////////////////////////////
// Class Declaration
//----------------------------
class XMLconfig :
	public ConfigObject
{
public:
	XMLconfig() = default;
	~XMLconfig() = default;

private:
	void Read_(const std::string& fileToRead) override;
	void Write_(const std::string& fileToWrite) override;
};

