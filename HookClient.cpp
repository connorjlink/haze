#include "HookClient.h"

import std;

// Haze HookClient.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	HookClient::HookClient(const std::string& ip, const std::string& port, const std::string& id)
		: _client{ std::format("mqtt://{}:{}", ip, port), id }
	{
		using namespace std::chrono_literals;

		mqtt::string test;

		_options = mqtt::connect_options_builder()
			.keep_alive_interval(5s)
			.connect_timeout(5s)
			.clean_session(true)
			.automatic_reconnect(true)
			.finalize();
	}

	void HookClient::connect()
	{
		_client.connect(_options);
	}

	void HookClient::disconnect()
	{
		_client.disconnect();
	}

	void HookClient::subscribe(const std::string& topic, QoS qos)
	{
		_client.subscribe(topic.data(), qos);
	}

	void HookClient::unsubscribe(const std::string& topic)
	{
		_client.unsubscribe(topic.data());
	}

	void HookClient::publish(const std::string& topic, const std::string& payload, QoS qos, bool retain)
	{
		const auto message = mqtt::make_message(topic.data(), payload.data(), qos, retain);
		_client.publish(message);
	}
}