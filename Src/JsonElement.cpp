// Copyright (c) 2015 Volodymyr Syvochka
#include "JsonElement.h"
#include "Rjson.h"

#include <cassert>

namespace Rio
{
	JsonElement::JsonElement()
		: jsonCurrentPos(NULL)
	{
	}

	JsonElement::JsonElement(const char* at)
		: jsonCurrentPos(at)
	{
	}

	JsonElement::JsonElement(const JsonElement& other)
		: jsonCurrentPos(other.jsonCurrentPos)
	{
	}

	JsonElement& JsonElement::operator=(const JsonElement& other)
	{
		// begin is the other's current position
		jsonCurrentPos = other.jsonCurrentPos;
		return *this;
	}

	bool JsonElement::toBool(bool def) const
	{
		return isNil() ? def : Rjson::parseBool(jsonCurrentPos);
	}

	int32_t JsonElement::toInt(int32_t def) const
	{
		return isNil() ? def : Rjson::parseInt(jsonCurrentPos);
	}

	uint32_t JsonElement::toUint(uint32_t def) const
	{
		return isNil() ? def : Rjson::parseUint(jsonCurrentPos);
	}

	float JsonElement::toFloat(float def) const
	{
		return isNil() ? def : Rjson::parseFloat(jsonCurrentPos);
	}

	void JsonElement::toString(std::string& str, const char* def) const
	{
		if (isNil())
		{
			str = def;
		}
		else
		{
			Rjson::parseString(jsonCurrentPos, str);
		}
	}

	void JsonElement::getAllKeys(std::vector<std::string>& keys) const
	{
		std::map<std::string, const char*> object;
		Rjson::parseObject(jsonCurrentPos, object);

		auto it = object.begin();
		while (it != object.end())
		{
			keys.push_back((*it).first);
			it++;
		}
	}

	bool JsonElement::isNil() const
	{
		if (jsonCurrentPos != NULL)
		{
			return Rjson::getJsonType(jsonCurrentPos) == JsonValueType::NIL;
		}
		return true;
	}

	bool JsonElement::isBool() const
	{
		if (jsonCurrentPos != NULL)
		{
			return Rjson::getJsonType(jsonCurrentPos) == JsonValueType::BOOL;
		}
		return false;
	}

	bool JsonElement::isNumber() const
	{
		if (jsonCurrentPos != NULL)
		{
			return Rjson::getJsonType(jsonCurrentPos) == JsonValueType::NUMBER;
		}
		return false;
	}

	bool JsonElement::isString() const
	{
		if (jsonCurrentPos != NULL)
		{
			return Rjson::getJsonType(jsonCurrentPos) == JsonValueType::STRING;
		}

		return false;
	}

	size_t JsonElement::getJsonElementSize() const
	{
		if (jsonCurrentPos == NULL)
		{
			return 0;
		}

		switch (Rjson::getJsonType(jsonCurrentPos))
		{
		case JsonValueType::NIL:
		{
			return 1;
		}
		case JsonValueType::STRING:
		{
			std::string str;
			Rjson::parseString(jsonCurrentPos, str);
			return str.size();
		}
		case JsonValueType::NUMBER:
		{
			return 1;
		}
		case JsonValueType::BOOL:
		{
			return 1;
		}
		default:
		{
			assert(false); // ("Unknown JSON value type");
			return 0;
		}
		}
	}
} // namespace Rio
