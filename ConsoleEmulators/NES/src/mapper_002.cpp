#include "mapper_002.h"

namespace nes
{
	Mapper_002::Mapper_002(uint8_t prgBanks, uint8_t chrBanks)
		: Mapper(prgBanks, chrBanks)
	{
	}

	bool Mapper_002::cpuMapRead(uint16_t addr, uint32_t& mapped_addr)
	{	
		if (addr >= 0x8000 && addr <= 0xFFFF)
		{
			// Each PRG ROM bank is 16KB
			if (nPRGBanks == 1)
				mapped_addr = 0x0000;
			else if (addr >= 0xC000 && nPRGBanks > 1) // Last bank hard-wired from 0xC000 to 0xFFFF
				mapped_addr = (nPRGBanks - 1) * 0x4000;
			else // Bank switch from 0x8000 to 0xBFFF
				mapped_addr = bankNumber * 0x4000;

			mapped_addr += (addr & 0x3FFF);

			return true;
		}

		return false;
	}

	bool Mapper_002::cpuMapWrite(uint16_t addr, uint32_t& mapped_addr, uint8_t data)
	{
		if (addr >= 0x8000 && addr <= 0xFFFF)
		{
			// Obtain selected bank number
			bankNumber = (data & 0x0F);

			//Mapper 2 usually does not contain PRG RAM so this is placeholder code line.
			mapped_addr = addr & (nPRGBanks > 1 ? 0x7FFF : 0x3FFF);
			return false; // We don't want to write to PRG data
		}

		return false;
	}

	bool Mapper_002::ppuMapRead(uint16_t addr, uint32_t& mapped_addr)
	{
		// There is no mapping required for PPU
		// PPU Address Bus          CHR ROM/RAM
		// 0x0000 -> 0x1FFF: Map    0x0000 -> 0x1FFF
		if (addr >= 0x0000 && addr <= 0x1FFF)
		{
			mapped_addr = addr;
			return true;
		}

		return false;
	}

	bool Mapper_002::ppuMapWrite(uint16_t addr, uint32_t& mapped_addr)
	{
		if (addr >= 0x0000 && addr <= 0x1FFF)
		{
			if (nCHRBanks == 0)
			{
				// Treat as RAM
				mapped_addr = addr;
				return true;
			}
		}

		return false;
	}
}