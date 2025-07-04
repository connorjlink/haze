#ifndef HAZE_AUTOJSON_H
#define HAZE_AUTOJSON_H

// Haze AutoJSON.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class JSON;
	class JSONObject;
	class JSONArray;

	// these are basically smart pointer classes, but they are somewhat nice for clarity/intention purpose

	class AutoJSONObject
	{
	private:
		JSONObject* object;

	public:
		decltype(object) get_object(void) const noexcept;

	public:
		void upsert(const std::string&, JSON*);
		std::string serialize(void) const noexcept;

	public:
		AutoJSONObject();
		~AutoJSONObject();
	};

	class AutoJSONArray
	{
	private:
		JSONArray* array;

	public:
		decltype(array) get_array(void) const noexcept;
		std::string serialize(void) const noexcept;

	public:
		void append(JSON*);

	public:
		AutoJSONArray();
		~AutoJSONArray();
	};
}

#endif
