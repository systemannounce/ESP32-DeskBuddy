#pragma once
#include <nvs_flash.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <PubSubClient.h>

// 初始化Wi-Fi
void setup_wifi(void);

// 断开Wi-Fi后自动重连到上一个Wi-Fi
void reconnect_wifi(void);

// 接收到消息解析以后要做 一次 什么事情
void do_once_what(void);

// BLE接收到消息解析以后要做一次什么事情
void ble_do_once_what();

// 删除所有nvs的数据[系统API]（慎用！！）
esp_err_t nvs_flash_erase(void);