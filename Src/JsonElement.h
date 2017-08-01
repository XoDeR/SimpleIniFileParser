// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Config.h"
#include "Types.h"

#include <string>
#include <vector>

namespace Rio
{
	// Represents a JSON element.
	// The objects of this class are valid until the parser exists
	class JsonElement
	{
	public:
		// Construct the nil JsonElement.
		// Used to forward-instantiate elements or as a special nil element.
		JsonElement();
		explicit JsonElement(const char* at);
		JsonElement(const JsonElement& other);
		JsonElement& operator=(const JsonElement& other);
		// Returns the i-th item of the current array.
		JsonElement operator[](size_t i);
		// Returns the i-th item of the current array or
		// the special nil JsonElement() if the index does not exist.
		JsonElement getElementByIndexOrNil(size_t i);
		// Returns true whether the element is the JSON nil special value.
		bool isNil() const;
		// Returns true whether the element is a JSON boolean (true or false).
		bool isBool() const;
		// Returns true whether the element is a JSON number.
		bool isNumber() const;
		// Returns true whether the element is a JSON string.
		bool isString() const;
		// Returns true whether the element is a JSON array.
		bool isArray() const;
		// Returns true whether the element is a JSON object.
		bool isObject() const;
		// Returns the size of the element based on the element's type:
		// nil, bool, number: 1
		// string: length of the string
		// array: number of elements in the array
		// object: number of keys in the object
		size_t getJsonElementSize() const;
		bool toBool(bool def = false) const;
		int32_t toInt(int32_t def = 0) const;
		uint32_t toUint(uint32_t def = 0) const;
		float toFloat(float def = 0) const;
		void toString(std::string& str, const char* def = "") const;
		void getAllKeys(std::vector<std::string>& keys) const;
	private:
		const char* jsonCurrentPos;
	};
} // namespace Rio
