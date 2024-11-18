/***********************

Board:   ESP32-DevKitC
Module:  ESP32-WROOM-32E
Chip:    ESP32-D0WD-V3
Author:  Felix_SANA

*************************/

#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPUpdate.h>
#include <PubSubClient.h>
#include <nvs_flash.h>
#include "main.h"


void setup()
{
	Serial.begin(115200);
	setup_wifi();
	nvs_inits("smartorb");
	client.setServer(mqtt_server, 1883);
	client.setCallback(callback);
}

void loop()
{
	if (WiFi.status() != WL_CONNECTED)
	{
		reconnect_wifi();
	}

	if (!client.connected())
	{
		reconnect_mqtt();
	}
	client.loop();
	Serial.println("Hello World!");
	delay(1000);
}

void nvs_inits(const char* list)
{
	esp_err_t error = nvs_flash_init();
	if (error == ESP_OK)
	{
		Serial.println("NVS已经成功初始化!");
		esp_err_t error_open = nvs_open(list, NVS_READWRITE, my_nvs_handle);
		if (error_open == ESP_OK)
		{
			Serial.println("NVS已经成功打开命名空间!");
			if (my_nvs_handle)
			{
				Serial.println("句柄返回成功");
			}
			else
			{
				Serial.println("句柄返回失败，未知原因。");
			}
		}
		else
		{
			Serial.println("NVS打开命名空间失败!!");
		}
	}
	else
	{
		Serial.println(error);
		Serial.println("NVS初始化失败!!");
	}
}

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
void deserialize_json(String receive)
{
	DeserializationError error = deserializeJson(rx, receive);
	if (error)
	{
		Serial.print(F("deserializeJson() failed: "));
		Serial.println(error.f_str());
		return;
	}
}

void callback(char *topic, byte *payload, unsigned int length)
{
	Serial.print("Message arrived [");
	Serial.print(topic);
	Serial.print("] \r\n");
	String str;
	for (int i = 0; i < length; i++)
	{
		str += String((char)payload[i]);
		// Serial.print((char)payload[i]);
	}
	Serial.println();
	Serial.print(str);

	deserialize_json(str);
	do_once_what();
}

void do_once_what()
{
	delay(1000);
}

void reconnect_mqtt()
{
	// Loop until we're reconnected
	while (!client.connected())
	{
		Serial.print("Attempting MQTT connection...");
		// Create a random client ID
		String clientId = "ESP32Client-";
		clientId += String(WiFi.macAddress());
		// Attempt to connect
		if (client.connect(clientId.c_str(), mqtt_username, mqtt_password))
		{
			Serial.println("connected");
			// Once connected, publish an announcement...
			client.publish(topic, "Hello, This is DeskBuddy test.");

			// ... and resubscribe
			client.subscribe(topic); /// subscribe
		}
		else
		{
			Serial.print("failed, rc=");
			Serial.print(client.state());
			Serial.println(" try again in 5 seconds");
			// Wait 5 seconds before retrying
			delay(5000);
		}
	}
}

void sendjson()
{
	char *msgo = (char *)malloc(sizeof(char) * 100);
	assert(msgo);
	// int a=22,b=3,c=52;

	int i = sprintf(msgo, "{\"weight\":%d}", 450 / 450);
	Serial.printf(msgo);

	// char *json = &msgo[0];

	// Serial.println(json);
	// boolean d = client.publish("ADDRESS","This is a message");
	delay(2000);

	boolean d = client.publish(topic, msgo, false);
	if (d)
		Serial.println("publish:success");
	else
		Serial.println("publish:eror");
}
