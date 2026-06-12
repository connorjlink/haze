#ifndef HAZE_JSON_H
#define HAZE_JSON_H

#include <utility/Variant.h>

// Haze JSON.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum struct JSONKind
	{
		ARRAY,
		OBJECT,
		VALUE,
	};
	

	struct JSON;
	struct JSONObjectEntry;

	using JSONArray = std::vector<JSON>;
	using JSONObject = std::vector<JSONObjectEntry>;


	using JSONVariant = std::variant
	<
		std::monostate,
		bool,
		double,
		std::string,
		JSONArray,
		JSONObject
	>;


	template<typename... Ts>
	struct Overloaded : public Ts...
	{
		using Ts::operator()...;
	};

	template<typename... Ts>
	Overloaded(Ts...) -> Overloaded<Ts...>;

	struct JSONSerializer
	{
	public:
		void operator()([[maybe_unused]] std::monostate, std::string&) const;
		void operator()(double, std::string&) const;
		void operator()(const std::string&, std::string&) const;
		void operator()(const JSONArray&, std::string&) const;
		void operator()(const JSONObject&, std::string&) const;
	};


	struct JSON
	{
	public:
		JSONVariant value;

	public:
		std::string serialize() const
		{
			auto result = std::string{};
			result.reserve(128);

			std::visit(JSONSerializer{}, value, std::ref(result));

			return result;
		}

	public:
		constexpr JSON()
			: value{ std::monostate{} }
		{
		}

		template<typename T>
		constexpr JSON(T value)
			: value{ std::move(value) }
		{
		}
	};

	struct JSONObjectEntry
	{
		std::string key;
		JSON value;
	};
}

#endif
