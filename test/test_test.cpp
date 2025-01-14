#include <unity.h>
#include <Arduino.h>
#include "http_json.h"
#include "my_wifi.h"


void setUp(void) {
    // set stuff up here
    
}

void tearDown(void) {
    // clean stuff up here
    
}

void test_nvs(void)
{
	JsonDocument doc;
	http_getjson("https://www.systemannounce.cn/download/esp.txt", doc);
	TEST_ASSERT_EQUAL_STRING((const char*)doc["Version"], "0.3");
	TEST_ASSERT_EQUAL_STRING((const char*)doc["link"], "https://www.systemannounce.cn/download/firmware.bin");
}

void setup()
{
	delay(2000);
	setup_wifi(100000);
	UNITY_BEGIN();
	RUN_TEST(test_nvs);
	UNITY_END();
}

void loop()
{

}