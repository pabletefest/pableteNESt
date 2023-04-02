#pragma once
#include "mapper.h"
#include <memory>
#include <vector>

namespace nes
{
	class Cartridge;
}

namespace nes
{
	// a.k.a Mapper MMC1
	class Mapper_001 : public Mapper
	{
	public:
		Mapper_001(uint8_t prgBanks, uint8_t chrBanks, bool batteryBackedPersistentMem, nes::Cartridge& cart);
		~Mapper_001() override = default;

	public:
		bool cpuMapRead(uint16_t addr, uint32_t& mapped_addr) override;
		bool cpuMapWrite(uint16_t addr, uint32_t& mapped_addr, uint8_t data = 0x00) override;
		bool ppuMapRead(uint16_t addr, uint32_t& mapped_addr) override;
		bool ppuMapWrite(uint16_t addr, uint32_t& mapped_addr) override;

	private:
		void bankSwitchingProcess(uint32_t destRegAddr);
		uint32_t getMappedAddress() const;
		void resetMapper();
		void configRegisterAt(uint16_t address);
		void setCartridgeMirroring();

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

		uint8_t chrBank0Select = 0x00;
		uint8_t chrBank1Select = 0x00;

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

		uint8_t currentLRWrite = 0x00;

		nes::Cartridge& cartRef;

		//std::vector<uint8_t> cartPersitentRAM;
	};
}