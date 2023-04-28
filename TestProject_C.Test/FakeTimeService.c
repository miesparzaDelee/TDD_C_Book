#include "FakeTimeService.h"

static int timeRet = TIME_UNKNOWN;
static int dayRet = TIME_UNKNOWN;

Time * TimeService_GetTime(Time* time)
{
	time->minuteOfDay = timeRet;
	time->dayOfWeek = dayRet;
	return time;
}

void TimeService_SetPeriodicAlarm(void) {
	
}

void FakeTimeService_SetMinute(int minute)
{
	timeRet = minute;
}

void FakeTimeService_SetDay(Day day) {
	dayRet = day;
}