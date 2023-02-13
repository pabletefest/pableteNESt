#pragma once
#include "mapper.h"

namespace nes
{
    class Mapper_066 : public Mapper
    {
	public:
		Mapper_066(uint8_t prgBanks, uint8_t chrBanks);
		~Mapper_066() override = default;

	public:
		bool cpuMapRead(uint16_t addr, uint32_t& mapped_addr) override;
		bool cpuMapWrite(uint16_t addr, uint32_t& mapped_addr, uint8_t data = 0x00) override;
		bool ppuMapRead(uint16_t addr, uint32_t& mapped_addr) override;
		bool ppuMapWrite(uint16_t addr, uint32_t& mapped_addr) override;

	private:
		uint8_t prgROMBankSelected = 0x00;
		uint8_t chrROMBankSelected = 0x00;
    };
}