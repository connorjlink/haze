import std;

#include "JSON.h"
#include "AutoJSON.h"

// Haze AutoJSON.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	decltype(AutoJSONObject::object) AutoJSONObject::get_object(void) const noexcept
	{
		return object;
	}

	std::string AutoJSONObject::serialize(void) const noexcept
	{
		return object->serialize();
	}

	void AutoJSONObject::upsert(const std::string& name, JSON* json)
	{
		object->upsert(name, json);
	}

	AutoJSONObject::AutoJSONObject()
	{
		object = new JSONObject{};
	}

	AutoJSONObject::~AutoJSONObject()
	{
		delete object;
	}


	decltype(AutoJSONArray::array) AutoJSONArray::get_array(void) const noexcept
	{
		return array;
	}

	std::string AutoJSONArray::serialize(void) const noexcept
	{
		return array->serialize();
	}

	void AutoJSONArray::append(JSON* json)
	{
		array->append(json);
	}

	AutoJSONArray::AutoJSONArray()
	{
		array = new JSONArray{};
	}

	AutoJSONArray::~AutoJSONArray()
	{
		delete array;
	}
}
