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