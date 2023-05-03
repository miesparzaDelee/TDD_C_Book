#include "LightControllerSpy.h"

static int lastId;
static int lastState;
static int states[32];

static void setLedState(int id, int state);

void LightController_Create(void)
{
	lastId = LIGHT_ID_UNKNOWN;
	lastState = LIGHT_STATE_UNKNOWN;
	for (int i = 0; i < 32; i++)
		states[i] = LIGHT_STATE_UNKNOWN;
}
void LightController_Destroy(void)
{

}

void LightController_On(int id)
{
	setLedState(id, LIGHT_ON);
}
void LightController_Off(int id)
{
	setLedState(id, LIGHT_OFF);
}
int LightControllerSpy_GetLastId(void)
{
	return lastId;
}
int LightControllerSpy_GetLastState(void)
{
	return lastState;
}
int LightControllerSpy_GetLightState(int id)
{
	return states[id];
}

static void setLedState(int id, int state)
{
	lastId = id;
	lastState = state;
	states[id] = state;
}