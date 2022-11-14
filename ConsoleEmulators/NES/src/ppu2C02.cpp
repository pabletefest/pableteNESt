#include "ppu2C02.h"

constexpr uint16_t baseNameTableAddresses[4] = { 0x2000, 0x2400, 0x2800, 0x2C00 };

PPU::PPU() 
    : pixelsFrameBufer(std::vector<Pixel>(341 * 262, Pixel()))
{
    init();

    nesPalToRGBAPalArray[0x00] = Pixel{ 255, 84, 84, 84 };
    nesPalToRGBAPalArray[0x01] = Pixel{ 255, 0, 30, 116 };
    nesPalToRGBAPalArray[0x02] = Pixel{ 255, 8, 16, 144 };
    nesPalToRGBAPalArray[0x03] = Pixel{ 255, 48, 0, 136 };
    nesPalToRGBAPalArray[0x04] = Pixel{ 255, 68, 0, 100 };
    nesPalToRGBAPalArray[0x05] = Pixel{ 255, 92, 0, 48 };
    nesPalToRGBAPalArray[0x06] = Pixel{ 255, 84, 4, 0 };
    nesPalToRGBAPalArray[0x07] = Pixel{ 255, 60, 24, 0 };
    nesPalToRGBAPalArray[0x08] = Pixel{ 255, 32, 42, 0 };
    nesPalToRGBAPalArray[0x09] = Pixel{ 255, 8, 58, 0 };
    nesPalToRGBAPalArray[0x0A] = Pixel{ 255, 0, 64, 0 };
    nesPalToRGBAPalArray[0x0B] = Pixel{ 255, 0, 60, 0 };
    nesPalToRGBAPalArray[0x0C] = Pixel{ 255, 0, 50, 60 };
    nesPalToRGBAPalArray[0x0D] = Pixel{ 255, 0, 0, 0 };
    nesPalToRGBAPalArray[0x0E] = Pixel{ 255, 0, 0, 0 };
    nesPalToRGBAPalArray[0x0F] = Pixel{ 255, 0, 0, 0 };

    nesPalToRGBAPalArray[0x10] = Pixel{ 255, 152, 150, 152 };
    nesPalToRGBAPalArray[0x11] = Pixel{ 255, 8, 76, 196 };
    nesPalToRGBAPalArray[0x12] = Pixel{ 255, 48, 50, 236 };
    nesPalToRGBAPalArray[0x13] = Pixel{ 255, 92, 30, 228 };
    nesPalToRGBAPalArray[0x14] = Pixel{ 255, 136, 20, 176 };
    nesPalToRGBAPalArray[0x15] = Pixel{ 255, 160, 20, 100 };
    nesPalToRGBAPalArray[0x16] = Pixel{ 255, 152, 34, 32 };
    nesPalToRGBAPalArray[0x17] = Pixel{ 255, 120, 60, 0 };
    nesPalToRGBAPalArray[0x18] = Pixel{ 255, 84, 90, 0 };
    nesPalToRGBAPalArray[0x19] = Pixel{ 255, 40, 114, 0 };
    nesPalToRGBAPalArray[0x1A] = Pixel{ 255, 8, 124, 0 };
    nesPalToRGBAPalArray[0x1B] = Pixel{ 255, 0, 118, 40 };
    nesPalToRGBAPalArray[0x1C] = Pixel{ 255, 0, 102, 120 };
    nesPalToRGBAPalArray[0x1D] = Pixel{ 255, 0, 0, 0 };
    nesPalToRGBAPalArray[0x1E] = Pixel{ 255, 0, 0, 0 };
    nesPalToRGBAPalArray[0x1F] = Pixel{ 255, 0, 0, 0 };

    nesPalToRGBAPalArray[0x20] = Pixel{ 255, 236, 238, 236 };
    nesPalToRGBAPalArray[0x21] = Pixel{ 255, 76, 154, 236 };
    nesPalToRGBAPalArray[0x22] = Pixel{ 255, 120, 124, 236 };
    nesPalToRGBAPalArray[0x23] = Pixel{ 255, 176, 98, 236 };
    nesPalToRGBAPalArray[0x24] = Pixel{ 255, 228, 84, 236 };
    nesPalToRGBAPalArray[0x25] = Pixel{ 255, 236, 88, 180 };
    nesPalToRGBAPalArray[0x26] = Pixel{ 255, 236, 106, 100 };
    nesPalToRGBAPalArray[0x27] = Pixel{ 255, 212, 136, 32 };
    nesPalToRGBAPalArray[0x28] = Pixel{ 255, 160, 170, 0 };
    nesPalToRGBAPalArray[0x29] = Pixel{ 255, 116, 196, 0 };
    nesPalToRGBAPalArray[0x2A] = Pixel{ 255, 76, 208, 32 };
    nesPalToRGBAPalArray[0x2B] = Pixel{ 255, 56, 204, 108 };
    nesPalToRGBAPalArray[0x2C] = Pixel{ 255, 56, 180, 204 };
    nesPalToRGBAPalArray[0x2D] = Pixel{ 255, 60, 60, 60 };
    nesPalToRGBAPalArray[0x2E] = Pixel{ 255, 0, 0, 0 };
    nesPalToRGBAPalArray[0x2F] = Pixel{ 255, 0, 0, 0 };

    nesPalToRGBAPalArray[0x30] = Pixel{ 255, 236, 238, 236 };
    nesPalToRGBAPalArray[0x31] = Pixel{ 255, 168, 204, 236 };
    nesPalToRGBAPalArray[0x32] = Pixel{ 255, 188, 188, 236 };
    nesPalToRGBAPalArray[0x33] = Pixel{ 255, 212, 178, 236 };
    nesPalToRGBAPalArray[0x34] = Pixel{ 255, 236, 174, 236 };
    nesPalToRGBAPalArray[0x35] = Pixel{ 255, 236, 174, 212 };
    nesPalToRGBAPalArray[0x36] = Pixel{ 255, 236, 180, 176 };
    nesPalToRGBAPalArray[0x37] = Pixel{ 255, 228, 196, 144 };
    nesPalToRGBAPalArray[0x38] = Pixel{ 255, 204, 210, 120 };
    nesPalToRGBAPalArray[0x39] = Pixel{ 255, 180, 222, 120 };
    nesPalToRGBAPalArray[0x3A] = Pixel{ 255, 168, 226, 144 };
    nesPalToRGBAPalArray[0x3B] = Pixel{ 255, 152, 226, 180 };
    nesPalToRGBAPalArray[0x3C] = Pixel{ 255, 160, 214, 228 };
    nesPalToRGBAPalArray[0x3D] = Pixel{ 255, 160, 162, 160 };
    nesPalToRGBAPalArray[0x3E] = Pixel{ 255, 0, 0, 0 };
    nesPalToRGBAPalArray[0x3F] = Pixel{ 255, 0, 0, 0 };
}

void PPU::connectCartridge(const std::shared_ptr<Cartridge>& cart)
{
    this->cartridge = cart;
}

void PPU::reset()
{
    PPUCTRL.controlReg = 0x00;
    PPUMASK.maskReg = 0x00;
}

void PPU::clock()
{
    if (cycle == 1 && scanline == -1)
    {
        PPUSTATUS.verticalBlank = 0;
        PPUSTATUS.spriteZeroHit = 0;
        PPUSTATUS.spriteOverflow = 0;
    }

    if (cycle == 1 && scanline == 241)
    {
        PPUSTATUS.verticalBlank = 1;

        if (PPUCTRL.enableNMI)
            nmi = true;
    }


    // ----- TESTING PORPOSE -----
    if (scanline == -1) scanline++;
    
    pixelsFrameBufer[scanline * 341 + cycle] = nesPalToRGBAPalArray[rand() % 0x3F];

    // ----- END OF TESTING ------

    cycle++;

    if (cycle >= 341)
    {
        cycle = 0;
        scanline++;

        if (scanline >= 261)
        {
            scanline = -1;
            frameCompleted = true;
            framesElapsed++;
        }
    }

    totalCycles++;
}

uint8_t PPU::cpuRead(uint16_t address)
{
    uint8_t dataRead = 0x00;

    switch (address)
    {
    case 0x0000: // Control
        break;
    case 0x0001: // Mask
        break;
    case 0x0002: // Status
        dataRead = PPUSTATUS.statusReg;
        PPUSTATUS.verticalBlank = 0;

        break;
    case 0x0003: // OAM Address
        break;
    case 0x0004: // OAM Data
        break;
    case 0x0005: // Scroll
        break;
    case 0x0006: // PPU Address
        break;
    case 0x0007: // PPU Data
        break;
    }

    return dataRead;
}

void PPU::cpuWrite(uint16_t address, uint8_t data)
{
    switch (address)
    {
    case 0x0000: // Control
        PPUCTRL.controlReg = data;
        loopyV.coarseXScroll = PPUCTRL.nametableX;
        loopyV.coarseYScroll = PPUCTRL.nametableY;
        break;
    case 0x0001: // Mask
        PPUMASK.maskReg = data;
        break;
    case 0x0002: // Status
        break;
    case 0x0003: // OAM Address
        break;
    case 0x0004: // OAM Data
        break;
    case 0x0005: // Scroll
        break;
    case 0x0006: // PPU Address
        break;
    case 0x0007: // PPU Data
        break;
    }
}

uint8_t PPU::ppuRead(uint16_t address)
{
    uint8_t dataRead = 0x00;
    address &= 0x3FFF; // Masking address to avoid going beyond the PPU addressable range

    if (cartridge->ppuRead(address, dataRead))
    {

    }

    return dataRead;
}

void PPU::ppuWrite(uint16_t address, uint8_t data)
{
    address &= 0x3FFF; // Masking address to avoid going beyond the PPU addressable range

    if (cartridge->ppuWrite(address, data))
    {

    }
}

void PPU::init()
{
    PPUCTRL.controlReg = 0x00;
    PPUMASK.maskReg = 0x00;
    PPUSTATUS.statusReg = 0xA0;
}
