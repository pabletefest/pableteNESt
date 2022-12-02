#pragma once
#include "cartridge.h"

#include <memory>
#include <vector>

namespace nes
{
    // PPU 2C02 (Picture Processing Unit) of the NES console
    class PPU
    {
    public:

        struct Pixel
        {
            uint8_t A;
            uint8_t B;
            uint8_t G;
            uint8_t R;
        };

        PPU();
        ~PPU() = default;

        void connectCartridge(const std::shared_ptr<Cartridge>& cart);

        void reset();
        void clock(); // PPU tick

        // Communications with Main Bus
        uint8_t cpuRead(uint16_t address);
        void    cpuWrite(uint16_t address, uint8_t  data);

        // Communications with PPU Bus
        uint8_t ppuRead(uint16_t address);
        void    ppuWrite(uint16_t address, uint8_t data);

        bool nmi = false;
        bool frameCompleted = false;

        const std::vector<Pixel>& getPixelsFrameBuffer() const
        {
            return pixelsFrameBufer;
        }

        PPU::Pixel getRGBAFromNesPalette(uint8_t paletteIndex, uint8_t pixelIndex);

        std::vector<PPU::Pixel> getPatternTableBuffer(uint8_t patternIndex, uint8_t paletteIndex);

        // If selectNT is -1, a pointer the both NT is returned
        void* getNametable(int8_t selectNT);

    private:
        void init();

    private:
        std::shared_ptr<Cartridge> cartridge;

        uint8_t nameTables[2][1024];
        uint8_t paletteRam[32];

        int32_t scanline = 0;
        int32_t cycle = 0; // Scanline cycle (reset after each scanline)

        uint32_t framesElapsed = 0;
        uint32_t totalCycles = 0;

        std::vector<Pixel> pixelsFrameBufer;

        Pixel nesPalToRGBAPalArray[0x40]; // 64 colours

        // REGISTERS

        union
        {
            struct
            {
                // Bits 0 and 1 can be baseNameTabAddreses
                uint8_t nametableX : 1;
                uint8_t nametableY : 1;
                uint8_t vramAddrInc : 1;
                uint8_t sprPatternTabAddr : 1;
                uint8_t backgroundPatternTabAddr : 1;
                uint8_t sprSize : 1;
                uint8_t ppuMasterSlaveSelect : 1;
                uint8_t enableNMI : 1;
            };

            uint8_t controlReg; //Used to represent struct as uint8_t and move it around
        }PPUCTRL;

        union
        {
            struct
            {
                uint8_t greyscale : 1;
                uint8_t bgLeftmostScreen : 1;
                uint8_t sprLeftmostScreen : 1;
                uint8_t showBackground : 1;
                uint8_t showSprites : 1;
                uint8_t emphasizeRed : 1;
                uint8_t emphasizeGreen : 1;
                uint8_t emphasizeBlue : 1;
            };

            uint8_t maskReg; //Used to represent struct as uint8_t and move it around
        }PPUMASK;

        union
        {
            struct
            {
                uint8_t unused : 5;
                uint8_t spriteOverflow : 1;
                uint8_t spriteZeroHit : 1;
                uint8_t verticalBlank : 1;
            };

            uint8_t statusReg; //Used to represent struct as uint8_t and move it around
        }PPUSTATUS;

        union loopy_register
        {
            struct
            {
                uint16_t coarseXScroll : 5;
                uint16_t coarseYScroll : 5;
                uint16_t nametableSelectX : 1;
                uint16_t nametableSelectY : 1;
                uint16_t fineYScroll : 3;
                uint16_t unused : 1;
            };
            uint16_t vramAddrPtr;
        };

        // Internal PPU registers (yyyNNYYYYYXXXXX format)
        loopy_register loopyV; // (V)ram Address (v register)
        loopy_register loopyT; // (T)emporary Address (t register)
        uint8_t fineXScroll = 0x00; 
        bool addressLatchToggle = false; // (w register)

        uint8_t internalReadBuffer = 0x00;

        uint16_t low_pattern_shifter = 0x0000;
        uint16_t high_pattern_shifter = 0x0000;
        uint8_t low_attribute_shifter = 0x00;
        uint8_t high_attribute_shifter = 0x00;
    };
}