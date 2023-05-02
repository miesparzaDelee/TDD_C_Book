#include "FakeTimeService.h"

static int timeRet = TIME_UNKNOWN;
static int dayRet = TIME_UNKNOWN;

static void (*alarm)(void);

Time * TimeService_GetTime(Time* time)
{
	time->minuteOfDay = timeRet;
	time->dayOfWeek = dayRet;
	return time;
}

void TimeService_SetPeriodicAlarm(int seconds, void (* callback)(void)) 
{
	alarm = callback;
}

void TimeService_CancelPeriodicAlarm(void (*callback)(void))
{
	if (callback == alarm)
		alarm = 0;
}

void FakeTimeService_SetMinute(int minute)
{
	timeRet = minute;
}

void FakeTimeService_SetDay(Day day) {
	dayRet = day;
}

void * FakeTimeService_GetAlarm()
{
	return (void*)alarm;
}