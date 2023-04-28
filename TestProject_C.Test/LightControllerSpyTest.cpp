#include "gtest/gtest.h"

extern "C" {
#include "LightControllerSpy.h"
}


class LightControllerSpy : public testing::Test
{
	void SetUp()
	{
		LightController_Create();
	}

	void TearDown()
	{
		LightController_Destroy();
	}
};

TEST_F(LightControllerSpy, Create)
{
	ASSERT_EQ(LIGHT_ID_UNKNOWN, LightControllerSpy_GetLastId());
	ASSERT_EQ(LIGHT_STATE_UNKNOWN, LightControllerSpy_GetLastState());
}

TEST_F(LightControllerSpy, RememberTheLastLightIdControlled)
{
	LightController_On(10);
	ASSERT_EQ(10, LightControllerSpy_GetLastId());
	ASSERT_EQ(LIGHT_ON, LightControllerSpy_GetLastState());
}