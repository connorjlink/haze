#ifndef HAZE_JSON_H
#define HAZE_JSON_H

// Haze JSON.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class JSONType
	{
		OBJECT,
		ARRAY,
		VALUE,
	};

	class JSON
	{
	public:
		virtual JSONType jtype(void) const noexcept = 0;
		virtual std::string serialize(void) const noexcept = 0;

	public:
		virtual ~JSON() = default;
	};


	enum class JSONValueType
	{
		STRING,
		NUMBER,
		BOOLEAN,
		NULL,
	};

	class JSONValue : public JSON
	{
	public:
		virtual JSONType jtype(void) const noexcept override;
		virtual JSONValueType vtype(void) const noexcept = 0;

	public:
		virtual ~JSONValue() = default;
	};

	class StringJSONValue : public JSONValue
	{
	public:
		std::string value;

	public:
		JSONValueType vtype(void) const noexcept override;
		std::string serialize(void) const noexcept override;

	public:
		StringJSONValue(const std::string& value) : value { value } {}
		virtual ~StringJSONValue() = default;
	};

	class NumberJSONValue : public JSONValue
	{
	public:
		std::int32_t value;

	public:
		JSONValueType vtype(void) const noexcept override;
		std::string serialize(void) const noexcept override;

	public:
		NumberJSONValue(std::int32_t value) : value{ value } {}
		virtual ~NumberJSONValue() = default;
	};

	class BooleanJSONValue : public JSONValue
	{
	public:
		bool value;

	public:
		JSONValueType vtype(void) const noexcept override;
		std::string serialize(void) const noexcept override;

	public:
		BooleanJSONValue(bool value) : value{ value } {}
		virtual ~BooleanJSONValue() = default;
	};

	class NullJSONValue : public JSONValue
	{
	public:
		JSONValueType vtype(void) const noexcept override;
		std::string serialize(void) const noexcept override;

	public:
		NullJSONValue() {}
		virtual ~NullJSONValue() = default;
	};




	class JSONObject : public JSON
	{
	public:
		std::unordered_map<std::string, JSON*> members;

	public:
		void upsert(const std::string&, JSON*);

	public:
		JSONType jtype(void) const noexcept override;
		std::string serialize(void) const noexcept override;

	public:
		virtual ~JSONObject()
		{
#pragma message ("TODO FIX THE OBJECT DELETION")
			/*for (auto it = members.begin(); it != members.end(); it++)
			{
				delete it->second;
			}
			members.clear();*/
		}
	};

	
	class JSONArray : public JSON
	{
	public:
		std::vector<JSON*> objects;

	public:
		void append(JSON*);

	public:
		JSONType jtype(void) const noexcept override;
		std::string serialize(void) const noexcept override;

	public:
		virtual ~JSONArray()
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
