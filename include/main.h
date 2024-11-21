#pragma once
#include <nvs_flash.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <PubSubClient.h>


JsonDocument rx;

WiFiClient espClient;
PubSubClient client(espClient);


TaskHandle_t *th_p[2];

float t=0;

// 初始化Wi-Fi
void setup_wifi(void);

// 断开Wi-Fi后自动重连到上一个Wi-Fi
void reconnect_wifi(void);

// 重新连接到mqtt服务器
void reconnect_mqtt(void);

// Callback函数，接收消息后要做的事情
void callback(char *topic, byte *payload, unsigned int length);

// 发送JSON数据到mqtt服务器
void sendjson(void);

// 将字符串解析到JsonDocument rx
void deserialize_json(String receive);

// 接收到消息解析以后要做 一次 什么事情
void do_once_what(void);

// nvs初始化以及打开命名表格
void nvs_inits(const char* list);

// 删除所有nvs的数据[系统API]（慎用！！）
esp_err_t nvs_flash_erase(void);