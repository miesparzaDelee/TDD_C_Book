#ifndef LIGHT_SCHEDULLER_H
#define LIGHT_SCHEDULLER_H

#define LS_OK 0
#define LS_TOO_MANY_EVENTS 1
#define LS_NO_EVENT_SCHEDULED 2

typedef enum {
	NONE = -1,
	MONDAY = 1,
	TUESDAY,
	WEDNESDAY,
	THURSDAY,
	FRIDAY,
	SATURDAY,
	SUNDAY,
	EVERYDAY = 10, WEEKDAY, WEEKEND,
}Day;

void LightScheduller_Destroy();

void LightScheduller_Create();

void LightScheduler_Wakeup();

int LightScheduler_ScheduleTurnOn(int id, Day day, int minuteOftheDay);

int LightScheduler_ScheduleTurnOff(int id, Day day, int minuteOftheDay);

int LightScheduler_Randomize(int id, Day day, int minuteOftheDay);

int LightScheduler_ScheduleRemove(int id, Day day, int minuteOftheDay);

#endif
