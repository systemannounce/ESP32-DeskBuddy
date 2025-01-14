#include <Arduino.h>
#include "port.h"

void sensor_init()
{
	pinMode(LDO, INPUT);
	pinMode(BUTTON, INPUT_PULLUP);
}

uint16_t read_sr501()
{
	return analogRead(SR501);
}

uint16_t read_light_ao()
{
	return analogRead(LAO);
}

bool read_light_do()
{
	return digitalRead(LDO);
}

bool read_button()
{
	return digitalRead(BUTTON);
}