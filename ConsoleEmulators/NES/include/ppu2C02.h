#pragma once
#include "cartridge.h"

#include <memory>
#include <vector>

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

private:
    std::shared_ptr<Cartridge> cartridge;

    int32_t scanline = 0;
    int32_t cycle = 0;

    std::vector<Pixel> pixelsFrameBufer;

    Pixel nesPalToRGBAPalArray[0x40]; // 64 colours
};