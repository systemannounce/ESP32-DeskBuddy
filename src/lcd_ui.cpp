#include "lcd.h"
#include "lcd_init.h"
#include "time_ntp.h"
#include "Arduino.h"
#include "main.h"
#include "ble.h"


const char* time_test;
String weather_string;
bool wlan_button = false;
bool ble_button = false;

/*******************************
 HOME

 显示时间，天气，BLE连接，WLAN连接，MQTT服务器的信息
 显示当前的警告提醒

 *******************************/
void ui_interface_home()
{
	LCD_DrawLine(100, 0, 100, 130, WHITE);

	time_test = time_get_time_local("%H:%M:%S");
	Serial.println(time_test);
	LCD_ShowString(0, 0 , time_test, WHITE, BLACK, 12, 0);
}

// 息屏显示 ✔
void ui_interface_close()
{
	// time
	time_test = time_get_time_local("%H:%M");
	LCD_ShowString(35, 50, time_test, WHITE, BLACK, 24, 0);
	
	//weather
	if (weather_json.isNull()) weather_string = "NO DATA";
	else weather_string = weather_json["now"]["text"].as<String>(); 
	LCD_ShowString(40, 85, weather_string.c_str(), WHITE, BLACK, 12, 0);
}

// BLUETOOTH CONFIG (disable in wlan connected)
void ui_interface_ble()
{
	// TIME
	time_test = time_get_time_local("%H:%M:%S");
	LCD_ShowString(0, 0, time_test, WHITE, BLACK, 12, 0);

	// TITLE
	LCD_ShowString(30, 40, "Bluetooth", WHITE, BLACK, 16, 0);
	LCD_ShowString(50, 65, "Config", WHITE, BLACK, 12, 0);

	// BLE
	LCD_ShowString(15, LCD_H - 10, macAddress.c_str(), WHITE, BLACK, 12, 0);
	if (deviceConnected)
	{
		if(ble_button != true) LCD_Fill(15,105,LCD_W,LCD_H,BLACK);
		ble_button = true;
		LCD_ShowString(30, 105, "Connected", BLACK, WHITE, 16, 0);
	}
	else
	{
		if(ble_button != false) LCD_Fill(15,105,LCD_W,LCD_H,BLACK);
		ble_button = false;
		LCD_ShowString(15, 105, "Disconnected", BLACK, WHITE, 16, 0);
	}
}

// WLAN
void ui_interface_wlan()
{
	// TIME
	time_test = time_get_time_local("%H:%M:%S");
	LCD_ShowString(0, 0, time_test, WHITE, BLACK, 12, 0);

	// TITLE
	LCD_ShowString(55, 40, "WLAN", WHITE, BLACK, 16, 0);
	LCD_ShowString(50, 65, "Config", WHITE, BLACK, 12, 0);

	// WLAN
	LCD_ShowString(15, 100, wifi_ssid, WHITE, BLACK, 12, 0);
	if (WiFi.status() == WL_CONNECTED)
	{
		if(wlan_button != true) LCD_Fill(15,130,LCD_W,LCD_H,BLACK);
		wlan_button = true;
		LCD_ShowString(30, 130, "Connected", BLACK, WHITE, 16, 0);
	}
	else
	{
		if(wlan_button != false) LCD_Fill(15,130,LCD_W,LCD_H,BLACK);
		wlan_button = false;
		LCD_ShowString(15, 130, "Disconnected", BLACK, WHITE, 16, 0);
	}

}

// INFO
void ui_interface_info()
{

}

// OTA
void ui_interface_ota()
{

}