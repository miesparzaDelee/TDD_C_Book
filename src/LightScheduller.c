#include "LightScheduller.h"
#include "LightController.h"
#include "TimeService.h"
#include "RandomMinute.h"
#include <stdbool.h>

#define LIGHT_SCHEDULER_NO_ID -1
#define LIGHT_SCHEDULER_LIGHT_OFF 0
#define LIGHT_SCHEDULER_LIGHT_ON 1
#define MAX_EVENTS 128


typedef struct
{
    int id;
    Day reactionDay;
    int minuteOfDay;
    int event;
}ScheduledLightEvent;

static ScheduledLightEvent scheduledLightEvent;
static ScheduledLightEvent scheduledLightEvents[MAX_EVENTS];

static int scheduleEvent(int id, Day day, int minuteOftheDay, int event);
static void processEventDueNow(Time * time, ScheduledLightEvent* lightEvent);
static int IsWeekend(ScheduledLightEvent * lightEvent, Time* time);
static int isSameDay(ScheduledLightEvent * lightEvent, Time* time);
static int isEveryDay(ScheduledLightEvent * lightEvent);
static void operateLight(ScheduledLightEvent * lightEvent);
static int GetSchedulled(int id, Day day, int minuteOftheDay);

void LightScheduller_Create()
{
    for (int i = 0; i < MAX_EVENTS; i++)
        scheduledLightEvents[i].id = LIGHT_SCHEDULER_NO_ID;
    scheduledLightEvent.id = LIGHT_SCHEDULER_NO_ID;
    TimeService_SetPeriodicAlarm(60, LightScheduler_Wakeup);
}

void LightScheduller_Destroy()
{
    TimeService_CancelPeriodicAlarm(LightScheduler_Wakeup);
}

int LightScheduler_ScheduleTurnOn(int id, Day day, int minuteOftheDay)
{
   return scheduleEvent(id, day, minuteOftheDay, LIGHT_SCHEDULER_LIGHT_ON);
}

int LightScheduler_ScheduleTurnOff(int id, Day day, int minuteOftheDay)
{
   return scheduleEvent(id, day, minuteOftheDay, LIGHT_SCHEDULER_LIGHT_OFF);
}

int LightScheduler_Randomize(int id, Day day, int minuteOftheDay)
{
    int index = GetSchedulled(id, day, minuteOftheDay);
    if (index != -1)
    {
        scheduledLightEvents[index].minuteOfDay += RandomMinute_Get();
        return LS_OK;
    }
    return LS_NO_EVENT_SCHEDULED;
}

static int GetSchedulled(int id, Day day, int minuteOftheDay)
{
    for (int i = 0; i < MAX_EVENTS; i++)
    {
        if (scheduledLightEvents[i].id == id && scheduledLightEvents[i].reactionDay == day && scheduledLightEvents[i].minuteOfDay == minuteOftheDay)
        {
            return i;
        }
    }
    return -1;
}

int LightScheduler_ScheduleRemove(int id, Day day, int minuteOftheDay)
{
	int index = GetSchedulled(id, day, minuteOftheDay);
	if (index != -1)
	{
		scheduledLightEvents[index].id = LIGHT_SCHEDULER_NO_ID;
		return LS_OK;
	}
	return LS_NO_EVENT_SCHEDULED;
}

void LightScheduler_Wakeup()
{
    Time time;
    TimeService_GetTime(&time);
    for(int i = 0; i < MAX_EVENTS; i++)
        processEventDueNow(&time, scheduledLightEvents + i);
}

int scheduleEvent(int id, Day day, int minuteOftheDay, int event)
{
    for (int i = 0; i < MAX_EVENTS; i++)
    {
        if (scheduledLightEvents[i].id == LIGHT_SCHEDULER_NO_ID)
        {
            scheduledLightEvents[i].id = id;
            scheduledLightEvents[i].reactionDay = day;
            scheduledLightEvents[i].minuteOfDay = minuteOftheDay;
            scheduledLightEvents[i].event = event;
            return LS_OK;
        }
    }
    return LS_TOO_MANY_EVENTS;
}

void processEventDueNow(Time* time, ScheduledLightEvent * lightEvent)
{
    if (lightEvent->id == LIGHT_SCHEDULER_NO_ID)
        return;
    if (!isEveryDay(lightEvent) 
            && !isSameDay(lightEvent, time) 
                && !IsWeekend(lightEvent, time) 
                     && !IsWeekday(lightEvent, time))
        return;
    if (lightEvent->minuteOfDay != time->minuteOfDay)
        return;
    operateLight(lightEvent);
}

int IsWeekend(ScheduledLightEvent* lightEvent, Time* time)
{
    return lightEvent->reactionDay == WEEKEND && 
        (time->dayOfWeek == SATURDAY || time->dayOfWeek == SUNDAY);
}

int IsWeekday(ScheduledLightEvent* lightEvent, Time* time)
{
    return lightEvent->reactionDay == WEEKDAY &&
        !(time->dayOfWeek == SATURDAY || time->dayOfWeek == SUNDAY);
}

int isSameDay(ScheduledLightEvent* lightEvent, Time* time)
{
    return lightEvent->reactionDay == time->dayOfWeek;
}

int isEveryDay(ScheduledLightEvent* lightEvent)
{
    return lightEvent->reactionDay == EVERYDAY;
}

void operateLight(ScheduledLightEvent* lightEvent)
{
    if (lightEvent->event == LIGHT_SCHEDULER_LIGHT_ON)
        LightController_On(lightEvent->id);
    if (lightEvent->event == LIGHT_SCHEDULER_LIGHT_OFF)
        LightController_Off(lightEvent->id);
}
