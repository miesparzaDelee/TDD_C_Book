#ifndef FAKE_TIME_SERVICE_H
#define FAKE_TIME_SERVICE_H

#include "TimeService.h"

typedef enum {
	TIME_UNKNOWN = - 1,
	SATURDAY = 3,
}Day;

void FakeTimeService_SetMinute(int minute);
void FakeTimeService_SetDay(Day day);

#endif