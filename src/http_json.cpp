#include <Arduino.h>
#include <ArduinoJson.h>
#include <ArduinoUZlib.h>
#include <HTTPUpdate.h>
#include "main.h"
#include "config.h"

void weather()
{
	String weather_api = "https://devapi.qweather.com/v7/weather/now?location=101280401&lang=en&key=" + String(qweather_api);
	String payload;
	HTTPClient http;
	http.begin(weather_api);
	int httpCode = http.GET();
	Serial.println(httpCode);
	if (httpCode > 0)
	{

		// 读取压缩的 Gzip 数据
		WiFiClient *stream = http.getStreamPtr();
		size_t len = http.getSize();
		uint8_t *compressedData = new uint8_t[len];
		size_t bytesRead = stream->readBytes(compressedData, len);

		// 解压 Gzip 数据
		uint32_t uncompressedLen = 0; // 解压缓冲区大小
		uint8_t *uncompressedData = NULL;
		// uint8_t* uncompressedData = new uint8_t[uncompressedLen];
		int result = ArduinoUZlib::decompress(compressedData, bytesRead, uncompressedData, uncompressedLen);
		if (result != 0)
		{
			// 解压成功，解析 JSON
			Serial.write(uncompressedData, uncompressedLen);
			deserializeJson(weather_json, uncompressedData, uncompressedLen);
			Serial.println(weather_json["now"]["text"].as<String>().c_str());
		}

		// 释放内存
		delete[] compressedData;
		delete[] uncompressedData;
	}
	else
	{
		Serial.println("HTTP请求失败: " + http.errorToString(httpCode));
	}
	http.end();
}

void http_getjson(String url, JsonDocument &json)
{
	HTTPClient http;
	http.begin(url);
	int httpCode = http.GET();
	if (httpCode > 0)
	{
		// 检查HTTP响应码
		if (httpCode == HTTP_CODE_OK)
		{
			// 读取响应数据
			String payload = http.getString();
			// Serial.println("Response payload:");
			// Serial.println(payload);

			// 解析JSON
			DeserializationError error = deserializeJson(json, payload);

			if (error)
			{
				Serial.print("deserializeJson() failed: ");
				Serial.println(error.c_str());
			}
		}
		else
		{
			Serial.println("HTTP请求成功，但响应码不是200: " + String(httpCode));
		}
	}
	else
	{
		Serial.println("HTTP请求失败: " + http.errorToString(httpCode));
	}

	// 结束HTTP请求
	http.end();
}