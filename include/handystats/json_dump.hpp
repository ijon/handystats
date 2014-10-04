// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_JSON_DUMP_HPP_
#define HANDYSTATS_JSON_DUMP_HPP_

#include <string>

#include <handystats/json/gauge_json_writer.hpp>
#include <handystats/json/counter_json_writer.hpp>
#include <handystats/json/timer_json_writer.hpp>
#include <handystats/json/attribute_json_writer.hpp>

#include <handystats/metrics_dump.hpp>

namespace handystats { namespace json {

template<typename Allocator>
void fill(
		rapidjson::Value& dump, Allocator& allocator,
		const metrics_dump_type& metrics_dump
	)
{
	dump.SetObject();

	const auto& metrics_map = metrics_dump.first;
	for (auto metric_iter = metrics_map.cbegin(); metric_iter != metrics_map.cend(); ++metric_iter) {
		rapidjson::Value metric_value;
		switch (metric_iter->second.which()) {
			case metrics::metric_index::GAUGE:
				json::write_to_json_value(&boost::get<metrics::gauge>(metric_iter->second), &metric_value, allocator);
				break;
			case metrics::metric_index::COUNTER:
				json::write_to_json_value(&boost::get<metrics::counter>(metric_iter->second), &metric_value, allocator);
				break;
			case metrics::metric_index::TIMER:
				json::write_to_json_value(&boost::get<metrics::timer>(metric_iter->second), &metric_value, allocator);
				break;
		}

		dump.AddMember(metric_iter->first.c_str(), allocator, metric_value, allocator);
	}

	const auto& attributes_map = metrics_dump.second;
	for (auto attr_iter = attributes_map.cbegin(); attr_iter != attributes_map.cend(); ++attr_iter) {
		rapidjson::Value attr_value;
		json::write_to_json_value(&attr_iter->second, &attr_value, allocator);

		dump.AddMember(attr_iter->first.c_str(), allocator, attr_value, allocator);
	}
}

std::string to_string(const metrics_dump_type&);

}} // namespace handystats::json

std::string HANDY_JSON_DUMP();

#endif // HANDYSTATS_JSON_DUMP_HPP_
