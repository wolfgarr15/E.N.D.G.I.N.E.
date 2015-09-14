///////////////////////////////
// Filename: XMLconfig.cpp
////////////////////////////////

///////////////////////////////
// Class Header
//-----------------------------
#include "XMLconfig.hpp"

//////////////////////////////
// Class Definition
//----------------------------
void XMLconfig::Read_(const std::string& fileToRead)
{
	boost::property_tree::xml_parser::read_xml(fileToRead, m_data, boost::property_tree::xml_parser::trim_whitespace);
}

void XMLconfig::Write_(const std::string& fileToWrite)
{
	boost::property_tree::xml_parser::xml_writer_settings<std::string> settings('\t', 1);
	boost::property_tree::xml_parser::write_xml(fileToWrite, m_data, std::locale(), settings);
}
