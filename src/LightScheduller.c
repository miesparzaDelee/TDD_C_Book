#include "LightScheduller.h"
#include "LightController.h"
#include "TimeService.h"

#define LIGHT_SCHEDULER_NO_ID -1
#define LIGHT_SCHEDULER_LIGHT_OFF 0
#define LIGHT_SCHEDULER_LIGHT_ON 1

typedef struct
{
    int id;
    Day reactionDay;
    int minuteOfDay;
    int event;
}ScheduledLightEvent;

static ScheduledLightEvent scheduledLightEvent;

static void scheduleEvent(int id, Day day, int minuteOftheDay, int event);
static void processEventDueNow(Time* time, ScheduledLightEvent* lightEvent);
static int IsWeekend(ScheduledLightEvent* lightEvent, Time* time);
static int isSameDay(ScheduledLightEvent* lightEvent, Time* time);
static int isEveryDay(ScheduledLightEvent* lightEvent);
static void operateLight(ScheduledLightEvent* lightEvent);

void LightScheduller_Create()
{
    scheduledLightEvent.id = LIGHT_SCHEDULER_NO_ID;
    TimeService_SetPeriodicAlarm(60, LightScheduler_Wakeup);
}

void LightScheduller_Destroy()
{
    TimeService_CancelPeriodicAlarm(LightScheduler_Wakeup);
}

void LightScheduler_ScheduleTurnOn(int id, Day day, int minuteOftheDay)
{
    scheduleEvent(id, day, minuteOftheDay, LIGHT_SCHEDULER_LIGHT_ON);
}

void LightScheduler_ScheduleTurnOff(int id, Day day, int minuteOftheDay)
{
    scheduleEvent(id, day, minuteOftheDay, LIGHT_SCHEDULER_LIGHT_OFF);
}

void LightScheduler_Wakeup()
{
    Time time;
    TimeService_GetTime(&time);
    processEventDueNow(&time, &scheduledLightEvent);
}

void scheduleEvent(int id, Day day, int minuteOftheDay, int event)
{
    scheduledLightEvent.id = id;
    scheduledLightEvent.reactionDay = day;
    scheduledLightEvent.minuteOfDay = minuteOftheDay;
    scheduledLightEvent.event = event;
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
