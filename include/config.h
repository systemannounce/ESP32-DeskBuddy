#pragma once

// system
extern const char *release;		// 版本类型
extern float      db_version;	// 版本号

// mqtt
extern const char *mqtt_server;
extern const char *mqtt_username;
extern const char *mqtt_password;
extern const char *topic;

// wifi
extern String wifi_ssid;
extern String wifi_password;

// NTP
extern const char* ntpServer;
extern const long utcOffsetInSeconds;

// Weather
extern const char* qweather_api;

// update
extern const char* update_ad;	// 更新广播地址

// LCD
#define USE_HORIZONTAL 1  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏