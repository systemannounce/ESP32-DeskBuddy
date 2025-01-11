/***********************

Board:   ESP32-DevKitC
Module:  ESP32-WROOM-32E
Chip:    ESP32-D0WD-V3
Author:  Felix_SANA

*************************/

// 系统库头文件
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ArduinoUZlib.h> // Gzip 解压缩库
// #include <sstream>
// #include <string>

// 一些配置和定义头文件
#include "config.h"
#include "port.h"
#include "main.h"

#include "led.h"	   // 三色LED头文件

// LCD头文件
#include "lcd_init.h"
#include "lcd.h"
#include "pic.h"
#include "lcd_ui.h"

#include "mqtt.h"		// mqtt
#include "my_wifi.h"	// wifi
#include "ota.h"		// ota
#include "my_nvs.h"		//nvs
#include "HX711.h"		// HX711
#include "ble.h"		// BLE
#include "time_ntp.h"	// time_ntp
#include "sensor.h"		// 传感器SR501 光敏电阻


TaskHandle_t th_p[2];	// RTOS多任务 （用于存储传回一个句柄，通过该句柄可以引用所创建的任务
JsonDocument weather_json;
float weight = 0;
long long i;


void setup()
{
	// 串口
	Serial.begin(115200);
	// Wi-Fi
	setup_wifi();
	// nvs
	nvs_inits("test");

	// mqtt
	client.setServer(mqtt_server, 1883);
	client.setCallback(callback);

	// time_ntp
	time_ntp_init();

	// 按钮
	pinMode(BUTTON, INPUT_PULLUP);

	// LCD初始化
	LCD_Init();
	LCD_Fill(0,0,LCD_W,LCD_H,BLACK);	// 黑屏

	// HX711电子秤初始化
	// Init_Hx711();
	// Get_Maopi();

	// ble
	ble_init();
	
	// temp
	// nvs_get_string("test");
	xTaskCreatePinnedToCore(reflesh_1s, "1S", 4096, NULL, 1, &th_p[0], tskNO_AFFINITY);
	xTaskCreatePinnedToCore(reflesh_100ms, "100MS", 4096, NULL, 3, &th_p[1], tskNO_AFFINITY);

	// Weather
	// weather();
}

void loop()
{
	if (WiFi.status() != WL_CONNECTED)
	{
		reconnect_wifi();
	}

	if (!client.connected())
	{
		reconnect_mqtt(client);
	}
	client.loop();

	//防止过快卡死
	delay(1000);

	// loop测试区
	{

		// if (!digitalRead(BUTTON))
		// {
		// 	vTaskDelete(th_p[0]);
		// }
	}
	ble_reconnect();
}



void do_once_what()
{
	led_rgb(int(rx["color"]), int(rx["deeply"]));
	if (int(rx["bk"]) == 1)
	{
		LCD_BLK_Set();
		Serial.print("打开背光");
	}
	else
	{
		LCD_BLK_Clr();
		Serial.print("关闭背光");
	}
	if (rx["ota"])
	{
		ota_update(rx["ota"]);
	}
	delay(1000);
}

void ble_do_once_what()
{
	if(resStr == "getid"){
		pTxCharacteristic->setValue(chipId.c_str());
		pTxCharacteristic->notify();
	}
}

void weather()
{
	String weather_api = "https://devapi.qweather.com/v7/weather/now?location=101280401&lang=en&key=" + String(qweather_api);
	String payload;
	HTTPClient http;
	http.begin(weather_api);
	int httpCode = http.GET();
	Serial.println(httpCode);
	if (httpCode > 0)
	{
		
		// 读取压缩的 Gzip 数据
		WiFiClient* stream = http.getStreamPtr();
		size_t len = http.getSize();
		uint8_t* compressedData = new uint8_t[len];
		size_t bytesRead = stream->readBytes(compressedData, len);

		// 解压 Gzip 数据
		uint32_t uncompressedLen = 0; // 解压缓冲区大小
		uint8_t *uncompressedData = NULL;
		// uint8_t* uncompressedData = new uint8_t[uncompressedLen];
		int result = ArduinoUZlib::decompress(compressedData, bytesRead, uncompressedData, uncompressedLen);
		if (result != 0) {
		// 解压成功，解析 JSON
		Serial.write(uncompressedData, uncompressedLen);
		deserializeJson(weather_json, uncompressedData, uncompressedLen);
		Serial.println(weather_json["now"]["text"].as<String>().c_str());
		}

		// 释放内存
		delete[] compressedData;
		delete[] uncompressedData;
	} else {
		Serial.println("HTTP请求失败: " + http.errorToString(httpCode));
	}
	http.end();
}

// RTOS示范
void vtask_sinple(void *args)
{

}

void reflesh_1s(void *args)
{
	while (1)
	{
		ui_interface_ble();
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
	
}

void reflesh_100ms(void *args)
{
	while (1)
	{
		// TIME
		// const char* time_test = time_get_time_local("%H:%M:%S");
		// Serial.println(time_test);
		vTaskDelay(100 / portTICK_PERIOD_MS);
	}
}
	
