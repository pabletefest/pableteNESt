#pragma once
#include "mapper.h"

namespace nes
{
	class Cartridge;
}

namespace nes
{
    // Mapper 7 is the one used by the infamous "Battletoads", known to be the hardest game to emulate
    class Mapper_007 : public Mapper
    {
	public:
		Mapper_007(uint8_t prgBanks, uint8_t chrBanks, nes::Cartridge& cart);
		~Mapper_007() override = default;

	public:
		bool cpuMapRead(uint16_t addr, uint32_t& mapped_addr) override;
		bool cpuMapWrite(uint16_t addr, uint32_t& mapped_addr, uint8_t data = 0x00) override;
		bool ppuMapRead(uint16_t addr, uint32_t& mapped_addr) override;
		bool ppuMapWrite(uint16_t addr, uint32_t& mapped_addr) override;

	private:
		uint8_t prgRomBankSelected = 0x00;

		nes::Cartridge& cartRef;
	};
}