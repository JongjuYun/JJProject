/*!
 * @file
 * @brief Tests for light scheduler implementation.
 */

extern "C"
{
#include "LightScheduler.h"
}

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "DigitalOutputGroup_Mock.h"
#include "TimeSource_Mock.h"

#include "uassert_test.h"

TEST_GROUP(LightScheduler)
{
   LightScheduler_t scheduler;

   DigitalOutputGroup_Mock_t fakeDigitalOutputGroup;
   TimeSource_Mock_t fakeTimeSource;

   void setup()
   {
      DigitalOutputGroup_Mock_Init(&fakeDigitalOutputGroup);
      TimeSource_Mock_Init(&fakeTimeSource);

//      LightScheduler_Init(&scheduler, (I_DigitalOutputGroup_t *)&fakeDigitalOutputGroup, (I_TimeSource_t *)&fakeTimeSource);
   }

   void teardown()
   {
   }
};

TEST(LightScheduler, FirstTest)
{
//   FAIL("Oh good the tests are working");
}

TEST(LightScheduler, NothingShouldHappenOnInitialization)
{
   LightScheduler_Init(&scheduler, (I_DigitalOutputGroup_t *)&fakeDigitalOutputGroup, (I_TimeSource_t *)&fakeTimeSource);
}

TEST(LightScheduler, ShouldBeAbleToAddOneSchedule)
{
   LightScheduler_Init(&scheduler, (I_DigitalOutputGroup_t *)&fakeDigitalOutputGroup, (I_TimeSource_t *)&fakeTimeSource);
   LightScheduler_AddSchedule(&scheduler, 1, true, 2);

   mock().expectOneCall("GetTicks")
         .onObject((void *)&fakeTimeSource)
         .andReturnValue(2);

   mock().expectOneCall("Write")
      .onObject(&fakeDigitalOutputGroup)
      .withParameter("channel", 1)
      .withParameter("state", true);

   LightScheduler_Run(&scheduler);
}

TEST(LightScheduler, ShouldBeAbleToAddTwoSchedules)
{
   LightScheduler_Init(&scheduler, (I_DigitalOutputGroup_t *)&fakeDigitalOutputGroup, (I_TimeSource_t *)&fakeTimeSource);
   LightScheduler_AddSchedule(&scheduler, 1, true, 3);
   LightScheduler_AddSchedule(&scheduler, 2, true, 4);

   mock().expectOneCall("GetTicks")
         .onObject((void *)&fakeTimeSource)
         .andReturnValue(3);

   mock().expectOneCall("Write")
      .onObject(&fakeDigitalOutputGroup)
      .withParameter("channel", 1)
      .withParameter("state", true);

   LightScheduler_Run(&scheduler);

   mock().expectOneCall("GetTicks")
            .onObject((void *)&fakeTimeSource)
            .andReturnValue(4);

      mock().expectOneCall("Write")
         .onObject(&fakeDigitalOutputGroup)
         .withParameter("channel", 2)
         .withParameter("state", true);

      LightScheduler_Run(&scheduler);

}

TEST(LightScheduler, CanAddSchedulesForOneLedWithDifferentStates)
{
   LightScheduler_Init(&scheduler, (I_DigitalOutputGroup_t *)&fakeDigitalOutputGroup, (I_TimeSource_t *)&fakeTimeSource);

   LightScheduler_AddSchedule(&scheduler, 1, true, 2);
   LightScheduler_AddSchedule(&scheduler, 1, false, 4);

   mock().expectOneCall("GetTicks")
         .onObject((void *)&fakeTimeSource)
         .andReturnValue(2);

   mock().expectOneCall("Write")
      .onObject(&fakeDigitalOutputGroup)
      .withParameter("channel", 1)
      .withParameter("state", true);

   LightScheduler_Run(&scheduler);

   mock().expectOneCall("GetTicks")
         .onObject((void *)&fakeTimeSource)
         .andReturnValue(4);

   mock().expectOneCall("Write")
      .onObject(&fakeDigitalOutputGroup)
      .withParameter("channel", 1)
      .withParameter("state", false);

   LightScheduler_Run(&scheduler);
}

TEST(LightScheduler, CanIgnoreScheduleInputWhenSchedulerIsFull)
{
   LightScheduler_Init(&scheduler, (I_DigitalOutputGroup_t *)&fakeDigitalOutputGroup, (I_TimeSource_t *)&fakeTimeSource);

   for(int i=1;i<=MAX_SCHEDULES+1;i++){
   LightScheduler_AddSchedule(&scheduler, i, true, i+2);
   }


   // mock().expectOneCall("GetTicks")
   //                     .onObject((void *)&fakeTimeSource)
   //                     .andReturnValue(3);
   // LightScheduler_Run(&scheduler);


   for(int i=1;i<=MAX_SCHEDULES;i++){

      mock().expectOneCall("GetTicks")
                           .onObject((void *)&fakeTimeSource)
                           .andReturnValue(i+2);

                     mock().expectOneCall("Write")
                        .onObject(&fakeDigitalOutputGroup)
                        .withParameter("channel",i )
                        .withParameter("state", true);

                     LightScheduler_Run(&scheduler);

   }
   //
   //
   mock().expectOneCall("GetTicks")
                       .onObject((void *)&fakeTimeSource)
                       .andReturnValue(MAX_SCHEDULES+3);
   LightScheduler_Run(&scheduler);
}

TEST(LightScheduler, CanUpdateScheculeWhenTickAndNumberMatches)
{
  LightScheduler_Init(&scheduler, (I_DigitalOutputGroup_t *)&fakeDigitalOutputGroup, (I_TimeSource_t *)&fakeTimeSource);

  LightScheduler_AddSchedule(&scheduler, 1, true, 4);
  LightScheduler_AddSchedule(&scheduler, 1, false, 4);

  mock().expectOneCall("GetTicks")
        .onObject((void *)&fakeTimeSource)
        .andReturnValue(4);

  mock().expectOneCall("Write")
     .onObject(&fakeDigitalOutputGroup)
     .withParameter("channel", 1)
     .withParameter("state", false);

  LightScheduler_Run(&scheduler);


}

TEST(LightScheduler, CanRemoveOneSchedule)
{
  LightScheduler_Init(&scheduler, (I_DigitalOutputGroup_t *)&fakeDigitalOutputGroup, (I_TimeSource_t *)&fakeTimeSource);
  LightScheduler_AddSchedule(&scheduler, 2, true, 10);
  LightScheduler_RemoveSchedule(&scheduler, 2, true, 10);

  mock().expectOneCall("GetTicks")
  .onObject((void *)&fakeTimeSource)
  .andReturnValue(10);

  LightScheduler_Run(&scheduler);
}

TEST(LightScheduler, CanRemoveMultipleSchedules)
{
  LightScheduler_Init(&scheduler, (I_DigitalOutputGroup_t *)&fakeDigitalOutputGroup, (I_TimeSource_t *)&fakeTimeSource);
  LightScheduler_AddSchedule(&scheduler, 1, true, 1);
  LightScheduler_AddSchedule(&scheduler, 2, true, 2);
  LightScheduler_AddSchedule(&scheduler, 3, true, 3);
  LightScheduler_RemoveSchedule(&scheduler, 2, true, 2);
  LightScheduler_RemoveSchedule(&scheduler, 3, true, 3);

  mock().expectOneCall("GetTicks")
  .onObject((void *)&fakeTimeSource)
  .andReturnValue(1);

  mock().expectOneCall("Write")
     .onObject(&fakeDigitalOutputGroup)
     .withParameter("channel", 1)
     .withParameter("state", true);

       LightScheduler_Run(&scheduler);

     mock().expectOneCall("GetTicks")
     .onObject((void *)&fakeTimeSource)
     .andReturnValue(2);

     LightScheduler_Run(&scheduler);

     mock().expectOneCall("GetTicks")
     .onObject((void *)&fakeTimeSource)
     .andReturnValue(3);


  LightScheduler_Run(&scheduler);

}

TEST(LightScheduler, CanRemoveAndAddSchedulesWhenScheduleIsFull)
{

  LightScheduler_Init(&scheduler, (I_DigitalOutputGroup_t *)&fakeDigitalOutputGroup, (I_TimeSource_t *)&fakeTimeSource);

  for(int i=1;i<=MAX_SCHEDULES;i++){
  LightScheduler_AddSchedule(&scheduler, i, true, i);
  }
  LightScheduler_RemoveSchedule(&scheduler, 2, true, 2);
  LightScheduler_AddSchedule(&scheduler, 12, true, 12);

  mock().expectOneCall("GetTicks")
  .onObject((void *)&fakeTimeSource)
  .andReturnValue(2);

     LightScheduler_Run(&scheduler);


     mock().expectOneCall("GetTicks")
     .onObject((void *)&fakeTimeSource)
     .andReturnValue(12);

     mock().expectOneCall("Write")
        .onObject(&fakeDigitalOutputGroup)
        .withParameter("channel", 12)
        .withParameter("state", true);

    LightScheduler_Run(&scheduler);
}

TEST(LightScheduler, NothingHappenWhenRemoveScheduleAndScheduleIsEmpty)
{

}

TEST(LightScheduler, AssertionWhenRemovedScheduleNotInScheduler)
{

}

TEST(LightScheduler, ShouldRemoveScheduleAfterRunning)
{
  LightScheduler_Init(&scheduler, (I_DigitalOutputGroup_t *)&fakeDigitalOutputGroup, (I_TimeSource_t *)&fakeTimeSource);
  LightScheduler_AddSchedule(&scheduler, 1, true, 1);
  LightScheduler_AddSchedule(&scheduler, 2, true, 2);
  LightScheduler_AddSchedule(&scheduler, 3, true, 3);

  mock().expectOneCall("GetTicks")
  .onObject((void *)&fakeTimeSource)
  .andReturnValue(1);
  mock().expectOneCall("Write")
     .onObject(&fakeDigitalOutputGroup)
     .withParameter("channel", 1)
     .withParameter("state", true);
  LightScheduler_Run(&scheduler);

  mock().expectOneCall("GetTicks")
  .onObject((void *)&fakeTimeSource)
  .andReturnValue(2);
  mock().expectOneCall("Write")
     .onObject(&fakeDigitalOutputGroup)
     .withParameter("channel", 2)
     .withParameter("state", true);
  LightScheduler_Run(&scheduler);

  mock().expectOneCall("GetTicks")
  .onObject((void *)&fakeTimeSource)
  .andReturnValue(3);
  mock().expectOneCall("Write")
     .onObject(&fakeDigitalOutputGroup)
     .withParameter("channel", 3)
     .withParameter("state", true);
  LightScheduler_Run(&scheduler);

  mock().expectOneCall("GetTicks")
  .onObject((void *)&fakeTimeSource)
  .andReturnValue(1);
  LightScheduler_Run(&scheduler);


 //Test코드 채우기  추가 예외도 생각하기

}


// TEST(LightScheduler, CanRemoveScheduler)
// {
//   LightScheduler_Init(&scheduler, (I_DigitalOutputGroup_t *)&fakeDigitalOutputGroup, (I_TimeSource_t *)&fakeTimeSource);
//
//   for(int i=1;i<=MAX_SCHEDULES;i++){
//   LightScheduler_AddSchedule(&scheduler, i, true, i);
//
//   }
//
//   LightScheduler_RemoveSchedule(&scheduler, 2, true, 2);
//   LightScheduler_RemoveSchedule(&scheduler, 3, true, 3);
//   LightScheduler_AddSchedule(&scheduler, 11, true, 11);
//   LightScheduler_AddSchedule(&scheduler, 12, true, 12);
//
//
//   mock().expectOneCall("GetTicks")
//   .onObject((void *)&fakeTimeSource)
//   .andReturnValue(1);
//
//   mock().expectOneCall("Write")
//   .onObject(&fakeDigitalOutputGroup)
//   .withParameter("channel",1 )
//   .withParameter("state", true);
//
//   LightScheduler_Run(&scheduler);
//
//
//   mock().expectOneCall("GetTicks")
//   .onObject((void *)&fakeTimeSource)
//   .andReturnValue(4);
//
//   mock().expectOneCall("Write")
//   .onObject(&fakeDigitalOutputGroup)
//   .withParameter("channel",4 )
//   .withParameter("state", true);
//
//   LightScheduler_Run(&scheduler);
//
//
//   mock().expectOneCall("GetTicks")
//   .onObject((void *)&fakeTimeSource)
//   .andReturnValue(5);
//
//   mock().expectOneCall("Write")
//   .onObject(&fakeDigitalOutputGroup)
//   .withParameter("channel",5 )
//   .withParameter("state", true);
//
//   LightScheduler_Run(&scheduler);
//
//
//   mock().expectOneCall("GetTicks")
//   .onObject((void *)&fakeTimeSource)
//   .andReturnValue(6);
//
//   mock().expectOneCall("Write")
//   .onObject(&fakeDigitalOutputGroup)
//   .withParameter("channel",6 )
//   .withParameter("state", true);
//
//   LightScheduler_Run(&scheduler);
//
//   mock().expectOneCall("GetTicks")
//   .onObject((void *)&fakeTimeSource)
//   .andReturnValue(7);
//
//   mock().expectOneCall("Write")
//   .onObject(&fakeDigitalOutputGroup)
//   .withParameter("channel",7 )
//   .withParameter("state", true);
//
//   LightScheduler_Run(&scheduler);
//
//   mock().expectOneCall("GetTicks")
//   .onObject((void *)&fakeTimeSource)
//   .andReturnValue(8);
//
//   mock().expectOneCall("Write")
//   .onObject(&fakeDigitalOutputGroup)
//   .withParameter("channel",8 )
//   .withParameter("state", true);
//
//   LightScheduler_Run(&scheduler);
//
//   mock().expectOneCall("GetTicks")
//   .onObject((void *)&fakeTimeSource)
//   .andReturnValue(9);
//
//   mock().expectOneCall("Write")
//   .onObject(&fakeDigitalOutputGroup)
//   .withParameter("channel",9 )
//   .withParameter("state", true);
//
//   LightScheduler_Run(&scheduler);
//
//   mock().expectOneCall("GetTicks")
//   .onObject((void *)&fakeTimeSource)
//   .andReturnValue(10);
//
//   mock().expectOneCall("Write")
//   .onObject(&fakeDigitalOutputGroup)
//   .withParameter("channel",10 )
//   .withParameter("state", true);
//
//   LightScheduler_Run(&scheduler);
//
//
//   mock().expectOneCall("GetTicks")
//   .onObject((void *)&fakeTimeSource)
//   .andReturnValue(11);
//
//   mock().expectOneCall("Write")
//   .onObject(&fakeDigitalOutputGroup)
//   .withParameter("channel",11 )
//   .withParameter("state", true);
//
//   LightScheduler_Run(&scheduler);
//
//   mock().expectOneCall("GetTicks")
//   .onObject((void *)&fakeTimeSource)
//   .andReturnValue(12);
//
//   mock().expectOneCall("Write")
//   .onObject(&fakeDigitalOutputGroup)
//   .withParameter("channel",12)
//   .withParameter("state", true);
//
// LightScheduler_Run(&scheduler);
//
//
//
//
// //테스트 먼저 작성
//
// }
