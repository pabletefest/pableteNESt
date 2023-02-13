#include "mapper_066.h"

nes::Mapper_066::Mapper_066(uint8_t prgBanks, uint8_t chrBanks)
    : Mapper(prgBanks, chrBanks)
{
}

bool nes::Mapper_066::cpuMapRead(uint16_t addr, uint32_t& mapped_addr)
{
    if (addr >= 0x8000 && addr <= 0xFFFF)
    {
        mapped_addr = (prgROMBankSelected % nPRGBanks) * 0x8000 + (addr & 0x7FFF);
        return true;
    }

    return false;
}

bool nes::Mapper_066::cpuMapWrite(uint16_t addr, uint32_t& mapped_addr, uint8_t data)
{
    if (addr >= 0x8000 && addr <= 0xFFFF)
    {
        prgROMBankSelected = (data & 0x30) >> 4;
        chrROMBankSelected = data & 0x03;
        return false;
    }

    return false;
}

bool nes::Mapper_066::ppuMapRead(uint16_t addr, uint32_t& mapped_addr)
{
    if (addr >= 0x0000 && addr <= 0x1FFF)
    {
        if (nCHRBanks == 0)
            mapped_addr = addr;
        else
            mapped_addr = (chrROMBankSelected % nCHRBanks) * 0x2000 + addr;

        return true;
    }

    return false;
}

bool nes::Mapper_066::ppuMapWrite(uint16_t addr, uint32_t& mapped_addr)
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
