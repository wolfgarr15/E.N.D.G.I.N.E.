#include "FileUtils.hpp"

namespace FileUtils {
	/* Private methods and variables via inner anonymous namespace */
	namespace {
		void WriteFile(const std::string& filename, std::string* contents, bool overwrite) {
			std::ofstream fout;

			if (overwrite) {
				fout.open(filename, std::ofstream::out);
			} else {
				fout.open(filename, std::ofstream::out | std::ios_base::app);
			}

			if (fout.fail() || !fout) {
				throw CantOpenFile();
			}

			fout << *contents;

			fout.flush();
			fout.close();
		}
	}

	/* Public methods */
	bool FileExists(const std::string& filename) {
		struct stat buffer;
		return (stat(filename.c_str(), &buffer) == 0);
	}

	std::ifstream* OpenFile(const std::string& filename) {
		// Check to make sure the file exists
		if (!FileExists(filename)) {
			throw FileNotFound();
		}

		// Create the input filestream
		std::ifstream in(filename);
		if(in.fail() || !in) {
			throw CantOpenFile();
		}

		return &in;
	}

	std::string* BufferFile(const std::string& filename) {
		// Open the file and return the buffer
		return BufferFile(OpenFile(filename));
	}

	std::string* BufferFile(std::ifstream* fin) {
		// Get the file length
		fin->seekg(0, std::ios::end);
		std::streampos len = fin->tellg();
		fin->seekg(0, std::ios::beg);
		// Create a string buffer
		std::string contents(len, ' ');
		// Read into the string buffer
		fin->read(&contents[0], len);
		// Close the file
		fin->close();

		return &contents;
	}

	// Append to a file if it exists
	inline void WriteToFile(const std::string& filename, std::string* contents) {
		WriteFile(filename, contents, false);
	}

	// Overwrite a file if it exists
	inline void CreateFile(const std::string& filename, std::string* contents) {
		WriteFile(filename, contents, true);
	}
};