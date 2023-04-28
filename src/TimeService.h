#ifndef TIME_SERVICE_H
#define TIME_SERVICE_H

typedef struct
{
    int minuteOfDay;
    int dayOfWeek;
}Time;


Time * TimeService_GetTime(Time * time);

void TimeService_SetPeriodicAlarm(void);

#endif