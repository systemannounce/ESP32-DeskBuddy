#include <WiFi.h>
#include <HTTPUpdate.h>
#include "config.h"
#include "lcd_ui.h"
#include "lcd.h"
#include "lcd_init.h"
#include "main.h"
#include <cmath>


double roundToDecimal(double value, int decimalPlaces) {
    double factor = std::pow(10, decimalPlaces);
    return std::round(value * factor) / factor;
}

void ota_update_started()
{
	Serial.println("HTTP update process started.");
	ui_interface_ota("Update", "Please wait");
}

void ota_update_finished()
{
	LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
	Serial.println("HTTP update process finished.");
	ui_interface_ota("Update", "Finished");
}

void ota_update_progress(int cur, int total)
{
    Serial.printf("HTTP update process at %d of %d bytes[%.1f%%]...\n", cur, total, cur * 100.0 / total);
	double process = roundToDecimal(cur * 100.0 / total, 2);
	ui_interface_ota("Update", "Please wait", String(process).c_str());
}

void ota_update_error(int err)
{
	LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
	Serial.printf("HTTP update fatal errr code %d\n", err);
	ui_interface_ota("Update", "Error", String(err).c_str());
}

t_httpUpdate_return updateBin(const char *update_url)
{
    Serial.println("start update");
    WiFiClient UpdateClient;

    httpUpdate.onStart(ota_update_started);     //当升级开始时
    httpUpdate.onEnd(ota_update_finished);      //当升级结束时
    httpUpdate.onProgress(ota_update_progress); //当升级中
    httpUpdate.onError(ota_update_error);       //当升级失败时

    t_httpUpdate_return ret = httpUpdate.update(UpdateClient, update_url);

    return ret;
}

void ota_update(const char* update_link)
{
	t_httpUpdate_return ret = updateBin(update_link); //开始升级
	Serial.println(ret);
    switch (ret)
	{
		case HTTP_UPDATE_FAILED: //当升级失败
			Serial.println("[update] Update failed.");
			break;
		case HTTP_UPDATE_NO_UPDATES: //当无升级
			Serial.println("[update] Update no Update.");
			break;
		case HTTP_UPDATE_OK: //当升级成功
			Serial.println("[update] Update ok.");
			break;
	}
}