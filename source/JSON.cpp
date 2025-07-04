import std;

#include <utility/JSON.h>

// Haze JSON.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	JSONType JSONValue::jtype(void) const noexcept
	{
		return JSONType::VALUE;
	}


	JSONValueType StringJSONValue::vtype(void) const noexcept
	{
		return JSONValueType::STRING;
	}

	std::string StringJSONValue::serialize(void) const noexcept
	{
		return std::format("\"{}\"", value);
	}


	JSONValueType NumberJSONValue::vtype(void) const noexcept
	{
		return JSONValueType::NUMBER;
	}

	std::string NumberJSONValue::serialize(void) const noexcept
	{
		return std::format("{}", value);
	}


	JSONValueType BooleanJSONValue::vtype(void) const noexcept
	{
		return JSONValueType::BOOLEAN;
	}

	std::string BooleanJSONValue::serialize(void) const noexcept
	{
		return std::format("{}", value);
	}


	JSONValueType NullJSONValue::vtype(void) const noexcept
	{
		return JSONValueType::NULL;
	}

	std::string NullJSONValue::serialize(void) const noexcept
	{
		return "null";
	}



	void JSONObject::upsert(const std::string& name, JSON* json)
	{
		members[name] = json;
	}

	JSONType JSONObject::jtype(void) const noexcept
	{
		return JSONType::OBJECT;
	}

	std::string JSONObject::serialize(void) const noexcept
	{
		const auto children = members
			| std::ranges::views::transform([](const auto& pair) 
				{ return std::format(R"("{}":{})", pair.first, pair.second->serialize()); })
			| std::ranges::views::join_with(std::string(","))
			| std::ranges::to<std::string>();

		return std::format(R"({{{}}})", children);
	}


	void JSONArray::append(JSON* json)
	{
		objects.emplace_back(json);
	}

	JSONType JSONArray::jtype(void) const noexcept
	{
		return JSONType::ARRAY;
	}

	std::string JSONArray::serialize(void) const noexcept
	{
		const auto children = objects
			| std::ranges::views::transform([](const JSON* j) { return j->serialize(); })
			| std::ranges::views::join_with(std::string(","))
			| std::ranges::to<std::string>();
		
		return std::format("[{}]", children);
	}
}
