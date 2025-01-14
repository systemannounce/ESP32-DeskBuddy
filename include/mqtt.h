#pragma once
#include <Arduino.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

extern JsonDocument rx;
extern PubSubClient client;

// 用config.cpp里面的服务器地址初始化
void mqtt_init();

// 将字符串解析到JsonDocument rx
void deserialize_json(String &receive, JsonDocument &json_document);

// 发送JSON数据到mqtt服务器
void sendjson(const char* send_topic, float payload, PubSubClient &my_client);

// 重新连接到mqtt服务器
void reconnect_mqtt(PubSubClient &my_client);

// Callback函数，接收消息后要做的事情
void callback(char *topic, byte *payload, unsigned int length);