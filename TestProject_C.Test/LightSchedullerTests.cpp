#include "gtest/gtest.h"

extern "C" {
#include "LightScheduller.h"
#include "LightControllerSpy.h"
}

class LightScheduller : public testing::Test
{
	void SetUp()
	{
		LightController_Create();
		LightScheduller_Create();
	}

	void TearDown()
	{
		LightController_Destroy();
		LightScheduller_Destroy();
	}
};

TEST_F(LightScheduller, SchedulleCreateL)
{
	ASSERT_EQ(LIGHT_ID_UNKNOWN, LightControllerSpy_GetLastId());
	ASSERT_EQ(LIGHT_STATE_UNKNOWN, LightControllerSpy_GetLastState());
}

//TEST_F(LightScheduller, SchedulleOnEverydayNotTimeYet)
//{
//	LightScheduller_ScheduleTurnOn(3, EVERYDAY, 1200);
//	FakeTimeService_SetDay(MONDAY);
//	FakeTimeService_SetMinute(1199);
//
//	LightScheduller_WakeUp();
//
//	ASSERT_EQ(LIGHT_ID_UNKNOWN, LightControllerSpy_GetLastId());
//	ASSERT_EQ(LIGHT_STATE_UNKNOWN, LightControllerSpy_GetLastState());
//}