#pragma once
#include <Arduino.h>

// 传感器初始化
void sensor_init();

// 读取HC-SR501的值
uint16_t read_sr501();

// 读取光敏电阻模拟值
uint16_t read_light_ao();

// 读取光敏电阻数字量（0/1）
bool read_light_do();

// 读取按键状态
bool read_button();