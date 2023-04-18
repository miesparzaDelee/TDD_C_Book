#include "LedDriver.h"

static uint16_t* LedAddress;

void LedDriver_Create(uint16_t* address)
{
	LedAddress = address;
	*LedAddress = 0;
}

void LedDriver_Destroy(void)
{

}

void LedDriver_TurnOn(int ledNumber)
{
	*LedAddress = 1;
}

void LedDriver_TurnOff(int ledNumber)
{
	*LedAddress = 0;
}