#include <Arduino.h>
#include "port.h"

void sensor_init()
{
	pinMode(39, INPUT);
}

uint16_t read_sr501()
{
	return analogRead(SR501);
}

uint16_t read_light_ao()
{
	return analogRead(AO);
}

bool read_light_do()
{
	return digitalRead(DO);
}

bool read_button()
{
	return digitalRead(BUTTON);
}