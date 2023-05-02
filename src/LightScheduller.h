#ifndef LIGHT_SCHEDULLER_H
#define LIGHT_SCHEDULLER_H

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

void LightScheduler_ScheduleTurnOn(int id, Day day, int minuteOftheDay);

void LightScheduler_ScheduleTurnOff(int id, Day day, int minuteOftheDay);

#endif
