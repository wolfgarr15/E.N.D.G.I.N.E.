///////////////////////////////
// Filename: INIconfig.cpp
////////////////////////////////

///////////////////////////////
// Class Header
//-----------------------------
#include "INIconfig.hpp"

//////////////////////////////
// Class Definition
//----------------------------
void INIconfig::Read_(const std::string& fileToRead)
{
	boost::property_tree::ini_parser::read_ini(fileToRead, m_data);
}

void INIconfig::Write_(const std::string& fileToWrite)
{
	boost::property_tree::ini_parser::write_ini(fileToWrite, m_data);
}