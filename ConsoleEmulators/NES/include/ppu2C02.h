#pragma once
#include "cartridge.h"

#include <memory>

// PPU 2C02 (Picture Processing Unit) of the NES console
class PPU
{
public:
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

private:
    std::shared_ptr<Cartridge> cartridge;
};