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
#include "http_json.h"	// http请求JSON


TaskHandle_t th_p[6];	// RTOS多任务 （用于存储传回一个句柄，通过该句柄可以引用所创建的任务
JsonDocument weather_json;	// 天气json
float version_web = 0.0;	// 网络版本号
String version_url_web = "";	// 网络版本链接
bool update_available = false;	// 是否有更新
uint64_t develop_time = 1736666185;	// 开发时间
bool synced_time = false;	// 已同步时间？
APPData *appdata = nullptr;	// 数据实例
bool enable_ble = false;	// 是否启用BLE
bool ble_executed = false;	// BLE是否执行过一次

void setup()
{
	// 串口
	Serial.begin(115200);
	// nvs
	nvs_inits();

	// LCD初始化
	LCD_Init();
	LCD_Fill(0,0,LCD_W,LCD_H,BLACK);	// 黑屏

	// LED初始化
	led_init();

	// 传感器初始化
	Init_Hx711();
	Get_Maopi();
	sensor_init();

	// Wi-Fi读取
	NvsManager nvs("wlan");
	wifi_ssid = nvs.get_string("wifi_ssid").get();
	wifi_password = nvs.get_string("wifi_password").get();

	// 数据
	appdata = new APPData();
	
	// RTOS
	xTaskCreatePinnedToCore(iot, "init_wifi_ble_mqtt", 3000, NULL, 1, &th_p[0], tskNO_AFFINITY);
	xTaskCreatePinnedToCore(sensor, "read_sensor", 2200, NULL, 6, &th_p[1], tskNO_AFFINITY);
	xTaskCreatePinnedToCore(push_message, "push_message", 2000, NULL, 2, &th_p[2], tskNO_AFFINITY);
	xTaskCreatePinnedToCore(user_interface, "user_interface", 8192, NULL, 5, &th_p[3], tskNO_AFFINITY);
	xTaskCreatePinnedToCore(data_manager, "data_manager", 2000, NULL, 3, &th_p[4], tskNO_AFFINITY);
	xTaskCreatePinnedToCore(main_func, "main_func", 8192, NULL, 4, &th_p[5], tskNO_AFFINITY);

	// Weather
	weather();
}

void loop()
{

}

void do_once_what()
{
	if (rx["ota"])
	{
		vTaskSuspend(th_p[1]);
		vTaskSuspend(th_p[2]);
		vTaskSuspend(th_p[4]);
		vTaskSuspend(th_p[5]);

		ota_update(rx["ota"]);
	}
}

void ble_do_once_what()
{
	if(resStr == "getid"){
		pTxCharacteristic->setValue(chipId.c_str());
		pTxCharacteristic->notify();
	}
	// 蓝牙设置Wi-Fi
	if (resStr.indexOf("ssid:") != -1 && resStr.indexOf("password:") != -1)
	{
		uint8_t ssidStart = resStr.indexOf("ssid:") + 5;
		uint8_t ssidEnd = 	resStr.indexOf("|\\|/|", ssidStart);
		String ssid = resStr.substring(ssidStart, ssidEnd);

		uint8_t passwordStart = resStr.indexOf("password:") + 9;
		String password = resStr.substring(passwordStart);
		wifi_ssid = ssid;
		wifi_password = password;
		NvsManager nvs("wlan");
		nvs.set_string("wifi_ssid", wifi_ssid.c_str());
		nvs.set_string("wifi_password", wifi_password.c_str());

	}
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
		ble_reconnect();
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
		
		vTaskDelay(5000 / portTICK_PERIOD_MS);
	}
	
}

// 50ms读取一次传感器和按钮状态
void sensor(void *args)
{
	while (1)
	{
		appdata->hx711_weight = Get_Weight();
		appdata->sr501 = read_sr501();
		appdata->light_ao = read_light_ao();
		appdata->light_do = !read_light_do();
		appdata->button = !read_button();

		vTaskDelay(50 / portTICK_PERIOD_MS);
	}
}

void push_message(void *args)
{
	JsonDocument jsonDoc;
	while (1)
	{
		  // 将变量添加到JSON对象中
		jsonDoc["last_drinking_time"] = 		appdata->last_drinking_time;
		jsonDoc["dark_time"] = 					appdata->dark_time;
		jsonDoc["on_chair_time"] = 				appdata->on_chair_time;

		jsonDoc["drink_times"] = 				appdata->drink_times;
		jsonDoc["long_chair_times"] = 			appdata->long_chair_times;

		jsonDoc["last_day_drink_times"] = 		appdata->last_day_drink_times;
		jsonDoc["last_day_dark_time"] = 		appdata->last_day_dark_time;
		jsonDoc["last_day_long_chair_times"] = 	appdata->last_day_long_chair_times;

		jsonDoc["healthy_score"] = 				appdata->healthy_score;

		jsonDoc["hx711_weight"] = 				appdata->hx711_weight;
		jsonDoc["sr501"] = 						appdata->on_chair;
		jsonDoc["light_do"] = 					appdata->light_do;

		// 将JSON对象转换为字符串
		char jsonBuffer[512];
		serializeJson(jsonDoc, jsonBuffer);

		// 发布JSON字符串到MQTT主题
		if (!client.publish(topic, jsonBuffer)) {
			// Serial.println("Failed to publish message");
		}

		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

void user_interface(void *args)
{
	uint16_t button_tick = 0;
	bool button_flag = false;
	uint16_t stay_tick = 0;
	bool lcd_bk = true;
	uint8_t interface = 1;
	bool ignored = false;
	uint16_t ignored_tick = 0;
	while (1)
	{
		if (stay_tick > 1000) stay_tick = 0;

		// 背光自动关闭逻辑
		if (lcd_bk) LCD_BLK_Set();
		else LCD_BLK_Clr();

		if (appdata->button)
		{
			button_tick++;
			button_flag = true;
			stay_tick = 0;
			lcd_bk = true;
			if (appdata->need_drink || appdata->need_light || appdata->need_walk)
			{
				ignored = true;
			}
		}
		else
		{
			button_tick = 0;
			stay_tick++;
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
			if (stay_tick > 300) lcd_bk = false;
		}
		else if (interface == 3)
		{
			ui_interface_ble();
			if (!ble_executed)
			{
				enable_ble = true;
				ble_executed = true;
			}
		}
		else if (interface == 4)
		{
			ui_interface_wlan();
			if (stay_tick > 300) lcd_bk = false;
		}
		else if (interface == 5)
		{
			ui_interface_info();
			if (stay_tick > 200 && update_available)
			{
				interface = 6;
			}
		}
		else if (interface == 6)
		{
			LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
			// vTaskSuspend(th_p[0]);
			vTaskSuspend(th_p[1]);
			vTaskSuspend(th_p[2]);
			vTaskSuspend(th_p[4]);
			vTaskSuspend(th_p[5]);

			ota_update(version_url_web.c_str());
		}
		if(appdata->need_drink && stay_tick % 30 == 0 && !ignored)
		{
			led_rgb(1);
		}
		else if (appdata->need_light && stay_tick % 30 == 1 && !ignored)
		{
			led_rgb(2);
		}
		else if (appdata->need_walk && stay_tick % 30 == 2 && !ignored)
		{
			led_rgb(4);
		}
		else led_rgb(0,0);
		if (ignored)
		{
			ignored_tick++;
			led_rgb(0,0);
		}
		if (ignored_tick > 6000)
		{
			ignored = false;
			ignored_tick = 0;
		}
		vTaskDelay(100 / portTICK_PERIOD_MS);
	}
}

void data_manager(void *args)
{
	while (1)
	{
		appdata->refresh_day();
		appdata->storage();
		vTaskDelay(60000 / portTICK_PERIOD_MS);
	}
}

void main_func(void *args)
{
	uint16_t weight = 0;
	uint16_t weight_last = 0;
	APPData::cup_status cup_status = APPData::CUP_ON_SCALE;
	uint16_t on_chair_tick = 0;
	TickType_t xLastWakeTime = xTaskGetTickCount();
	BaseType_t xWasDelay;
	uint32_t schedule_time = 0;
	appdata->need_walk = false;
	appdata->need_drink = false;
	appdata->need_light = false;
	while (1)
	{
		for (uint8_t i = 0; i < 5; i++)
		{
			weight += appdata->hx711_weight;
			vTaskDelay(50 / portTICK_PERIOD_MS);
		}
		weight /= 5;
		xTaskDelayUntil(&xLastWakeTime, 1000 / portTICK_PERIOD_MS);
		schedule_time++;

		// drinking
		if (weight - weight_last > 50)
		{
			weight_last = weight;
			if (cup_status == APPData::CUP_PICKED_UP)
			{
				appdata->need_drink = false;
				appdata->drink_times++;
				appdata->last_drinking_time = 0;
			}
			cup_status = APPData::CUP_PUT_DOWN;
		}
		else if (weight_last - weight > 50)
		{
			weight_last = weight;
			cup_status = APPData::CUP_PICKED_UP;
		}
		else
		{
			appdata->last_drinking_time++;
			if (appdata->last_drinking_time > 3600)
			{
				appdata->need_drink = true;
			}
			if (cup_status == APPData::CUP_PUT_DOWN)
			{
				cup_status = APPData::CUP_ON_SCALE;
			}
		}

		// low_light
		if (!appdata->light_do)
		{
			appdata->dark_time++;
			appdata->need_light = true;
		}
		else appdata->need_light = false;

		// human
		on_chair_tick++;
		if (appdata->sr501)
		{
			appdata->on_chair = true;
		}
		if (on_chair_tick > 20 && appdata->on_chair)
		{
			appdata->on_chair_time += 20;
			on_chair_tick = 0;
			appdata->on_chair = false;
			if (appdata->on_chair_time % 3600 == 0)
			{
				appdata->long_chair_times++;
				appdata->need_walk = true;
			}
		}
		else if (on_chair_tick > 20 && !appdata->on_chair)
		{
			appdata->on_chair_time = 0;
			on_chair_tick = 0;
			appdata->on_chair = false;
			appdata->need_walk = false;
		}

		// 定时任务处
		if (schedule_time %600 == 0)
		{
			weather();
		}
		if (schedule_time % 10 == 0)
		{
			JsonDocument doc;
			http_getjson(update_ad, doc);\
			version_web = doc["version"].as<float>();
			version_url_web = doc["url"].as<String>();
			if (version_web > db_version)
			{
				update_available = true;
			}
			else update_available = false;
		}

	}
	v_get_free_stack();
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