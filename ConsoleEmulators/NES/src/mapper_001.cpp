#include "mapper_001.h"

nes::Mapper_001::Mapper_001(uint8_t prgBanks, uint8_t chrBanks)
	: Mapper(prgBanks, chrBanks)
{
}

bool nes::Mapper_001::cpuMapRead(uint16_t addr, uint32_t& mapped_addr)
{
	return false;
}

bool nes::Mapper_001::cpuMapWrite(uint16_t addr, uint32_t& mapped_addr, uint8_t data)
{
	return false;
}

bool nes::Mapper_001::ppuMapRead(uint16_t addr, uint32_t& mapped_addr)
{
	return false;
}

bool nes::Mapper_001::ppuMapWrite(uint16_t addr, uint32_t& mapped_addr)
{
	return false;
}

void nes::Mapper_001::bankSwitchingProcess(uint16_t destRegAddr)
{
}
