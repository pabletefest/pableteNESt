#pragma once
#include "mapper.h"

namespace nes
{
	// a.k.a Mapper MMC1
	class Mapper_001 : public Mapper
	{
	public:
		Mapper_001(uint8_t prgBanks, uint8_t chrBanks);
		~Mapper_001() override = default;

	public:
		bool cpuMapRead(uint16_t addr, uint32_t& mapped_addr) override;
		bool cpuMapWrite(uint16_t addr, uint32_t& mapped_addr, uint8_t data = 0x00) override;
		bool ppuMapRead(uint16_t addr, uint32_t& mapped_addr) override;
		bool ppuMapWrite(uint16_t addr, uint32_t& mapped_addr) override;
	};
}