#pragma once

namespace nes
{
    static uint8_t dutyCyclePulseTables[4][8] = {
        {0, 0, 0, 0, 0, 0, 0, 1}, // 12.5%
        {0, 0, 0, 0, 0, 0, 1, 1}, // 25%
        {0, 0, 0, 0, 1, 1, 1, 1}, // 50%
        {1, 1, 1, 1, 1, 1, 0, 0}  // 25% negated
    };

    // Audio processor included in the 2A03 chip (CPU + APU)
    class APU
    {
    public:
        APU();
        ~APU() = default;

        void reset();
        void clock();

        uint8_t getOutputAPU() const;

        // Communications with CPU
        uint8_t cpuRead(uint16_t address);
        void    cpuWrite(uint16_t address, uint8_t  data);

    private:
        uint64_t elapsedCycles = 0;

        /*struct PulseChannelParameters
        {
            uint8_t dutyCycle : 2;
            uint8_t loopEnvelope_disableLengthCounter : 1;
            uint8_t constantVolumeFlag : 1;
            uint8_t envelopeDividerPeriod : 4;

            struct
            {
                uint8_t enabledFlag : 1;
                uint8_t dividerPeriod : 3;
                uint8_t negateFlag : 1;
                uint8_t shiftCount : 3;
            } sweepUnit;

            uint16_t pulseTimer : 11;
            uint8_t lengthCounterLoad : 5;
        };

        PulseChannelParameters pulse1Parameters;*/

        struct PulseSequencer
        {
            bool enabled = false;
            uint8_t dutyCycleTable = 0x00;
            //uint8_t sequence;
            uint16_t pulseTimer = 0x0000;
            uint16_t timerReload = 0x0000;
            uint8_t lengthCounterLoad = 0;
            uint8_t pulseChannelOutput = 0x00;

            void clock()
            {
                static uint8_t tableIndex = 0;

                if (pulseTimer > 0)
                {
                    pulseTimer--;
                    pulseChannelOutput = dutyCyclePulseTables[dutyCycleTable][tableIndex & 7];
                }
                else
                {
                    pulseTimer = timerReload;
                    tableIndex++;
                }
            }

            uint8_t output() const
            {
                return pulseChannelOutput * 15;
            }
        };

        PulseSequencer pulse1Sequencer;
    };
}