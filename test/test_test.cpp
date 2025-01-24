#include <unity.h>


void setUp(void) {
    // set stuff up here
    
}

void tearDown(void) {
    // clean stuff up here
    
}

void test_nvs(void)
{

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