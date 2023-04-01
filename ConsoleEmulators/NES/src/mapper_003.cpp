#include "mapper_003.h"

nes::Mapper_003::Mapper_003(uint8_t prgBanks, uint8_t chrBanks)
    : Mapper(prgBanks, chrBanks)
{
}

bool nes::Mapper_003::cpuMapRead(uint16_t addr, uint32_t& mapped_addr)
{
    if (addr >= 0x8000 && addr <= 0xFFFF)
    {
        mapped_addr = addr & (nPRGBanks > 1 ? 0x7FFF : 0x3FFF);

        return true;
    }

    return false;
}

bool nes::Mapper_003::cpuMapWrite(uint16_t addr, uint32_t& mapped_addr, uint8_t data)
{
    if (addr >= 0x8000 && addr <= 0xFFFF)
    {
        mapped_addr = addr & (nPRGBanks > 1 ? 0x7FFF : 0x3FFF);

        chrRomBankSelected = data & 0x03;

        return false; // No PRG RAM
    }

    return false;
}

bool nes::Mapper_003::ppuMapRead(uint16_t addr, uint32_t& mapped_addr)
{
    if (addr >= 0x0000 && addr <= 0x1FFF)
    {
        if (nCHRBanks == 0)
            mapped_addr = addr;
        else
            mapped_addr = (chrRomBankSelected % nCHRBanks) * 0x2000 + addr;

        return true;
    }

    return false;
}

bool nes::Mapper_003::ppuMapWrite(uint16_t addr, uint32_t& mapped_addr)
{
    if (addr >= 0x0000 && addr <= 0x1FFF)
    {
        if (nCHRBanks == 0)
        {
            mapped_addr = addr;
            return true;
        }
    }

    return false;
}
