#ifndef HAZE_HOOKCLIENT_H
#define HAZE_HOOKCLIENT_H

#include <string>
#include <string_view>

#include <mqtt/client.h>

// Haze HookClient.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum QoS
	{
		AT_MOST_ONCE = 0,
		AT_LEAST_ONCE = 1,
		EXACTLY_ONCE = 2,
	};

	class HookClient
	{
	private:
		mqtt::client _client;
		mqtt::connect_options _options;

	public:
		void connect();
		void disconnect();

	public:
		void subscribe(const std::string&, QoS);
		void unsubscribe(const std::string&);

	public:
		void publish(const std::string&, const std::string&, QoS, bool);

	public:
		HookClient(const std::string&, const std::string&, const std::string&);
	};
}

#endif 
