#include <Arduino.h>
#include <WiFi.h>
#include <config.h>

bool setup_wifi(uint32_t timeout_ms)
{
    uint32_t start_time_ms = millis(); // 记录开始时间
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(wifi_ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(wifi_ssid, wifi_password);

    while (WiFi.status() != WL_CONNECTED)
    {
        if (millis() - start_time_ms >= timeout_ms)
        {
            Serial.println("\nWiFi connection timeout!");
            return false; // 超时未连接，返回失败
        }
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("RRSI: ");
    Serial.println(WiFi.RSSI());
    return true; // 连接成功
}

void reconnect_wifi()
{
	Serial.println("Reconnecting to WiFi...");
	WiFi.disconnect();
	WiFi.reconnect();
}