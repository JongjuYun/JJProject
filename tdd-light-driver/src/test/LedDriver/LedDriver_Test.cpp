#include "LedDriver.h"
#include "CppUTest/TestHarness.h"

#define When
LedDriver *ledDriver;
enum
{
  LedId_Green = 0,
  LedId_Yellow,
  LedId_Red,

  GpioChannel_5 = 5,
  GpioChannel_6 = 6,
  GpioChannel_7 = 7
};

LedDriver_MappingTable_t mappingTable = {
    { LedId_Green, GpioChannel_5 },
    { LedId_Yellow, GpioChannel_6 },
    { LedId_Red, GpioChannel_7 }
  };

TEST_GROUP(LedDriver)
{
  void setup()
  {

  }

  void teardown()
  {
    delete ledDriver;
  }

  void LedDriverIsInitialized()
  {
    // shared_ptr<LedDriver> ledDriver2(new LedDriver(&mappingTable));
    // ledDriver = ledDriver2;
    ledDriver = new LedDriver(&mappingTable);
  }

  void ShouldTurnAllLedsOff()
  {
     CHECK_EQUAL(0, ledDriver->GetState());
  }

  void ShouldLedsStatusBe(uint16_t expected)
  {
      CHECK_EQUAL(expected, ledDriver->GetState());
  }

};

TEST(LedDriver, LedsShouldBeOffAfterCreated)
{
  LedDriverIsInitialized();
  ShouldTurnAllLedsOff();
}

TEST(LedDriver, ShouldAbleToTurnSingleLedOn)
{
  LedDriverIsInitialized();

  ledDriver->TurnOn(LedId_Green);
  ShouldLedsStatusBe(0x1 << GpioChannel_5);
}

TEST(LedDriver, ShouldAbleToTurnMultipleLedOn)
{
  LedDriverIsInitialized();

  ledDriver->TurnOn(LedId_Green);
  ledDriver->TurnOn(LedId_Yellow);
  ShouldLedsStatusBe(0x1 << GpioChannel_5 | 0x1 << GpioChannel_6);

  ledDriver->TurnOn(LedId_Red);
  ShouldLedsStatusBe(0x1 << GpioChannel_5 | 0x1 << GpioChannel_6 | 0x1 << GpioChannel_7);
}

TEST(LedDriver, ShouldAbleToTurnSingleLedOff)
{
  LedDriverIsInitialized();

  ledDriver->TurnOn(LedId_Green);
  ledDriver->TurnOff(LedId_Green);

  ShouldTurnAllLedsOff();
}

TEST(LedDriver, ShouldAbleToTurnMultipleLedOff)
{
  LedDriverIsInitialized();

  ledDriver->TurnOn(LedId_Green);
  ledDriver->TurnOn(LedId_Yellow);
  ledDriver->TurnOn(LedId_Red);

  ledDriver->TurnOff(LedId_Green);
  ledDriver->TurnOff(LedId_Yellow);

  ShouldLedsStatusBe(0x1 << GpioChannel_7);
}
/* Tips : reference로 받아서 변수처럼 사용하고 싶다면,
   &로 받지만 보낼 때는 변수로 보낸다 !!
*/
 TEST(LedDriver, ShouldAbleToTurnSingleLedOn_Channel)
 {
   //LedDriver_Create(&virtualLeds, mappingTable);
   LedDriverIsInitialized();

   ledDriver->TurnOn(LedId_Green);
   ShouldLedsStatusBe(0x1 << GpioChannel_5);

   ledDriver->TurnOn(LedId_Yellow);
   ShouldLedsStatusBe(0x1 << GpioChannel_5 | 0x1 << GpioChannel_6);

   ledDriver->TurnOff(LedId_Yellow);
   ShouldLedsStatusBe((0x1 << GpioChannel_5 | 0x1 << GpioChannel_6) & ~(0x1 << GpioChannel_6));

 }
