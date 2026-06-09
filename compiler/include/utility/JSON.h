#ifndef HAZE_JSON_H
#define HAZE_JSON_H

// Haze JSON.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum struct JSONType
	{
		OBJECT,
		ARRAY,
		VALUE,
	};

	struct JSON
	{
	public:
		virtual JSONType jtype(void) const noexcept = 0;
		virtual std::string serialize(void) const noexcept = 0;

	public:
		virtual ~JSON() = default;
	};


	enum struct JSONValueType
	{
		STRING,
		NUMBER,
		BOOLEAN,
		NULL,
	};

	struct JSONValue : public JSON
	{
	public:
		virtual JSONType jtype(void) const noexcept override;
		virtual JSONValueType vtype(void) const noexcept = 0;

	public:
		~JSONValue() = default;
	};

	struct StringJSONValue : public JSONValue
	{
	public:
		std::string value;

	public:
		JSONValueType vtype(void) const noexcept override;
		std::string serialize(void) const noexcept override;

	public:
		StringJSONValue(const std::string& value) : value { value } 
		{
		}

	public:
		~StringJSONValue() = default;
	};

	struct NumberJSONValue : public JSONValue
	{
	public:
		std::int32_t value;

	public:
		JSONValueType vtype(void) const noexcept override;
		std::string serialize(void) const noexcept override;

	public:
		NumberJSONValue(std::int32_t value) : value{ value } 
		{
		}

	public:
		~NumberJSONValue() = default;
	};

	struct BooleanJSONValue : public JSONValue
	{
	public:
		bool value;

	public:
		JSONValueType vtype(void) const noexcept override;
		std::string serialize(void) const noexcept override;

	public:
		BooleanJSONValue(bool value) : value{ value } 
		{
		}

	public:
		~BooleanJSONValue() = default;
	};

	struct NullJSONValue : public JSONValue
	{
	public:
		JSONValueType vtype(void) const noexcept override;
		std::string serialize(void) const noexcept override;

	public:
		NullJSONValue() 
		{
		}

	public:
		~NullJSONValue() = default;
	};


	struct JSONObject : public JSON
	{
	public:
		std::unordered_map<std::string, JSON*> members;

	public:
		void upsert(const std::string&, JSON*);

	public:
		JSONType jtype(void) const noexcept override;
		std::string serialize(void) const noexcept override;

	public:
		~JSONObject()
		{
			for (auto& [key, value] : members)
			{
				delete value;
			}
			members.clear();
		}
	};

	
	struct JSONArray : public JSON
	{
	public:
		std::vector<JSON*> objects;

	public:
		void append(JSON*);

	public:
		JSONType jtype(void) const noexcept override;
		std::string serialize(void) const noexcept override;

	public:
		~JSONArray()
		{
			for (auto it = objects.begin(); it != objects.end(); it++)
			{
				delete *it;
			}
			objects.clear();
		}
	};
}

#endif
