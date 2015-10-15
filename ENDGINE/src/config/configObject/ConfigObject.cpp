///////////////////////////////
// Filename: ConfigObject.cpp
////////////////////////////////

///////////////////////////////
// Class Header
//-----------------------------
#include "ConfigObject.hpp"

//////////////////////////////
// Class Definition
//----------------------------
bool ConfigObject::Initialize(const std::string& defaultFile,
							  const std::string& fileToLoad)
{
	if (!FileExists(defaultFile))
		throw NoDefault();

	if (!LoadFile(defaultFile))
		throw InvalidDefault();

	m_default = defaultFile;
	m_config = fileToLoad;

	if (LoadFile(fileToLoad))
		return true;
	
	else
	{
		CreateFile(fileToLoad);
		return true;
	}
}

bool ConfigObject::UpdateFile()
{
	return CreateFile(m_config);
}

bool ConfigObject::RestoreDefaults()
{
	LoadFile(m_default);
	return UpdateFile();
}

bool ConfigObject::FileExists(const std::string& fileName)
{
	struct stat buffer;
	return (stat(fileName.c_str(), &buffer) == 0);
}

bool ConfigObject::LoadFile(const std::string& fileToLoad)
{
	try
	{
		Read_(fileToLoad);
	}
	catch (const std::exception& e)
	{
		// ADD ERROR HANDLER (Logging?)
		return false;
	}

	return true;
}

bool ConfigObject::CreateFile(const std::string& writeToFile)
{
	try
	{
		Write_(writeToFile);
	}
	catch (const std::exception& e)
	{
		// ADD ERROR HANDLER (Logging?)
		return false;
	}

	return true;
}



