#ifndef LEDDRIVER_H
#define LEDDRIVER_H

#include <cstdint>
#include <vector>
#include <memory>
using namespace std;

typedef vector<pair<int,int>> LedDriver_MappingTable_t;

class LedDriver
{
private:
  uint16_t ledState;
  const LedDriver_MappingTable_t *mappingTable;

public:
  LedDriver(const LedDriver_MappingTable_t *mappingTable);
  ~LedDriver();

  void TurnOn(int color);
  void TurnOff(int color);

  uint16_t GetState();
};

#endif
