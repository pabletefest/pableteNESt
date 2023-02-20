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
        pulse1Sequencer.clock();
    }

    elapsedCycles++;
}

uint8_t nes::APU::getOutputAPU() const
{
    double output = 0.0;
    double pulse_out = 0.0;
    double tnd_out = 0.0;

    uint8_t pulseChannelsOutput = pulse1Sequencer.output() + 0;

    if (pulseChannelsOutput > 0)
    {
        pulse_out = 95.88 / ((8128 / pulseChannelsOutput) + 100);
    }

    output = pulse_out + tnd_out;

    return output * 255;
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
        pulse1Sequencer.lengthCounterLoad = (data >> 3);
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
