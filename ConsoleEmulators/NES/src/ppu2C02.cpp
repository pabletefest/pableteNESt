#include "ppu2C02.h"

PPU::PPU()
{
}

void PPU::connectCartridge(const std::shared_ptr<Cartridge>& cart)
{
    this->cartridge = cart;
}

void PPU::clock()
{
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
        break;
    case 0x0001: // Mask
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