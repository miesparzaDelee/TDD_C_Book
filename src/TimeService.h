#ifndef TIME_SERVICE_H
#define TIME_SERVICE_H

typedef struct
{
    int minuteOfDay;
    int dayOfWeek;
}Time;


Time * TimeService_GetTime(Time * time);

void TimeService_SetPeriodicAlarm(int seconds, void (*callback)(void));

void TimeService_CancelPeriodicAlarm(void (*callback)(void));

#endif