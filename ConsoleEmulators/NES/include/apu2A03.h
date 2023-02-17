#pragma once

namespace nes
{
    // Audio processor included in the 2A03 chip (CPU + APU)
    class APU
    {
    public:
        APU();
        ~APU() = default;

        void reset();
        void clock();

        // Communications with CPU
        uint8_t cpuRead(uint16_t address);
        void    cpuWrite(uint16_t address, uint8_t  data);

    private:
        uint64_t elapsedCycles = 0;

        struct PulseChannelParameters
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

        PulseChannelParameters pulse1Parameters;
    };
}