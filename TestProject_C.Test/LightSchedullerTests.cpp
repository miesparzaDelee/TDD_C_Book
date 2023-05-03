#include "gtest/gtest.h"

extern "C" {
#include "LightScheduller.h"
#include "FakeTimeService.h"
#include "LightControllerSpy.h"
#include "RandomMinute.h"
}

static void setTimeTo(Day day, int minute);
static void checkLightState(int light, int id);
static void checkLightState(int id, int level);

class LightScheduler : public testing::Test
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

TEST_F(LightScheduler, NoScheduleNothingHappens)
{
	setTimeTo(MONDAY, 100);

	LightScheduler_Wakeup();
	
	checkLightState(LIGHT_STATE_UNKNOWN, LIGHT_ID_UNKNOWN);
}

TEST_F(LightScheduler, SchedulleOnEverydayNotTimeYet)
{
	LightScheduler_ScheduleTurnOn(3, EVERYDAY, 1200);
	setTimeTo(MONDAY, 1199);

	LightScheduler_Wakeup();

	checkLightState(LIGHT_STATE_UNKNOWN, LIGHT_ID_UNKNOWN);
}

TEST_F(LightScheduler, SchedulleOnEverydayItsTime)
{
	LightScheduler_ScheduleTurnOn(3, EVERYDAY, 1200);
	setTimeTo(MONDAY, 1200);

	LightScheduler_Wakeup();

	checkLightState(LIGHT_ON, 3);
}

TEST_F(LightScheduler, SchedulleOffEverydayItsTime)
{
	LightScheduler_ScheduleTurnOff(3, EVERYDAY, 1200);
	setTimeTo(MONDAY, 1200);

	LightScheduler_Wakeup();

	checkLightState(LIGHT_OFF, 3);
}

TEST_F(LightScheduler, SheduleTuesdayButItsMonday)
{
	LightScheduler_ScheduleTurnOn(3, TUESDAY, 1200);
	setTimeTo(MONDAY, 1200);

	LightScheduler_Wakeup();

	checkLightState(LIGHT_STATE_UNKNOWN, LIGHT_ID_UNKNOWN);
}

TEST_F(LightScheduler, ScheduleTuesdayAndItsTuesday)
{
	LightScheduler_ScheduleTurnOn(3, TUESDAY, 1200);
	setTimeTo(TUESDAY, 1200);
	LightScheduler_Wakeup();
	checkLightState(LIGHT_ON, 3);
}

TEST_F(LightScheduler, ScheduleWeekendItsFriday)
{
	LightScheduler_ScheduleTurnOn(3, WEEKEND, 1200);
	setTimeTo(FRIDAY, 1200);

	LightScheduler_Wakeup();

	checkLightState(LIGHT_STATE_UNKNOWN, LIGHT_ID_UNKNOWN);
}

TEST_F(LightScheduler, ScheduleWeekendItsSaturday)
{
	LightScheduler_ScheduleTurnOn(3, WEEKEND, 1200);
	setTimeTo(SATURDAY, 1200);

	LightScheduler_Wakeup();

	checkLightState(LIGHT_ON, 3);
}

TEST_F(LightScheduler, ScheduleWeekendItsSunday)
{
	LightScheduler_ScheduleTurnOn(3, WEEKEND, 1200);
	setTimeTo(SUNDAY, 1200);

	LightScheduler_Wakeup();

	checkLightState(LIGHT_ON, 3);
}

TEST_F(LightScheduler, ScheduleWeekdayItsSunday)
{
	LightScheduler_ScheduleTurnOn(3, WEEKDAY, 1200);
	setTimeTo(SUNDAY, 1200);

	LightScheduler_Wakeup();

	checkLightState(LIGHT_STATE_UNKNOWN, LIGHT_ID_UNKNOWN);
}

TEST_F(LightScheduler, ScheduleWeekdayItsMonday)
{
	LightScheduler_ScheduleTurnOn(3, WEEKDAY, 1200);
	setTimeTo(MONDAY, 1200);

	LightScheduler_Wakeup();

	checkLightState(LIGHT_ON, 3);
}

TEST_F(LightScheduler, ScheduleTwoEventsSameTime)
{
	LightScheduler_ScheduleTurnOn(3, SUNDAY, 1200);
	LightScheduler_ScheduleTurnOn(12, SUNDAY, 1200);

	setTimeTo(SUNDAY, 1200);

	LightScheduler_Wakeup();

	checkLightState(LIGHT_ON, 3);
	checkLightState(LIGHT_ON, 12);
}

TEST_F(LightScheduler, ScheduleMaxEvents)
{
	int i;
	for (i = 0; i < 128; i++)
		ASSERT_EQ(LS_OK, LightScheduler_ScheduleTurnOn(6, MONDAY, 600 + i));

	ASSERT_EQ(LS_TOO_MANY_EVENTS, LightScheduler_ScheduleTurnOn(6, MONDAY, 600 + i));
}

TEST_F(LightScheduler, RemoveRecyclesSlot)
{
	int i;
	for (i = 0; i < 128; i++)
		ASSERT_EQ(LS_OK, LightScheduler_ScheduleTurnOn(6, MONDAY, 600 + i));
	
	LightScheduler_ScheduleRemove(6, MONDAY, 600);

	ASSERT_EQ(LS_OK, LightScheduler_ScheduleTurnOn(13, MONDAY, 1000));

}

TEST_F(LightScheduler, RemoveEventsStates)
{
	LightScheduler_ScheduleTurnOn(6, MONDAY, 600);
	LightScheduler_ScheduleTurnOn(7, MONDAY, 600);

	LightScheduler_ScheduleRemove(6, MONDAY, 600);

	setTimeTo(MONDAY, 600);

	LightScheduler_Wakeup();

	checkLightState(LIGHT_STATE_UNKNOWN, 6);
	checkLightState(LIGHT_ON, 7);
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
	if (id == LIGHT_ID_UNKNOWN)
	{
		ASSERT_EQ(id, LightControllerSpy_GetLastId());
		ASSERT_EQ(light, LightControllerSpy_GetLastState());
	}
	else
		ASSERT_EQ(light, LightControllerSpy_GetLightState(id));
}