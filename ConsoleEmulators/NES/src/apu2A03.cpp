#include "apu2A03.h"

nes::APU::APU()
{
}

void nes::APU::reset()
{
    elapsedCycles = 0;
}

void nes::APU::clock()
{
    if (elapsedCycles % 2 == 1) // Every other cycle work
    {

    }

    elapsedCycles++;
}

uint8_t nes::APU::cpuRead(uint16_t address)
{
    if (address == 0x4015)
    {

    }

    return 0x00;
}

void nes::APU::cpuWrite(uint16_t address, uint8_t data)
{
    switch (address)
    {
    case 0x4000:
        break;
    case 0x4001:
        break;
    case 0x4002:
        break;
    case 0x4003:
        break;
    case 0x4004:
        break;
    case 0x4005:
        break;
    case 0x4006:
        break;
    case 0x4007:
        break;
    case 0x4008:
        break;
    case 0x400A:
        break;
    case 0x400B:
        break;
    case 0x400C:
        break;
    case 0x400D:
        break;
    case 0x400F:
        break;
    case 0x4010:
        break;
    case 0x4011:
        break;
    case 0x4012:
        break;
    case 0x4013:
        break;
    case 0x4015:
        break;
    case 0x4017:
        break;
    }
}
