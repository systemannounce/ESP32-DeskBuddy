#include <Arduino.h>
#include <ArduinoJson.h>
#include <port.h>
#include "main.h"
#include "config.h"
#include "mqtt.h"

JsonDocument rx;
WiFiClient espClient;
PubSubClient client(espClient);

void mqtt_init(PubSubClient &my_client)
{
	my_client.setServer(mqtt_server, 1883);
	my_client.setCallback(callback);

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

void reconnect_mqtt(PubSubClient &my_client)
{
	// Loop until we're reconnected
	while (!my_client.connected())
	{
		Serial.print("Attempting MQTT connection...");
		// Create a random client ID
		String clientId = "ESP32Client-";
		clientId += String(WiFi.macAddress());
		// Attempt to connect
		if (my_client.connect(clientId.c_str(), mqtt_username, mqtt_password))
		{
			Serial.println("connected");
			// Once connected, publish an announcement...
			my_client.publish(topic, "Hello, This is DeskBuddy test.");

			// ... and resubscribe
			my_client.subscribe(topic); /// subscribe
		}
		else
		{
			Serial.print("failed, rc=");
			Serial.print(my_client.state());
			Serial.println(" try again in 5 seconds");
			// Wait 5 seconds before retrying
			delay(5000);
		}
	}
}

void sendjson(const char* send_topic, float payload, PubSubClient &my_client)
{
	char *msgo = (char *)malloc(sizeof(char) * 100);
	assert(msgo);
	// int a=22,b=3,c=52;

	int i = sprintf(msgo, "{\"weight\":%.2f}", payload);
	Serial.printf(msgo);

	// char *json = &msgo[0];

	// Serial.println(json);
	// boolean d = client.publish("ADDRESS","This is a message");
	delay(2000);

	boolean d = my_client.publish(send_topic, msgo, false);
	if (d)
		Serial.println("publish:success");
	else
		Serial.println("publish:eror");
}