#include "gtest/gtest.h"

extern "C" {
#include "FakeTimeService.h"
}

class FakeTimeService : public testing::Test
{
	void SetUp()
	{

	}

	void TearDown()
	{

	}
};

TEST_F(FakeTimeService, Create)
{
	Time time;

	TimeService_GetTime(&time);

	ASSERT_EQ(TIME_UNKNOWN, time.minuteOfDay);
	ASSERT_EQ(TIME_UNKNOWN, time.dayOfWeek);
}

TEST_F(FakeTimeService, Set)
{
	Time time;
	FakeTimeService_SetMinute(42);
	FakeTimeService_SetDay(SATURDAY);

	TimeService_GetTime(&time);

	ASSERT_EQ(42, time.minuteOfDay);
	ASSERT_EQ(SATURDAY, time.dayOfWeek);
}