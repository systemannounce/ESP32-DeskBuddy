#include <Arduino.h>
#include <ArduinoJson.h>
#include <port.h>
#include "main.h"
#include "config.h"
#include "mqtt.h"

JsonDocument rx;
WiFiClient espClient;
PubSubClient client(espClient);

void mqtt_init()
{
	client.setServer(mqtt_server, 1883);
	client.setCallback(callback);
	client.setBufferSize(512);


}

void deserialize_json(String &receive, JsonDocument &json_document)
{
	DeserializationError error = deserializeJson(json_document, receive);
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

	deserialize_json(str, rx);
	do_once_what();
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
			client.publish(send_topic, "Hello, This is DeskBuddy.");

			// ... and resubscribe
			client.subscribe(receive_topic); /// subscribe
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

void sendjson(const char* topic, const JsonDocument &jsondoc)
{
	// 将JSON对象转换为字符串
	char jsonBuffer[512];
	serializeJson(jsondoc, jsonBuffer);

	// 发布JSON字符串到MQTT主题
	if (!client.publish(topic, jsonBuffer)) {
		Serial.println("Failed to publish message");
	}
}