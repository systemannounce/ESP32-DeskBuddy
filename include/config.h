#pragma once
// mqtt
extern const char *mqtt_server;
extern const char *mqtt_username;
extern const char *mqtt_password;
extern const char *topic;

// wifi
extern const char *wifi_ssid;
extern const char *wifi_password;

// NTP
extern const char* ntpServer;
extern const long utcOffsetInSeconds;

// LCD
#define USE_HORIZONTAL 1  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏