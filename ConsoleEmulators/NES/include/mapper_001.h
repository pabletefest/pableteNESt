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

	private:
		void bankSwitchingProcess(uint16_t destRegAddr);

	private:

		//Registers
		uint8_t dataShiftRegister = 0x00; // When writting 5 times to Load Register (when bit 7 is 0 and not resetting), data is being put here shifting on each write
		
		union
		{
			struct
			{
				uint8_t mirroring : 2;
				uint8_t prgROMBankMode : 2;
				uint8_t chrROMBankMode : 1;
				uint8_t unused : 3;
			};

			uint8_t reg;
		}CONTROL;

		union
		{
			struct
			{
				uint8_t prgROMBankSelect : 4;
				uint8_t prgRAMChipEnable : 1;
				uint8_t unused : 3;
			};

			uint8_t reg;
		}PRGBANK;
	};
}