#include "data.h"
#include "config.h"
#include "main.h"

APPData::APPData()
{
	nvs = new NvsManager(release);

	today_version = nvs->get_variable<uint32_t>("today_version");
	if (!today_version)
	{
		init_flag = true;
		APPData::init();
	}
	healthy_score = 0;
	last_drinking_time = nvs->get_variable<uint32_t>("l_drink_time");
	dark_time = nvs->get_variable<uint32_t>("dark_time");
	on_chair_time = nvs->get_variable<uint32_t>("on_chair_time");

	drink_times = nvs->get_variable<uint8_t>("drink_times");
	long_chair_times = nvs->get_variable<uint8_t>("lo_chair_times");

	history_drink_times = nvs->get_array<uint8_t>("h_drink_times");
	history_dark_time = nvs->get_array<uint32_t>("h_dark_time");
	history_long_chair_times = nvs->get_array<uint8_t>("h_lo_chair_time");
	
	last_day_drink_times = history_drink_times[1];
	last_day_dark_time = history_dark_time[1];
	last_day_long_chair_times = history_long_chair_times[1];
	init_flag = false;
}

APPData::~APPData()
{
	delete nvs;
}

void APPData::init()
{
	last_drinking_time = 100;
	dark_time = 60;
	on_chair_time = 300;
	drink_times = 0;
	long_chair_times = 0;
	last_day_drink_times = 0;
	last_day_dark_time = 0;
	last_day_long_chair_times = 0;
	uint8_t temp_history_drink_times[7] = {0, 0, 0, 0, 0, 0, 0};
	uint32_t temp_history_dark_time[7] = {0, 0, 0, 0, 0, 0, 0};
	uint8_t temp_history_long_chair_times[7] = {0, 0, 0, 0, 0, 0, 0};
	nvs->set_array("h_drink_times", temp_history_drink_times, sizeof(temp_history_drink_times));
	nvs->set_array("h_dark_time", temp_history_dark_time, sizeof(temp_history_dark_time));
	nvs->set_array("h_lo_chair_time", temp_history_long_chair_times, sizeof(temp_history_long_chair_times));
	APPData::storage();
}

void APPData::storage()
{
	if (!init_flag)
	{
		history_drink_times[0] = drink_times;
		history_dark_time[0] = dark_time;
		history_long_chair_times[0] = long_chair_times;
	}
	// healthy sorce caculate
	healthy_score = 
    ((last_day_drink_times >= 7) ? 30 : 
     (last_day_drink_times >= 5) ? 20 : 
     (last_day_drink_times >= 3) ? 10 : 0) +
    ((last_day_long_chair_times <= 4) ? 40 : 
     (last_day_long_chair_times <= 6) ? 30 : 
     (last_day_long_chair_times <= 8) ? 20 : 0) +
    ((last_day_dark_time <= 3600) ? 30 : 
     (last_day_dark_time <= 7200) ? 20 : 
     (last_day_dark_time <= 10800) ? 10 : 0);

	nvs->set_variable<uint32_t>("l_drink_time", last_drinking_time);
	nvs->set_variable<uint32_t>("dark_time", dark_time);
	nvs->set_variable<uint32_t>("on_chair_time", on_chair_time);

	nvs->set_variable<uint8_t>("drink_times", drink_times);
	nvs->set_variable<uint8_t>("lo_chair_times", long_chair_times);

	nvs->set_variable<uint32_t>("today_version", today_version);
	if (!init_flag)
	{
	nvs->set_array("h_drink_times", history_drink_times.get(), sizeof(history_drink_times));
	nvs->set_array("h_dark_time", history_dark_time.get(), sizeof(history_dark_time));
	nvs->set_array("h_lo_chair_time", history_long_chair_times.get(), sizeof(history_long_chair_times));
	}
}

void APPData::refresh_day()
{
	time_t now = time(nullptr);
	if (refresh_time < now)
	{
		refresh_time = now - (now % 86400) - 4 * 3600;
	}
	
	if (synced_time)
	{
		uint32_t real_today_version = now / 86400;
		uint16_t mis_day = real_today_version - today_version;
		today_version = real_today_version;
		if (mis_day > 1)
		{
			if (mis_day > 7) mis_day = 7;
			for (uint8_t x = 1; x < mis_day; ++x)
			{
				for (int i = 7 - 1; i > 0; --i) {
					history_drink_times[i] = history_drink_times[i - 1];
					history_dark_time[i] = history_dark_time[i - 1];
					history_long_chair_times[i] = history_long_chair_times[i - 1];
				}
				history_drink_times[0] = 0;
				history_dark_time[0] = 0;
				history_long_chair_times[0] = 0;

				last_drinking_time = 0;
				dark_time = 0;
				on_chair_time = 0;
				drink_times = 0;
				long_chair_times = 0;
				last_day_drink_times = 0;
				last_day_dark_time = 0;
				last_day_long_chair_times = 0;
			}
		}
	}
	if (now > refresh_time)
	{
		APPData::storage();
		for (int i = 7 - 1; i > 0; --i) {
			history_drink_times[i] = history_drink_times[i - 1];
			history_dark_time[i] = history_dark_time[i - 1];
			history_long_chair_times[i] = history_long_chair_times[i - 1];
		}
		history_drink_times[0] = 0;
		history_dark_time[0] = 0;
		history_long_chair_times[0] = 0;

		last_day_drink_times = drink_times;
		last_day_dark_time = dark_time;
		last_day_long_chair_times = long_chair_times;

		last_drinking_time = 0;
		dark_time = 0;
		on_chair_time = 0;

		drink_times = 0;
		long_chair_times = 0;

		refresh_time += 86400;
	}
}
