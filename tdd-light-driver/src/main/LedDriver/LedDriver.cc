#include "LedDriver.h"

LedDriver::LedDriver(const LedDriver_MappingTable_t *mappingTable)
{
  this->mappingTable = mappingTable;
  this->ledState= 0;
}

LedDriver::~LedDriver()
{
}

void LedDriver::TurnOn(int color)
{
  for(auto &item : *mappingTable) //iterator 말고 auto에 익숙해지자!!
  {
    if(color == item.first)
    {
      this->ledState |= 0x1 << item.second;
      break;
    }
  }
}

void LedDriver::TurnOff(int color)
{
  for(auto &item : *mappingTable)
  {
    if(color == item.first)
    {
      this->ledState &= ~(0x1 << item.second);
      break;
    }
  }
}

uint16_t LedDriver::GetState()
{
  return this->ledState;
}
