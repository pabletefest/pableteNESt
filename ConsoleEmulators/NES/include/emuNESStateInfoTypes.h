#pragma once

namespace nes
{
    struct EmulatorStateInfo
    {
        struct
        {
            uint8_t regA;
            uint8_t regX;
            uint8_t regY;
            uint16_t PC;
            uint8_t SP;
            uint8_t status;
            uint8_t fetched;
            uint16_t effectiveAddr;
            uint16_t relativeAddr;
            uint8_t currentOpcode;
            uint8_t instructionCycles;
            uint64_t debugTotalCyclesElapsed;
        } CPUStateInfo;

        struct
        {
            bool nmi;
            bool frameCompleted;
            uint8_t nameTablesVRAM[2][1024];
            uint8_t paletteRAM[32];
            int32_t scanline;
            int32_t cycle;
            uint32_t framesElapsed;
            uint32_t totalCycles;
            uint32_t frameBuffer[256][240];
            uint8_t controlReg;
            uint8_t maskReg;
            uint8_t statusReg;
            uint16_t loppyVReg;
            uint16_t loppyTReg;
            uint8_t fineXScroll;
            bool addressLatchToggle;
            uint8_t internalReadBuffer;
            uint16_t low_pattern_shifter;
            uint16_t high_pattern_shifter;
            uint16_t low_attribute_shifter;
            uint16_t high_attribute_shifter;
            uint8_t fetchedByteNT;
            uint8_t fetchedByteAT;
            uint8_t fetchedLowBytePT;
            uint8_t fetchedHighBytePT;
            uint32_t OAMbuffer[64];
            uint8_t OAMaddr;
            uint32_t scanlineSecondaryOAM[8];
            uint8_t numSprFound;
            uint8_t fetchedSprY;
            uint8_t fetchedSprTileIndex;
            uint8_t spritesLowBytePatternShifters[8];
            uint8_t spritesHighBytePatternShifters[8];
            uint8_t spritesAttributesLatches[8];
            uint8_t spritesXpositionCounters[8];
            uint8_t spriteRenderingCounters[8];
        } PPUStateInfo;

        uint8_t ram[2048];
        uint64_t totalSystemClockCycles;
        uint8_t controllers[2];
        uint8_t controllersCache[2];
        uint8_t dmaPageHiAddr;
        uint8_t dmaInternalLoAddr;
        uint8_t dmaReadData;
        bool dmaTransferInterrupt;
        bool waitForEvenCycle;
    };
}
