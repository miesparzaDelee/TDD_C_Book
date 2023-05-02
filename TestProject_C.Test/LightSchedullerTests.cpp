#include "gtest/gtest.h"

extern "C" {
#include "LightScheduller.h"
#include "FakeTimeService.h"
#include "LightControllerSpy.h"
}

static void setTimeTo(Day day, int minute);
static void checkLightState(int light, int id);

class LightScheduller : public testing::Test
{
	void SetUp()
	{
		LightController_Create();
		LightScheduller_Create();
	}

	void TearDown()
	{
		LightScheduller_Destroy();
	}
};

TEST_F(LightScheduller, NoScheduleNothingHappens)
{
	setTimeTo(MONDAY, 100);

	LightScheduler_Wakeup();
	
	checkLightState(LIGHT_STATE_UNKNOWN, LIGHT_ID_UNKNOWN);
}

TEST_F(LightScheduller, SchedulleOnEverydayNotTimeYet)
{
	LightScheduler_ScheduleTurnOn(3, EVERYDAY, 1200);
	setTimeTo(MONDAY, 1199);

	LightScheduler_Wakeup();

	checkLightState(LIGHT_STATE_UNKNOWN, LIGHT_ID_UNKNOWN);
}

TEST_F(LightScheduller, SchedulleOnEverydayItsTime)
{
	LightScheduler_ScheduleTurnOn(3, EVERYDAY, 1200);
	setTimeTo(MONDAY, 1200);

	LightScheduler_Wakeup();

	checkLightState(LIGHT_ON, 3);
}

TEST_F(LightScheduller, SchedulleOffEverydayItsTime)
{
	LightScheduler_ScheduleTurnOff(3, EVERYDAY, 1200);
	setTimeTo(MONDAY, 1200);

	LightScheduler_Wakeup();

	checkLightState(LIGHT_OFF, 3);
}

TEST_F(LightScheduller, SheduleTuesdayButItsMonday)
{
	LightScheduler_ScheduleTurnOn(3, TUESDAY, 1200);
	setTimeTo(MONDAY, 1200);

	LightScheduler_Wakeup();

	checkLightState(LIGHT_STATE_UNKNOWN, LIGHT_ID_UNKNOWN);
}

TEST_F(LightScheduller, ScheduleTuesdayAndItsTuesday)
{
	LightScheduler_ScheduleTurnOn(3, TUESDAY, 1200);
	setTimeTo(TUESDAY, 1200);
	LightScheduler_Wakeup();
	checkLightState(LIGHT_ON, 3);
}

TEST_F(LightScheduller, ScheduleWeekendItsFriday)
{
	LightScheduler_ScheduleTurnOn(3, WEEKEND, 1200);
	setTimeTo(FRIDAY, 1200);

	LightScheduler_Wakeup();

	checkLightState(LIGHT_STATE_UNKNOWN, LIGHT_ID_UNKNOWN);
}

TEST_F(LightScheduller, ScheduleWeekendItsSaturday)
{
	LightScheduler_ScheduleTurnOn(3, WEEKEND, 1200);
	setTimeTo(SATURDAY, 1200);

	LightScheduler_Wakeup();

	checkLightState(LIGHT_ON, 3);
}

TEST_F(LightScheduller, ScheduleWeekendItsSunday)
{
	LightScheduler_ScheduleTurnOn(3, WEEKEND, 1200);
	setTimeTo(SUNDAY, 1200);

	LightScheduler_Wakeup();

	checkLightState(LIGHT_ON, 3);
}

TEST_F(LightScheduller, ScheduleWeekdayItsSunday)
{
	LightScheduler_ScheduleTurnOn(3, WEEKDAY, 1200);
	setTimeTo(SUNDAY, 1200);

	LightScheduler_Wakeup();

	checkLightState(LIGHT_STATE_UNKNOWN, LIGHT_ID_UNKNOWN);
}

TEST_F(LightScheduller, ScheduleWeekdayItsMonday)
{
	LightScheduler_ScheduleTurnOn(3, WEEKDAY, 1200);
	setTimeTo(MONDAY, 1200);

	LightScheduler_Wakeup();

	checkLightState(LIGHT_ON, 3);
}

class LightSchedullerInitAndCleanup : public testing::Test
{
	void SetUp()
	{
	}

	void TearDown()
	{
	}
};

TEST_F(LightSchedullerInitAndCleanup, CreateStartAlarm)
{
	LightScheduller_Create();

	ASSERT_EQ((void*)LightScheduler_Wakeup, FakeTimeService_GetAlarm());

}

TEST_F(LightSchedullerInitAndCleanup, DestroyDeletesAlarm)
{
	LightScheduller_Create();
	LightScheduller_Destroy();

	ASSERT_EQ(0, FakeTimeService_GetAlarm());

}

void setTimeTo(Day day, int minute)
{
	FakeTimeService_SetDay(day);
	FakeTimeService_SetMinute(minute);
}

void checkLightState(int light, int id)
{
	ASSERT_EQ(id, LightControllerSpy_GetLastId());
	ASSERT_EQ(light, LightControllerSpy_GetLastState());
}