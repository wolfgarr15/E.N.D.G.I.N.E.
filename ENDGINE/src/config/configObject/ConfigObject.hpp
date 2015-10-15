////////////////////////////////
// Filename: ConfigObject.hpp
////////////////////////////////

////////////////////////////////
// Pre-processor Directives
//------------------------------
#pragma once

///////////////////////////////
// Include Headers
//-----------------------------
#include <sys\stat.h>
#include <string>
#include <exception>
#include <boost\property_tree\ptree.hpp>
#include <boost\lexical_cast.hpp>

//////////////////////////////
// Class Declaration
//----------------------------
class ConfigObject
{
	struct NoDefault : std::runtime_error
	{
		NoDefault() : runtime_error("Exception : Default configuration file does not exist!") {}
	};

	struct InvalidDefault : std::runtime_error
	{
		InvalidDefault() : runtime_error("Exception : Failed to load default configuartion file!") {}
	};

protected:
	std::string m_default;
	std::string m_config;
	boost::property_tree::ptree m_data;

public:
	ConfigObject() = default;

	virtual ~ConfigObject() = default;

	bool Initialize(const std::string& defaultFile,
				    const std::string& fileToLoad);

	bool UpdateFile();
	bool RestoreDefaults();

	template <typename T> T GetValue(const std::string& dataKey) const
	{
		return boost::lexical_cast<T>(m_data.get<std::string>(dataKey));
	}

	template <typename T> void SetValue(const std::string& dataKey, const T& dataValue)
	{
		m_data.put(dataKey, boost::lexical_cast<std::string>(dataValue));
	}

protected:
	// Pure virtual parser read/write methods.
	// These are implemented in each of the format-specific
	// subclasses: INIconfig, JSONconfig, and XMLconfig.
	virtual void Read_(const std::string& fileToRead) = 0;
	virtual void Write_(const std::string& fileToWrite) = 0;

private:
	ConfigObject(const ConfigObject& src) = delete;
	ConfigObject& operator=(const ConfigObject& other) = delete;

	bool FileExists(const std::string& fileName);
	bool LoadFile(const std::string& fileToLoad);
	bool CreateFile(const std::string& fileToCreate);
};