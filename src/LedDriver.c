#include "LedDriver.h"

static uint16_t* LedAddress;
static uint16_t ledsImage;

static uint16_t convertLedNumberToBit(int ledNumber);
static void updateLedStatus();
static bool outOfBounds(int ledNumber);

enum {ALL_LEDS_ON = ~0, ALL_LEDS_OFF = ~ALL_LEDS_ON};
enum {FIRST_LED = 1, LAST_LED = 16};

void LedDriver_Create(uint16_t* address)
{
	LedAddress = address;
	LedDriver_TurnAllOff();
}

void LedDriver_Destroy(void)
{

}

void LedDriver_TurnOn(int ledNumber)
{
	if (outOfBounds(ledNumber)) return;
	ledsImage |= convertLedNumberToBit(ledNumber);
	updateLedStatus();
}


void LedDriver_TurnOff(int ledNumber)
{
	if (outOfBounds(ledNumber)) return;
	ledsImage &= ~(convertLedNumberToBit(ledNumber));
	updateLedStatus();
}

void LedDriver_TurnAllOn()
{
	ledsImage = ALL_LEDS_ON;
	updateLedStatus();
}

void LedDriver_TurnAllOff()
{
	ledsImage = ALL_LEDS_OFF;
	updateLedStatus();
}

bool LedDriver_IsOn(int ledNumber)
{
	if (outOfBounds(ledNumber)) return false;
	return ledsImage & (convertLedNumberToBit(ledNumber));
}

static void updateLedStatus() 
{
	*LedAddress = ledsImage;
}

static bool outOfBounds(int ledNumber)
{
	return ledNumber < FIRST_LED || ledNumber > LAST_LED;
}

static uint16_t convertLedNumberToBit(int ledNumber)
{
	return 1 << (ledNumber - 1);
}