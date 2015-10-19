/*******************************************************************************
 * Filename: FileUtils.hpp                                                    *
 * Author: William Gehring                                                    *
 * Date: 2015/09/30                                                           *
 *                                                                            *
 * A set of utilities for reading and buffering files                         *
*******************************************************************************/

#pragma once

#include <sys\stat.h>
#include <fstream>
#include <string>
#include <sstream>
#include <exception>

namespace FileUtils {
	struct FileNotFound : std::runtime_error {
		FileNotFound() : runtime_error("Error : File not found!") {}
		FileNotFound(const std::string& filename) : runtime_error("Error : File " + filename + " not found!") {}
	};

	struct CantOpenFile : std::runtime_error {
		CantOpenFile() : runtime_error("Error : Can't open file!") {}
		CantOpenFile(const std::string& filename) : runtime_error("Error : Can't open file " + filename + "!") {}
	};

	bool FileExists(const std::string& name);
	std::ifstream* OpenFile(const std::string& name);
	std::string* BufferFile(std::string& filename);
	std::string* BufferFile(std::ifstream* fin);
	void WriteToFile(const std::string& filename, std::string* contents);
	void CreateFile(const std::string& filename, std::string* contents);
};