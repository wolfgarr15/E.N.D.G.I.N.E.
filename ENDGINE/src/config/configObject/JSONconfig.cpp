///////////////////////////////
// Filename: JSONconfig.cpp
//
// Author: Wolfe S. Greene
////////////////////////////////

///////////////////////////////
// Class Header
//-----------------------------
#include "JSONconfig.hpp"

//////////////////////////////
// Class Definition
//----------------------------
void JSONconfig::Read_(const std::string& fileToRead)
{
	boost::property_tree::json_parser::read_json(fileToRead, m_data);
}

void JSONconfig::Write_(const std::string& fileToWrite)
{
	boost::property_tree::json_parser::write_json(fileToWrite, m_data);
}
