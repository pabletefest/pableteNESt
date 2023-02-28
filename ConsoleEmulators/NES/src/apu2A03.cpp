#include "apu2A03.h"
#include <vector>

namespace nes
{
    static constexpr uint8_t lengthCounterTable[32] = {10, 254, 20, 2, 40, 4, 80, 6, 160, 8, 60, 10, 14, 12, 26, 14,
                                      12, 16, 24, 18, 48, 20, 96, 22, 192, 24, 72, 26, 16, 28, 32, 30};

    static const std::vector<uint16_t> stepSequenceModeTables[2] = { {3728, 7456, 11185, 14914}, {3728, 7456, 11185, 14914, 18640} };
}

nes::APU::APU()
{
    pulse1Sequencer.pulseId = PULSE1;
    pulse2Sequencer.pulseId = PULSE2;
}

void nes::APU::reset()
{
    elapsedCycles = 0;
    pulse1Sequencer.pulseId = PULSE1;
    pulse2Sequencer.pulseId = PULSE2;
    noiseChannelLFSR.shiftRegister = 0x0001;
    cpuWrite(0x4015, 0x00); // Power-up and reset have the effect of writing $00, silencing all channels.
}

void nes::APU::clock()
{
    triangleSequencer.clock((triangleLengthCounter.internalCounter > 0 && triangleLinearCounter.internalCounter > 0));

    if (elapsedCycles % 2 == 0) // Every other cpu cycle
    {
        pulse1Sequencer.clock();
        pulse2Sequencer.clock();
        noiseChannelLFSR.clock();

        elapsedFrameCounterCycles++;

        if (elapsedFrameCounterCycles == stepSequenceModeTables[frameCounter.mode][0]) // 3728
        {
            pulse1Envelope.clock();
            pulse2Envelope.clock();
            noiseEnvelope.clock();

            triangleLinearCounter.clock(triangleSequencer.enabled);
        }

        if (elapsedFrameCounterCycles == stepSequenceModeTables[frameCounter.mode][1]) // 7456
        {
            pulse1LengthCounter.clock(pulse1Sequencer.enabled);
            pulse2LengthCounter.clock(pulse1Sequencer.enabled);
            triangleLengthCounter.clock(triangleSequencer.enabled);
            noiseLengthCounter.clock(noiseChannelLFSR.enabled);

            pulse1Sequencer.pulseSweeper.clock();
            pulse2Sequencer.pulseSweeper.clock();

            pulse1Envelope.clock();
            pulse2Envelope.clock();
            noiseEnvelope.clock();

            triangleLinearCounter.clock(triangleSequencer.enabled);
        }

        if (elapsedFrameCounterCycles == stepSequenceModeTables[frameCounter.mode][2]) // 11185
        {
            pulse1Envelope.clock();
            pulse2Envelope.clock();
            noiseEnvelope.clock();

            triangleLinearCounter.clock(triangleSequencer.enabled);
        }

        if (elapsedFrameCounterCycles == stepSequenceModeTables[frameCounter.mode][3]) // 14914
        {
            if (frameCounter.mode == 0) // Only in mode 0: 4-step
            {
                pulse1LengthCounter.clock(pulse1Sequencer.enabled);
                pulse2LengthCounter.clock(pulse1Sequencer.enabled);
                triangleLengthCounter.clock(triangleSequencer.enabled);
                noiseLengthCounter.clock(noiseChannelLFSR.enabled);

                pulse1Sequencer.pulseSweeper.clock();
                pulse2Sequencer.pulseSweeper.clock();

                pulse1Envelope.clock();
                pulse2Envelope.clock();
                noiseEnvelope.clock();

                triangleLinearCounter.clock(triangleSequencer.enabled);

                elapsedFrameCounterCycles = 0;
            }
        }

        // Only in mode 1: 5-step
        if (frameCounter.mode)
        {
            if (elapsedFrameCounterCycles == stepSequenceModeTables[frameCounter.mode][4]) // 18640
            {
                pulse1LengthCounter.clock(pulse1Sequencer.enabled);
                pulse2LengthCounter.clock(pulse1Sequencer.enabled);
                triangleLengthCounter.clock(triangleSequencer.enabled);
                noiseLengthCounter.clock(noiseChannelLFSR.enabled);

                pulse1Sequencer.pulseSweeper.clock();
                pulse2Sequencer.pulseSweeper.clock();

                pulse1Envelope.clock();
                pulse2Envelope.clock();
                noiseEnvelope.clock();

                triangleLinearCounter.clock(triangleSequencer.enabled);

                elapsedFrameCounterCycles = 0;

                if (frameCounter.interruptInhibitFlag == 0)
                    irq = true;
            }
        }
    }

    // This is done always regardless of SweepUnit being disabled or its divider not outputting a clock signal
    pulse1Sequencer.pulseSweeper.calculateTargetPeriod();
    pulse2Sequencer.pulseSweeper.calculateTargetPeriod();

    elapsedCycles++;
}

float nes::APU::getOutputAPU() const
{
    float output = 0.0f;
    float pulse_out = 0.0f;
    float tnd_out = 0.0f;

    uint8_t pulse1Output = (pulse1LengthCounter.internalCounter > 0 && pulse1Sequencer.pulseTimer > 8) ? pulse1Sequencer.output(pulse1Envelope) : 0;
    uint8_t pulse2Output = (pulse2LengthCounter.internalCounter > 0 && pulse2Sequencer.pulseTimer > 8) ? pulse2Sequencer.output(pulse2Envelope) : 0;

    uint8_t pulseChannelsOutput = pulse1Output + pulse2Output;

    if (pulseChannelsOutput > 0)
    {
        pulse_out = 95.88f / ((8128 / pulseChannelsOutput) + 100);
    }

    uint8_t triangleOutput = triangleSequencer.output(); // if (triangleLengthCounter.internalCounter > 0 && triangleLinearCounter.internalCounter > 0), the channel is not silenced, otherwise output is 0 
    uint8_t noiseOutput = (noiseLengthCounter.internalCounter > 0) ? noiseChannelLFSR.output(noiseEnvelope) : 0;
    uint8_t dmcOutput = 127; // Temporal Stub

    float tndChannelsOutput = ((float)triangleOutput / 8227) + ((float)noiseOutput / 12241) + ((float)dmcOutput / 22638);

    if (tndChannelsOutput > 0)
    {
        tnd_out = 159.79f / ((1 / tndChannelsOutput) + 100);
    }

    output = pulse_out + tnd_out;

    return output;
}

uint8_t nes::APU::cpuRead(uint16_t address)
{
    uint8_t dataRead = 0x00;

    if (address == 0x4015)
    {
        dataRead |= (pulse1LengthCounter.internalCounter > 0) ? 0x01 : 0x00;
        dataRead |= (pulse2LengthCounter.internalCounter > 0) ? 0x02 : 0x00;
        dataRead |= irq ? 0x40 : 0x00;
        irq = false;
    }

    return dataRead;
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
        pulse1LengthCounter.haltFlag = (data & 0x20) > 0;
        pulse1Envelope.loopFlag = (data & 0x20) > 0;
        pulse1Envelope.constantVolumeFlag = (data & 0x10) > 0;
        pulse1Envelope.envelopeDividerPeriodVolume = data & 0x0F;
        break;
    case 0x4001:
        pulse1Sequencer.pulseSweeper.enabled = (data & 0x80) > 0;
        pulse1Sequencer.pulseSweeper.divider = ((data & 0x70) >> 4) + 1;
        pulse1Sequencer.pulseSweeper.dividerPeriodReload = ((data & 0x70) >> 4) + 1;
        pulse1Sequencer.pulseSweeper.negateFlag = (data & 0x08) > 0;
        pulse1Sequencer.pulseSweeper.shiftCount = data & 0x07;
        break;
    case 0x4002:
        pulse1Sequencer.timerReload = ((0x0700 & pulse1Sequencer.timerReload) | (data & 0x00FF)); // Low 8 bits
        break;
    case 0x4003:
        pulse1Sequencer.timerReload = (((data & 0x07) << 8) | (pulse1Sequencer.timerReload & 0x00FF)); // High 3 bits
        //pulse1Sequencer.pulseTimer = pulse1Sequencer.timerReload;
        pulse1Sequencer.lengthCounterLoad = lengthCounterTable[(data >> 3)];
        pulse1LengthCounter.internalCounter = lengthCounterTable[(data >> 3)];
        pulse1Envelope.startFlag = true;
        break;
    case 0x4004:
        pulse2Sequencer.dutyCycleTable = (data >> 6);
        pulse2LengthCounter.haltFlag = (data & 0x20) > 0;
        pulse2Envelope.loopFlag = (data & 0x20) > 0;
        pulse2Envelope.constantVolumeFlag = (data & 0x10) > 0;
        pulse2Envelope.envelopeDividerPeriodVolume = data & 0x0F;
        break;
    case 0x4005:
        pulse2Sequencer.pulseSweeper.enabled = (data & 0x80) > 0;
        pulse2Sequencer.pulseSweeper.divider = ((data & 0x70) >> 4) + 1;
        pulse2Sequencer.pulseSweeper.dividerPeriodReload = ((data & 0x70) >> 4) + 1;
        pulse2Sequencer.pulseSweeper.negateFlag = (data & 0x08) > 0;
        pulse2Sequencer.pulseSweeper.shiftCount = data & 0x07;
        break;
    case 0x4006:
        pulse2Sequencer.timerReload = ((0x0700 & pulse2Sequencer.timerReload) | (data & 0x00FF)); // Low 8 bits
        break;
    case 0x4007:
        pulse2Sequencer.timerReload = (((data & 0x07) << 8) | (pulse2Sequencer.timerReload & 0x00FF)); // High 3 bits
        pulse2Sequencer.lengthCounterLoad = lengthCounterTable[(data >> 3)];
        pulse2LengthCounter.internalCounter = lengthCounterTable[(data >> 3)];
        pulse2Envelope.startFlag = true;
        break;
    case 0x4008:
        triangleLengthCounter.haltFlag = (data & 0x80) > 0;
        triangleLinearCounter.controlFlag = (data & 0x80) > 0;
        triangleLinearCounter.counterReloadValue = data & 0x7F;
        break;
    case 0x400A:
        triangleSequencer.timerReload = ((0x0700 & triangleSequencer.timerReload) | (data & 0x00FF)); // Low 8 bits     
        break;
    case 0x400B:
        triangleSequencer.timerReload = (((data & 0x07) << 8) | (triangleSequencer.timerReload & 0x00FF)); // High 3 bits
        triangleLengthCounter.internalCounter = lengthCounterTable[(data >> 3)];
        triangleLinearCounter.reloadFlag = true;
        break;
    case 0x400C:
        noiseLengthCounter.haltFlag = (data & 0x20) > 0;
        noiseEnvelope.loopFlag = (data & 0x20) > 0;
        noiseEnvelope.constantVolumeFlag = (data & 0x10) > 0;
        noiseEnvelope.envelopeDividerPeriodVolume = data & 0x0F;
        break;
    case 0x400D:
        break;
    case 0x400E:
        noiseChannelLFSR.modeFlag = (data & 0x80) > 0;
        noiseChannelLFSR.timer = noisePeriodsTable[data & 0x0F];
        noiseChannelLFSR.timerReload = noisePeriodsTable[data & 0x0F]; // To reload the timer when reaching 0
        break;
    case 0x400F:
        noiseLengthCounter.internalCounter = lengthCounterTable[(data >> 3)];
        noiseEnvelope.startFlag = true;
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
        pulse1Sequencer.enabled = (data & 0x01) > 0;
        if (!pulse1Sequencer.enabled)
            pulse1LengthCounter.internalCounter = 0;

        pulse2Sequencer.enabled = (data & 0x02) > 0;
        if (!pulse2Sequencer.enabled)
            pulse2LengthCounter.internalCounter = 0;

        triangleSequencer.enabled = (data & 0x04) > 0;
        if (!triangleSequencer.enabled)
            triangleLengthCounter.internalCounter = 0;

        noiseChannelLFSR.enabled = (data & 0x08) > 0;
        if (!noiseChannelLFSR.enabled)
            noiseLengthCounter.internalCounter = 0;
        break;
    case 0x4017:
        frameCounter.mode = data >> 7;
        frameCounter.interruptInhibitFlag = (data & 0x40) >> 6;

        if (frameCounter.interruptInhibitFlag)
            irq = false;
        break;
    }
}
