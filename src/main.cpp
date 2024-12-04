/***********************

Board:   ESP32-DevKitC
Module:  ESP32-WROOM-32E
Chip:    ESP32-D0WD-V3
Author:  Felix_SANA

*************************/

// 系统库头文件
#include <Arduino.h>
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

#include "mqtt.h"		// mqtt
#include "my_wifi.h"	// wifi
#include "ota.h"		// ota
#include "my_nvs.h"		//nvs
#include "HX711.h"		// HX711
#include "ble.h"		// BLE
#include <time_ntp.h>	// time_ntp


// TaskHandle_t *th_p[2];
float weight = 0;

void setup()
{
	Serial.begin(115200);
	setup_wifi();
	nvs_inits("test");

	client.setServer(mqtt_server, 1883);
	client.setCallback(callback);

	// pinMode(39, INPUT);

	// led_init();
	pinMode(13, INPUT_PULLUP);

	// LCD_Init(); // LCD初始化
	// LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	Init_Hx711();
	delay(3000);

	// HX711电子秤初始化
	Get_Maopi();

	// ble
	ble_init();

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
	delay(1000);
	// Serial.println("GPIO13的当前状态：");
	// Serial.println(digitalRead(13));

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

void Interface1()
{
	
}