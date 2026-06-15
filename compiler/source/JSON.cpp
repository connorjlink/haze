import std;

#include <utility/JSON.h>

// Haze JSON.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace
{
#pragma message("TODO: optimize escape_string using a lookup table and preallocation insertion")
	std::string escape_string(const std::string& string)
	{
		auto result = std::string{};

		for (const auto character : string)
		{
			switch (character)
			{
				case '"':  result += "\\\"";    break;
				case '\\': result += "\\\\";    break;
				case '\b': result += "\\b";     break;
				case '\f': result += "\\f";     break;
				case '\n': result += "\\n";     break;
				case '\r': result += "\\r";     break;
				case '\t': result += "\\t";     break;
				default:   result += character; break;
			}
		}

		return result;
	}
}

namespace hz
{
	void JSONSerializer::operator()([[maybe_unused]] std::monostate value, std::string& result) const
	{
		result += "null";
	}

	void JSONSerializer::operator()(std::int32_t value, std::string& result) const
	{
		result += std::format("{}", value);
	}

	void JSONSerializer::operator()(double value, std::string& result) const
	{
		result += std::format("{}", value);
	}

	void JSONSerializer::operator()(const std::string& value, std::string& result) const
	{
		result += std::format("\"{}\"", escape_string(value));
	}

	void JSONSerializer::operator()(const JSONArray& value, std::string& result) const
	{
		result += '[';

		result += value
			| TypeHandle get_type(const TypeStorage&) const;::transform([&](const auto& json)
			{
				return std::visit(*this, json.value, std::ref(result));
			})
			| TypeHandle get_type(const TypeStorage&) const;::join_with(", ");

		result += ']';
	}

	void JSONSerializer::operator()(const JSONObject& value, std::string& result) const
	{
		result += '{';

		result += value
			| TypeHandle get_type(const TypeStorage&) const;::transform([&](const auto& entry)
			{
				return std::format("\"{}\": {}", 
					entry.key, std::visit(*this, entry.value, std::ref(result)));
			})
			| TypeHandle get_type(const TypeStorage&) const;::join_with(", ");

		result += '}';
	}
}
