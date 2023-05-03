#include "gtest/gtest.h"

extern "C" {
#include "RandomMinute.h"
#include <stdio.h>
}

enum {BOUND = 30};

class RandomMinute : public testing::Test
{
public:
	int minute = 0;

	void SetUp()
	{
		RandomMinute_Create(BOUND);
		srand(1);
	}

	void TearDown()
	{
	}

	void AssertMinuteIsInRange()
	{
		if (minute < -BOUND ||  minute > BOUND)
		{
			printf("bad minute value %d\n", minute);
			FAIL("Minute out of range");
		}
	}
};


TEST_F(RandomMinute, GetIsInRange)
{
	for (int i = 0; i < 100; i++)
	{
	  this->minute = RandomMinute_Get();
	  this->AssertMinuteIsInRange();
	}
}
