#include <Arduino.h>
#include "port.h"


enum color {
	RED = 1,
	GREEN = 2,
	BLUE = 4
};

void led_rgb(int rgb, int deeply = 255)
{
	if (deeply > 255) deeply = deeply % 255;
	if (rgb == 0)
	{
		analogWrite(LED_R, 0);
		analogWrite(LED_G, 0);
		analogWrite(LED_B, 0);
	}
	
	if (rgb & RED) analogWrite(LED_R, deeply); else analogWrite(LED_R, 0);
	if (rgb & GREEN) analogWrite(LED_G, deeply); else analogWrite(LED_G, 0);
	if (rgb & BLUE) analogWrite(LED_B, deeply); else analogWrite(LED_B, 0);
}

void led_init()
{
	pinMode(LED_R, OUTPUT);
	pinMode(LED_G, OUTPUT);
	pinMode(LED_B, OUTPUT);
}