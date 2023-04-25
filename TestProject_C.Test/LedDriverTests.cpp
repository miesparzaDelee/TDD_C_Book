#include "gtest/gtest.h"

extern "C" {
#include "LedDriver.h"
}

static uint16_t * virtualLeds;

class LedDriverTest : public testing::Test
{
	void SetUp()
	{
		virtualLeds = (uint16_t *)calloc(1, sizeof(uint16_t));
		LedDriver_Create(virtualLeds);
	}

	void TearDown()
	{
		free(virtualLeds);
	}
};

TEST_F(LedDriverTest, LedsOffAfterCreate)
{
	ASSERT_EQ(0, *virtualLeds);
}

TEST_F(LedDriverTest, TurnOnLedOne)
{
	LedDriver_TurnOn(1);
	ASSERT_EQ(1, *virtualLeds);
}

TEST_F(LedDriverTest, TurnOffLedOne)
{
	LedDriver_TurnOn(1);
	LedDriver_TurnOff(1);
	ASSERT_EQ(0, *virtualLeds);
}

TEST_F(LedDriverTest, TurnOnMultipleLeds)
{
	LedDriver_TurnOn(9);
	LedDriver_TurnOn(8);
	ASSERT_EQ(0x180, *virtualLeds);
}

TEST_F(LedDriverTest, TurnOffMultipleLeds)
{
	LedDriver_TurnAllOn();
	LedDriver_TurnOff(9);
	LedDriver_TurnOff(8);
	ASSERT_EQ((~0x180)&0xffff, *virtualLeds);
}

TEST_F(LedDriverTest, TurnOffAnyLed)
{
	LedDriver_TurnAllOn();
	LedDriver_TurnOff(8);
	ASSERT_EQ(0xff7f, *virtualLeds);
}

TEST_F(LedDriverTest, AllOn)
{
	LedDriver_TurnAllOn();
	ASSERT_EQ(0xffff, *virtualLeds);
}

TEST_F(LedDriverTest, AllOff)
{
	LedDriver_TurnAllOn();
	LedDriver_TurnAllOff();
	ASSERT_EQ(0x0, *virtualLeds);
}

TEST_F(LedDriverTest, LedMemoryIsNotReadable)
{
	*virtualLeds = 0xffff;
	LedDriver_TurnOn(8);
	ASSERT_EQ(0x80, *virtualLeds);
}

TEST_F(LedDriverTest, OutOfBoundsTurnOffDoesNotHarm)
{
	LedDriver_TurnAllOn();
	LedDriver_TurnOff(-1);
	LedDriver_TurnOff(0);
	LedDriver_TurnOff(17);
	LedDriver_TurnOff(3141);
	ASSERT_EQ(0xffff, *virtualLeds);
}

TEST_F(LedDriverTest, OutOfBoundsTurnOnDoesNotHarm)
{
	LedDriver_TurnOn(-1);
	LedDriver_TurnOn(0);
	LedDriver_TurnOn(17);
	LedDriver_TurnOn(3141);
	ASSERT_EQ(0x0, *virtualLeds);
}

TEST_F(LedDriverTest, IsOn)
{
	ASSERT_FALSE(LedDriver_IsOn(11));
	LedDriver_TurnOn(11);
	ASSERT_TRUE(LedDriver_IsOn(11));
}

TEST_F(LedDriverTest, OutOfBoundsLedsAreAlwaysOff)
{
	ASSERT_FALSE(LedDriver_IsOn(0));
	ASSERT_FALSE(LedDriver_IsOn(17));
}