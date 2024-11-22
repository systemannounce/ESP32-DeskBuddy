#include <Arduino.h>
#include <WiFi.h>
#include <config.h>

void setup_wifi()
{
	delay(10);
	Serial.println();
	Serial.print("Connecting to ");
	Serial.println(wifi_ssid);

	WiFi.mode(WIFI_STA);
	WiFi.begin(wifi_ssid, wifi_password);

	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}

	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
	Serial.print("RRSI: ");
	Serial.println(WiFi.RSSI());
}

void reconnect_wifi()
{
	Serial.println("Reconnecting to WiFi...");
	WiFi.disconnect();
	WiFi.reconnect();
}