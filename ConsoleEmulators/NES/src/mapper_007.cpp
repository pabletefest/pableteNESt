#include "Mapper_007.h"
#include "cartridge.h"

nes::Mapper_007::Mapper_007(uint8_t prgBanks, uint8_t chrBanks, nes::Cartridge& cart)
    : Mapper(prgBanks, chrBanks), cartRef(cart)
{
    //prgRomBankSelected = ((nPRGBanks / 2) - 1);
}

bool nes::Mapper_007::cpuMapRead(uint16_t addr, uint32_t& mapped_addr)
{
    if (addr >= 0x8000 && addr <= 0xFFFF)
    {
        mapped_addr = (prgRomBankSelected % (nPRGBanks / 2)) * 0x8000 + (addr & 0x7FFF);

        return true;
    }

    return false;
}

bool nes::Mapper_007::cpuMapWrite(uint16_t addr, uint32_t& mapped_addr, uint8_t data)
{
    if (addr >= 0x8000 && addr <= 0xFFFF)
    {
        prgRomBankSelected = data & 0x07;

        if (data & 0x10) // Check VRAM page selected
            cartRef.setNTMirroring(Cartridge::Mirroring::ONE_SCREEN_UPPER_BANK);
        else
            cartRef.setNTMirroring(Cartridge::Mirroring::ONE_SCREEN_LOWER_BANK);

        return false;
    }

    return false;
}

bool nes::Mapper_007::ppuMapRead(uint16_t addr, uint32_t& mapped_addr)
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

bool nes::Mapper_007::ppuMapWrite(uint16_t addr, uint32_t& mapped_addr)
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
