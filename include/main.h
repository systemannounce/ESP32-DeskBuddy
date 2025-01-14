#pragma once
#include <nvs_flash.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "data.h"

extern TaskHandle_t th_p[6];
extern JsonDocument weather_json;

extern float version_web;
extern String version_url_web;
extern bool update_available;

extern uint64_t develop_time;
extern bool synced_time;
extern APPData *appdata;


// 接收到消息解析以后要做 一次 什么事情
void do_once_what(void);

// BLE接收到消息解析以后要做一次什么事情
void ble_do_once_what();

// 天气请求
void weather();

// RTOS
void iot(void *args);

void sensor(void *args);

void push_message(void *args);

void user_interface(void *args);

void data_manager(void *args);

void main_func(void *args);

void v_get_free_stack();



// 删除所有nvs的数据[系统API]<初始化以前使用>（慎用！！）
// esp_err_t nvs_flash_erase(void);