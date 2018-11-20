/*!
 * @file
 * @brief Light scheduler implementation.
 */

#include "LightScheduler.h"


void LightScheduler_Init(LightScheduler_t *instance, I_DigitalOutputGroup_t *lights, I_TimeSource_t *timeSource)
{
  instance->_private.lights = lights;
  instance->_private.timeSource = timeSource;
  instance->_private.count = 0;

  for(int i=0; i<MAX_SCHEDULES; i++)
  {
     instance->_private.schedule[i].occupied = false;
     instance->_private.schedule[i].lightId = 0;
     instance->_private.schedule[i].lightState = false;
     instance->_private.schedule[i].time = 0;

   }
 }

void LightScheduler_AddSchedule(LightScheduler_t *instance, uint8_t lightId, bool lightState, TimeSourceTickCount_t time)
{
   for(int i=0;i<MAX_SCHEDULES;i++)
   {
     if(instance->_private.schedule[i].lightId==lightId && instance->_private.schedule[i].time==time)
     {
       instance->_private.schedule[i].lightState = lightState;
       return;
     }
   }

   for(i=0;i<MAX_SCHEDULES;i++)
   {
     if(instance->_private.schedule[i].occupied == false)
     {
       instance->_private.schedule[i].lightId = lightId;
       instance->_private.schedule[i].lightState = lightState;
       instance->_private.schedule[i].time = time;
       instance->_private.schedule[i].occupied= true;
       return;
     }
   }


  }
void LightScheduler_RemoveSchedule(LightScheduler_t *instance, uint8_t lightId, bool lightState, TimeSourceTickCount_t time)
{
  //삭제되어서 빈 배열 공간은 남겨두고 다음 ADD 시에, 그 스케쥴러가 그 빈 공간으로 위치해야함
   for(int i=0; i<MAX_SCHEDULES; i++)
   {
     if(instance->_private.schedule[i].occupied==true)
     {
       if(instance->_private.schedule[i].lightId==lightId && instance->_private.schedule[i].time==time && instance->_private.schedule[i].lightState==lightState)
       {
       instance->_private.schedule[i].lightId = 0;
       instance->_private.schedule[i].time = 0;
       instance->_private.schedule[i].lightState = false;
       instance->_private.schedule[i].occupied = false;
       return;
       }

     }
   }

}

void LightScheduler_Run(LightScheduler_t *instance)
{
   TimeSourceTickCount_t tick = TimeSource_GetTicks(instance->_private.timeSource);
   for(int i=0; i<MAX_SCHEDULES; i++)
   {
   if(instance->_private.schedule[i].time == tick)
   {
      DigitalOutputGroup_Write(instance->_private.lights, instance->_private.schedule[i].lightId, instance->_private.schedule[i].lightState);
      instance->_private.schedule[i].lightId = 0;
      instance->_private.schedule[i].time = 0;
      instance->_private.schedule[i].lightState = false;
      instance->_private.schedule[i].occupied = false;

   }
   }


}
