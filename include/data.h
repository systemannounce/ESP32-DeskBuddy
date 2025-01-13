#pragma once
#include <Arduino.h>
#include "my_nvs.h"

class APPData {
private:
	NvsManager* nvs;

	bool init_flag;

	uint32_t today_version;
	std::unique_ptr<uint8_t[]> history_drink_times;
	std::unique_ptr<uint32_t[]> history_dark_time;
	std::unique_ptr<uint8_t[]> history_long_chair_times;
public:
	uint32_t last_drinking_time;
	uint32_t dark_time;
	uint32_t on_chair_time;

	uint8_t drink_times;
	uint8_t long_chair_times;

	uint8_t last_day_drink_times;
	uint32_t last_day_dark_time;
	uint8_t last_day_long_chair_times;

	uint8_t healthy_score;

	// sensor
	uint16_t hx711_weight;
	uint16_t sr501;
	uint16_t light_ao;
	bool light_do;
	bool button;

 	APPData();
	~APPData();

	// 初始化存储
	void init();

	// 定时存储
	void storage();

	// 换天
	void refresh_day();

	// 健康监控(刷新)
	void health_monitor();
};