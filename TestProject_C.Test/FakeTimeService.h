#ifndef FAKE_TIME_SERVICE_H
#define FAKE_TIME_SERVICE_H

#include "TimeService.h"
#include "LightScheduller.h"

#define TIME_UNKNOWN -1

void FakeTimeService_SetMinute(int minute);
void FakeTimeService_SetDay(Day day);

void * FakeTimeService_GetAlarm(void);

#endif