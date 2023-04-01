#include "mapper.h"

namespace nes
{
    Mapper::Mapper(uint8_t prgBanks, uint8_t chrBanks, bool batteryBackedPersistentMem)
        : nPRGBanks(prgBanks), nCHRBanks(chrBanks), containsBatteryMemory(batteryBackedPersistentMem)
    {
    }
}