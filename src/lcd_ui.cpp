#include "lcd.h"
#include "lcd_init.h"
#include "time_ntp.h"
#include "Arduino.h"
#include "main.h"
#include "ble.h"
#include "port.h"
#include "data.h"
#include "main.h"


const char* time_test;
String weather_string;
bool wlan_button = false;
bool ble_button = false;

/*******************************
 HOME ✔

 显示时间，天气，BLE连接，WLAN连接，MQTT服务器的信息
 显示当前的警告提醒

 *******************************/
void ui_interface_home()
{
	// time
	time_test = time_get_time_local("%H:%M:%S");
	LCD_ShowString(2, 0, time_test, WHITE, BLACK, 12, 0);
	LCD_DrawRectangle(0, 0, 50, 11, WHITE);

	// weather
	if (weather_json.isNull()) weather_string = "NO DATA";
	else weather_string = weather_json["now"]["text"].as<String>(); 
	LCD_ShowString(60, 0, weather_string.c_str(), WHITE, BLACK, 12, 0);

	LCD_DrawLine(0, 11, LCD_W, 11, WHITE);

	// today
	LCD_ShowString(0, 20, "Last Drinking:", WHITE, BLACK, 12, 0);
	LCD_ShowString(90, 20, unix_to_str(appdata->last_drinking_time), WHITE, BLACK, 12, 0);
	LCD_ShowString(0, 40, "Low Light time:", WHITE, BLACK, 12, 0);
	LCD_ShowString(90, 40, unix_to_str(appdata->dark_time), WHITE, BLACK, 12, 0);
	LCD_ShowString(0, 60, "Still on chair:", WHITE, BLACK, 12, 0);
	LCD_ShowString(90, 60, unix_to_str(appdata->on_chair_time), WHITE, BLACK, 12, 0);

	LCD_DrawLine(0, 75, LCD_W, 75, WHITE);

	// lastday
	LCD_ShowString(0, 80, "LD Drinks:", WHITE, BLACK, 12, 0);
	LCD_ShowString(90, 80, String(appdata->last_day_drink_times).c_str(), WHITE, BLACK, 12, 0);
	LCD_ShowString(0, 100, "LD DarkHrs:", WHITE, BLACK, 12, 0);
	LCD_ShowString(90, 100, unix_to_str(appdata->last_day_dark_time), WHITE, BLACK, 12, 0);
	LCD_ShowString(0, 120, "LD OnChairs:", WHITE, BLACK, 12, 0);
	LCD_ShowString(90, 120, String(appdata->last_day_long_chair_times).c_str(), WHITE, BLACK, 12, 0);
	LCD_ShowString(0, 140, "LD HScore:", WHITE, BLACK, 16, 0);
	LCD_DrawLine(0, 158, 80, 158, WHITE);

	LCD_ShowString(90, 140, String(appdata->healthy_score).c_str(), WHITE, BLACK, 16, 0);

	LCD_DrawRectangle(85, 140, 120, 155, WHITE);

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

// BLUETOOTH CONFIG (disable in wlan connected) ✔
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

// WLAN ✔
void ui_interface_wlan()
{
	// TIME
	time_test = time_get_time_local("%H:%M:%S");
	LCD_ShowString(0, 0, time_test, WHITE, BLACK, 12, 0);

	// TITLE
	LCD_ShowString(55, 40, "WLAN", WHITE, BLACK, 16, 0);
	LCD_ShowString(50, 65, "Config", WHITE, BLACK, 12, 0);

	// WLAN
	LCD_ShowString(15, 100, wifi_ssid.c_str(), WHITE, BLACK, 12, 0);
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

// INFO ✔
void ui_interface_info()
{
	// TIME
	time_test = time_get_time_local("%H:%M:%S");
	LCD_ShowString(0, 0, time_test, WHITE, BLACK, 12, 0);

	// TITLE
	LCD_ShowString(40, 30, "INFO", WHITE, BLACK, 24, 0);

	// VERSION
	LCD_ShowString(45, 70, "Version", WHITE, BLACK, 12, 0);
	LCD_ShowString(55, 90, String(db_version).c_str(), BLACK, WHITE, 16, 0);

	if (update_available)
	{
		// TIPS
		LCD_ShowString(0, 130, "If U need update,", WHITE, BLACK, 12, 0);
		LCD_ShowString(0, 145, "Stay here 20s.", WHITE, BLACK, 12, 0);
	}
}

// OTA ✔
void ui_interface_ota(const char* title = "Error", const char* code = "Please check.", const char* process = "")
{
	vTaskDelay(5 / portTICK_PERIOD_MS);

	// TIME
	time_test = time_get_time_local("%H:%M:%S");
	LCD_ShowString(0, 0, time_test, WHITE, BLACK, 12, 0);

	// TITLE
	LCD_ShowString(25, 30, title, BLACK, WHITE, 24, 0);
	LCD_ShowString(20, 75, code, BLACK, WHITE, 16, 0);
	LCD_ShowString(45, 120, process, BLACK, WHITE, 16, 0);
}
