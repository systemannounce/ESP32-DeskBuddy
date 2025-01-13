#include <unity.h>
#include <Arduino.h>
#include "my_nvs.h"


void setUp(void) {
    // set stuff up here
    
}

void tearDown(void) {
    // clean stuff up here
    
}

void test_nvs(void)
{
	nvs_inits();
	NvsManager nvs("test");
	nvs.set_string("test_unit", "unit_success");
	TEST_ASSERT_EQUAL_STRING("unit_success", nvs.get_string("test_unit").get());
}

void setup()
{
	delay(2000);
	UNITY_BEGIN();
	RUN_TEST(test_nvs);
	UNITY_END();
}

void loop()
{

}