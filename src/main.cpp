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
#include "data.h"		// 数据管理


TaskHandle_t th_p[5];	// RTOS多任务 （用于存储传回一个句柄，通过该句柄可以引用所创建的任务
JsonDocument weather_json;	// 天气json
uint64_t develop_time = 1736666185;	// 开发时间
bool synced_time = false;	// 已同步时间？
APPData *appdata = nullptr;	// 数据实例
bool enable_ble = false;	// 是否启用BLE

void setup()
{
	// 串口
	Serial.begin(115200);
	// nvs
	nvs_inits();

	// 按钮
	pinMode(BUTTON, INPUT_PULLUP);

	// LCD初始化
	LCD_Init();
	LCD_Fill(0,0,LCD_W,LCD_H,BLACK);	// 黑屏

	// HX711电子秤初始化
	Init_Hx711();
	Get_Maopi();

	// 数据
	appdata = new APPData();
	Serial.println("APPData init");
	
	// nvs
	// NvsManager nvs("test");
	// uint8_t test[4] = {1, 2, 3, 4};
	// nvs.set_array("array", test, sizeof(test));
	// auto test_get = nvs.get_array<uint8_t>("array");
	// Serial.println(test_get[0]);
	// Serial.println(nvs.get_string("233").get());

	// nvs_get_string("test");
	xTaskCreatePinnedToCore(iot, "init_wifi_ble_mqtt", 2500, NULL, 1, &th_p[0], tskNO_AFFINITY);
	xTaskCreatePinnedToCore(sensor, "read_sensor", 2200, NULL, 3, &th_p[1], tskNO_AFFINITY);
	xTaskCreatePinnedToCore(push_message, "push_message", 2000, NULL, 4, &th_p[2], tskNO_AFFINITY);
	xTaskCreatePinnedToCore(user_interface, "user_interface", 2200, NULL, 2, &th_p[3], tskNO_AFFINITY);
	xTaskCreatePinnedToCore(data_manager, "data_manager", 2000, NULL, 5, &th_p[4], tskNO_AFFINITY);

	// Weather
	// weather();
}

void loop()
{
	// if (WiFi.status() != WL_CONNECTED)
	// {
	// 	reconnect_wifi();
	// }

	// if (!client.connected())
	// {
	// 	reconnect_mqtt(client);
	// }
	// client.loop();

	//防止过快卡死
	delay(1000);

	// loop测试区
	{
		// if (!digitalRead(BUTTON))
		// {
		// 	vTaskDelete(th_p[0]);
		// }
	}
	// ble_reconnect();
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

// RTOS
void iot(void *args)
{
	while (1)
	{
		// ble
		if (enable_ble && !deviceConnected && !oldDeviceConnected)
		{
			ble_init();
			enable_ble = false;
		}
		// ble_init();
		if (WiFi.status() != WL_CONNECTED)
		{
			// Wi-Fi
			setup_wifi(10000);
		}
		// mqtt
		client.setServer(mqtt_server, 1883);
		client.setCallback(callback);
		if (WiFi.status() == WL_CONNECTED && time(nullptr) < develop_time && !synced_time)
		{
			// time_ntp
			synced_time = time_ntp_init(10000);
		}
		if (WiFi.status() != WL_CONNECTED)
		{
			reconnect_wifi();
		}

		if (!client.connected() && WiFi.status() == WL_CONNECTED)
		{
			reconnect_mqtt(client);
		}
		client.loop();

		v_get_free_stack();
		
		vTaskDelay(5000 / portTICK_PERIOD_MS);
	}
	
}

// 100ms读取一次传感器和按钮状态
void sensor(void *args)
{
	while (1)
	{
		// HX711
		appdata->hx711_weight = Get_Weight();
		appdata->sr501 = read_sr501();
		appdata->light_ao = read_light_ao();
		appdata->light_do = read_light_do();
		appdata->button = !read_button();

		// v_get_free_stack();
		vTaskDelay(100 / portTICK_PERIOD_MS);
	}
}

void push_message(void *args)
{
	while (1)
	{
		// mqtt
		// client.loop();
		// client.publish(topic, "hello world");
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

void user_interface(void *args)
{
	uint16_t button_tick = 0;
	bool button_flag = false;
	uint8_t interface = 1;
	while (1)
	{
		if (appdata->button)
		{
			button_tick++;
			button_flag = true;
		}
		else
		{
			button_tick = 0;
			if (button_flag)
			{
				button_flag = false;
				LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
				interface = (interface % 5) + 1;
				if (interface ==3 && WiFi.status() == WL_CONNECTED)
				{
					interface++;
				}
			}
		}
		if (interface == 1)
		{
			ui_interface_close();
		}
		else if (interface == 2)
		{
			ui_interface_home();
		}
		else if (interface == 3)
		{
			ui_interface_ble();
			enable_ble = true;
		}
		else if (interface == 4)
		{
			ui_interface_wlan();
		}
		else if (interface == 5)
		{
			ui_interface_info();
		}
		// v_get_free_stack();
		vTaskDelay(200 / portTICK_PERIOD_MS);
	}
}

void data_manager(void *args)
{
	while (1)
	{
		
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    Serial.printf("Stack overflow in task: %s\n", pcTaskName);
    while (1); // 死循环，便于调试
}

void v_get_free_stack()
{
	// 获取堆栈剩余空间
	UBaseType_t highWaterMark = uxTaskGetStackHighWaterMark(NULL);
	printf("Stack high water mark: %u words (%u bytes)\n", highWaterMark, highWaterMark * sizeof(StackType_t));
}