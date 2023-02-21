#include "apu2A03.h"

namespace nes
{
    static uint8_t lengthCounterTable[32] = {10, 254, 20, 2, 40, 4, 80, 6, 160, 8, 60, 10, 14, 12, 26, 14,
                                      12, 16, 24, 18, 48, 20, 96, 22, 192, 24, 72, 26, 16, 28, 32, 30};
}

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
        pulse1Sequencer.clock();
        pulse2Sequencer.clock();
    }

    elapsedCycles++;
}

float nes::APU::getOutputAPU() const
{
    double output = 0.0;
    double pulse_out = 0.0;
    double tnd_out = 0.0;

    uint8_t pulseChannelsOutput = pulse1Sequencer.output() + pulse2Sequencer.output();

    if (pulseChannelsOutput > 0)
    {
        pulse_out = 95.88 / ((8128 / pulseChannelsOutput) + 100);
    }

    output = pulse_out + tnd_out;

    return output/* * 255*/;
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
        //switch (data >> 6)
        //{
        //case 0:
        //    pulse1Sequencer.sequence = 0b00000001; // 12.5%
        //    break;
        //case 1:
        //    pulse1Sequencer.sequence = 0b00000011; // 25%
        //    break;
        //case 2:
        //    pulse1Sequencer.sequence = 0b00001111; // 50%
        //    break;
        //case 3:
        //    pulse1Sequencer.sequence = 0b11111100; // 25% negated
        //    break;
        //}

        pulse1Sequencer.dutyCycleTable = (data >> 6);

        break;
    case 0x4001:
        break;
    case 0x4002:
        pulse1Sequencer.timerReload = ((0x0700 & pulse1Sequencer.timerReload) | (data & 0x00FF)); // Low 8 bits
        break;
    case 0x4003:
        pulse1Sequencer.timerReload = (((data & 0x07) << 8) | (pulse1Sequencer.timerReload & 0x00FF)); // High 3 bits
        //pulse1Sequencer.pulseTimer = pulse1Sequencer.timerReload;
        pulse1Sequencer.lengthCounterLoad = lengthCounterTable[(data >> 3)];
        break;
    case 0x4004:
        pulse2Sequencer.dutyCycleTable = (data >> 6);
        break;
    case 0x4005:
        break;
    case 0x4006:
        pulse2Sequencer.timerReload = ((0x0700 & pulse2Sequencer.timerReload) | (data & 0x00FF)); // Low 8 bits
        break;
    case 0x4007:
        pulse2Sequencer.timerReload = (((data & 0x07) << 8) | (pulse2Sequencer.timerReload & 0x00FF)); // High 3 bits
        pulse2Sequencer.lengthCounterLoad = lengthCounterTable[(data >> 3)];
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
