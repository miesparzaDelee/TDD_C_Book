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
static int FakeRandomMinute_Get();
static void FakeRandomMinute_SetFirstAndIncrement(int initVal, int increment);

static int inVal;
static int incr;
static int iter;

static int FakeRandomMinute_Get()
{
	iter++;
	return inVal + iter * incr;
}

static void FakeRandomMinute_SetFirstAndIncrement(int initVal, int increment)
{
	inVal = initVal;
	incr = increment;
	iter = -1;
}

class LightSchedullerRandom : public testing::Test
{
public:
	int (*savedRandomMinute_Get)();

	void SetUp()
	{
		LightController_Create();
		LightScheduller_Create();
		savedRandomMinute_Get = RandomMinute_Get;
		RandomMinute_Get = FakeRandomMinute_Get;
	}

	void TearDown()
	{
		LightController_Destroy();
		LightScheduller_Destroy();
		RandomMinute_Get = savedRandomMinute_Get;
	}

};

TEST_F(LightSchedullerRandom, TurnsOnEarly)
{
	FakeRandomMinute_SetFirstAndIncrement(-10, 5);
	LightScheduler_ScheduleTurnOn(4, EVERYDAY, 600);
	LightScheduler_Randomize(4, EVERYDAY, 600);
	setTimeTo(MONDAY, 600 - 10);
	LightScheduler_Wakeup();
	checkLightState(LIGHT_ON, 4);

	LightScheduler_ScheduleTurnOn(5, EVERYDAY, 600);
	LightScheduler_Randomize(5, EVERYDAY, 600);
	setTimeTo(MONDAY, 600 - 5);
	LightScheduler_Wakeup();
	checkLightState(LIGHT_ON, 5);
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