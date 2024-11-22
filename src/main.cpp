/***********************

Board:   ESP32-DevKitC
Module:  ESP32-WROOM-32E
Chip:    ESP32-D0WD-V3
Author:  Felix_SANA

*************************/

// 系统库头文件
#include <Arduino.h>
#include <sstream>
#include <string>

// 一些配置和定义头文件
#include "config.h"
#include "port.h"
#include "main.h"

// 三色LED头文件
#include "led.h"

// LCD头文件
#include "lcd_init.h"
#include "lcd.h"
#include "pic.h"

// mqtt
#include "mqtt.h"

//wifi
#include "my_wifi.h"

//ota
#include "ota.h"

//nvs
#include "my_nvs.h"

// HX711
#include "HX711.h"


// TaskHandle_t *th_p[2];
float weight = 0;

void setup()
{
	Serial.begin(115200);
	setup_wifi();
	nvs_inits("test");

	client.setServer(mqtt_server, 1883);
	client.setCallback(callback);

	pinMode(39, INPUT);

	// led_init();


	// configTime(utcOffsetInSeconds, 0, ntpServer);
	// while (!time(nullptr)) {
    // delay(1000);
    // Serial.println("Waiting for time sync...");
	// }
	// Serial.println("Time synced successfully");


	// LCD_Init(); // LCD初始化
	// LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	Init_Hx711();
	delay(3000);
	Get_Maopi();

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
	LCD_BLK_Clr();
	// Serial.println("Hello World!");
	// time_t now = time(nullptr);
	// Serial.print("Current time is: ");
	// Serial.println(ctime(&now));  //打印时间
	// // Convert current time to Unix timestamp
	// long unixTimestamp = static_cast<long>(now);  //获取unix时间戳 
	// Serial.print("Unix timestamp is: ");
	// Serial.println(unixTimestamp);
	delay(1000);

	// weight = Get_Weight();
	// // weight = weight/1000;
	// Serial.println(weight);
	// std::stringstream ss;
	// ss << weight;
	// std::string str = ss.str();
	// const char* out = str.c_str();

	// sendjson("test", weight, client);
	// Serial.print("AO:");
	// Serial.println(analogRead(36));
	// Serial.print("DO:");
	// Serial.println(digitalRead(39));
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