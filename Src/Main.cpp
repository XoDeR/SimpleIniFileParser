#include "Rjson.h"

#include <vector>
#include <string>
#include <map>

#include <iostream>
#include <fstream>
#include <cstdint>

namespace Rio
{

	void testIniFileParser()
	{
		using std::string;

		string filename = R"(c:\Users\Walter\Documents\GitHub\SimpleIniFileParser\Tests\A000.ini)";

		// open file and read all the content to a buffer
		std::ifstream file(filename, std::ios::binary | std::ios::ate);
		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);

		std::vector<char> buffer(size);
		if (file.read(buffer.data(), size))
		{
			std::map<std::string, const char*> settings;

			using namespace Rjson;
			Rjson::parse(buffer, settings);

			float floatVal = Rjson::parseFloat(settings["floatVal"]); // 0.1
			uint32_t uintVal = Rjson::parseUint(settings["uintVal"]); // 255
			int32_t intVal = Rjson::parseInt(settings["intVal"]); // -1
			std::string str;
			Rjson::parseString(settings["stringVal"], str); // "XXX"
			bool boolVal = Rjson::parseBool(settings["boolVal"]); // true
		}
	}

} // namespace Rio

int main()
{
	Rio::testIniFileParser();
	return 0;
}

