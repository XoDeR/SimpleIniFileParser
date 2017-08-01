// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Config.h"
#include "JsonTypes.h"

#include <cstdint>
#include <string>
#include <vector>
#include <map>

namespace Rio
{
	// Functions to parse RJSON-encoded strings
	namespace Rjson
	{
		// Returns the data type of the RJSON string
		JsonValueType::Enum getJsonType(const char* json);
		void parseString(const char* json, std::string& string);
		double parseDouble(const char* json);
		int32_t parseInt(const char* json);
		uint32_t parseUint(const char* json);
		float parseFloat(const char* json);
		bool parseBool(const char* json);
		// Parses the RJSON object and puts it into object as map from
		// key to pointer to the corresponding value into the original string json
		void parseObject(const char* json, std::map<std::string, const char*>& object);

		// Parses the RJSON object and puts it into object as map from
		// key to pointer to the corresponding value into the original json string
		void parseObject(const char* json, std::map<std::string, const char*>& object);

		// Parses the RJSON-encoded json
		void parse(const char* json, std::map<std::string, const char*>& object);

		// Parses the RJSON-encoded json.
		void parse(std::vector<char>& json, std::map<std::string, const char*>& object);
	} // namespace Rjson
} // namespace Rio
