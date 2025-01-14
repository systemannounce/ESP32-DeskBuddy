#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>

void weather();

void http_getjson(String url, JsonDocument &json);