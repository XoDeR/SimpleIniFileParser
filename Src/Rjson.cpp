// Copyright (c) 2015 Volodymyr Syvochka
#include "Rjson.h"
#include "StringUtils.h"

#include <cassert>

namespace Rio
{
	namespace Rjson
	{
		static const char* getNext(const char* json, const char c = 0)
		{
			assert(json != nullptr);

			if (c && c != *json)
			{
				assert(false);// , "Expected '%c' got '%c'", c, *json);
			}

			return ++json;
		}

		static const char* skipString(const char* json)
		{
			assert(json != nullptr);

			bool escaped = false;

			while ((*(json = getNext(json))) != 0)
			{
				if (*json == '"' && !escaped)
				{
					json = getNext(json);
					return json;
				}
				else if (*json == '\\')
				{
					escaped = true;
				}
				else
				{
					escaped = false;
				}
			}

			return json;
		}

		static const char* skipValue(const char* json)
		{
			assert(json != nullptr);

			switch (*json)
			{
			case '"': json = skipString(json); break;
			case '[': json = skipBlock(json, '[', ']'); break;
			case '{': json = skipBlock(json, '{', '}'); break;
			default: for (; *json != ',' && *json != '\n' && *json != ' ' && *json != '}' && *json != ']'; ++json); break;
			}

			return json;
		}

		static const char* skipComments(const char* json)
		{
			assert(json != nullptr);

			if (*json == '/')
			{
				json = getNext(json);
				if (*json == '/')
				{
					json = getNext(json, '/');
					while (*json && *json != '\n')
					{
						json = getNext(json);
					}
				}
				else if (*json == '*')
				{
					json = getNext(json);
					while (*json && *json != '*')
					{
						json = getNext(json);
					}
					json = getNext(json, '*');
					json = getNext(json, '/');
				}
				else
				{
					assert(false); // ("Bad comment");
				}
			}

			return json;
		}

		static const char* skipSpaces(const char* json)
		{
			assert(json != nullptr);

			while (*json)
			{
				if (*json == '/') json = skipComments(json);
				else if (isSpace(*json) || *json == ',') ++json;
				else break;
			}

			return json;
		}

		JsonValueType::Enum getJsonType(const char* json)
		{
			assert(json != nullptr);

			switch (*json)
			{
			case '"': return JsonValueType::STRING;
			case '-': return JsonValueType::NUMBER;
			default: return (::isdigit(*json)) ? JsonValueType::NUMBER : (*json == 'n' ? JsonValueType::NIL : JsonValueType::BOOL);
			}
		}

		void parseString(const char* json, std::string& string)
		{
			assert(json != nullptr);

			if (*json == '"')
			{
				while (*(json = getNext(json)))
				{
					// Empty string
					if (*json == '"')
					{
						json = getNext(json);
						return;
					}
					else if (*json == '\\')
					{
						json = getNext(json);

						switch (*json)
						{
						case '"': string += '"'; break;
						case '\\': string += '\\'; break;
						case '/': string += '/'; break;
						case 'b': string += '\b'; break;
						case 'f': string += '\f'; break;
						case 'n': string += '\n'; break;
						case 'r': string += '\r'; break;
						case 't': string += '\t'; break;
						default:
						{
							assert(false); // ("Bad escape character");
							break;
						}
						}
					}
					else
					{
						string += *json;
					}
				}
			}

			assert(false); // ("Bad string");
		}

		static const char* parseKey(const char* json, std::string& key)
		{
			assert(json != nullptr);

			if (*json == '"')
			{
				parseString(json, key);
				return skipString(json);
			}
			else if (::isalpha(*json)) 
			{
				while (true)
				{
					if (::isspace(*json) || *json == '=')
					{
						return json;
					}

					key += *json;
					++json;
				}
			}

			assert(false); // ("Bad key");
			return nullptr;
		}

		double parseDouble(const char* json)
		{
			assert(json != nullptr);

			std::vector<char> number;

			if (*json == '-')
			{
				number.push_back('-');
				json = getNext(json, '-');
			}
			while (isDigit(*json))
			{
				number.push_back(*json);
				json = getNext(json);
			}

			if (*json == '.')
			{
				number.push_back('.');
				while ((*(json = getNext(json))) && ::isdigit(*json)) // TODO replace for internal string function
				{
					number.push_back(*json);
				}
			}

			if (*json == 'e' || *json == 'E')
			{
				number.push_back(*json);
				json = getNext(json);

				if (*json == '-' || *json == '+')
				{
					number.push_back(*json);
					json = getNext(json);
				}
				while (::isdigit(*json)) // TODO replace for internal string function
				{
					number.push_back(*json);
					json = getNext(json);
				}
			}

			// Ensure null terminated
			number.push_back('\0');
			return Rio::parseDouble(&number[0]);
		}

		bool parseBool(const char* json)
		{
			assert(json != nullptr);

			switch (*json)
			{
			case 't':
			{
				json = getNext(json, 't');
				json = getNext(json, 'r');
				json = getNext(json, 'u');
				json = getNext(json, 'e');
				return true;
			}
			case 'f':
			{
				json = getNext(json, 'f');
				json = getNext(json, 'a');
				json = getNext(json, 'l');
				json = getNext(json, 's');
				json = getNext(json, 'e');
				return false;
			}
			default:
			{
				assert(false); // ("Bad boolean");
				return false;
			}
			}
		}

		int32_t parseInt(const char* json)
		{
			return (int32_t)parseDouble(json);
		}

		uint32_t parseUint(const char* json)
		{
			return (uint32_t)parseDouble(json);
		}

		float parseFloat(const char* json)
		{
			return (float)parseDouble(json);
		}

		void parseRootObject(const char* json, std::map<std::string, const char*>& object)
		{
			assert(json != nullptr);

			while (*json)
			{
				std::string key;
				json = parseKey(json, key);

				json = skipSpaces(json);
				json = getNext(json, '=');
				json = skipSpaces(json);

				object[key] = json;

				json = skipValue(json);
				json = skipSpaces(json);
			}
		}

		void parseObject(const char* json, std::map<std::string, const char*>& object)
		{
			assert(json != nullptr);

			if (*json == '{')
			{
				json = getNext(json, '{');

				json = skipSpaces(json);

				if (*json == '}')
				{
					getNext(json, '}');
					return;
				}

				while (*json)
				{
					std::string key;
					json = parseKey(json, key);

					json = skipSpaces(json);
					json = getNext(json, '=');
					json = skipSpaces(json);

					object[key] = json;

					json = skipValue(json);
					json = skipSpaces(json);

					if (*json == '}')
					{
						getNext(json, '}');
						return;
					}

					json = skipSpaces(json);
				}
			}

			assert(false); // ("Bad object");
		}

		void parse(const char* json, std::map<std::string, const char*>& object)
		{
			assert(json != nullptr);

			json = skipSpaces(json);
			parseRootObject(json, object);
		}

		void parse(std::vector<char>& json, std::map<std::string, const char*>& object)
		{
			json.push_back('\0');
			json.pop_back();
			parse((const char*)(&(json[0])), object);
		}

	} // namespace Rjson

} // namespace Rio
