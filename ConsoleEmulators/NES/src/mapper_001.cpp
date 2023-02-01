#include "mapper_001.h"
#include "cartridge.h"
#include "emu_typedefs.h"

nes::Mapper_001::Mapper_001(uint8_t prgBanks, uint8_t chrBanks, bool batteryBackedPersistentMem, nes::Cartridge& cart)
	: Mapper(prgBanks, chrBanks, batteryBackedPersistentMem), cartRef(cart)
{
	CONTROL.reg = 0x00;
	CONTROL.prgROMBankMode = 3;
	PRGBANK.reg = 0x00;

	//if (containsBatteryMemory)
	//	cartPersitentRAM.resize(8192); // 8KB RAM
}

bool nes::Mapper_001::cpuMapRead(uint16_t addr, uint32_t& mapped_addr)
{
	if (addr >= 0x8000 && addr <= 0xFFFF)
	{
		if (CONTROL.prgROMBankMode < 2) // Low bit ignored in 32KB mode
		{
			mapped_addr = ((PRGBANK.prgROMBankSelect >> 1) * 0x8000) + addr & 0x7FFF;
			return true;
		}

		if (addr >= 0x8000 && addr <= 0xBFFF)
		{
			if (CONTROL.prgROMBankMode == 2) // fix first bank at $8000 and switch 16 KB bank at $C000
			{
				mapped_addr = 0x0000 + (addr & 0x3FFF);
			}
			else if (CONTROL.prgROMBankMode == 3) // fix last bank at $C000 and switch 16 KB bank at $8000
			{
				mapped_addr = (PRGBANK.prgROMBankSelect * 0x4000) + (addr & 0x3FFF);
			}
		}
		else if (addr >= 0xC000 && addr <= 0xFFFF)
		{
			if (CONTROL.prgROMBankMode == 2) // fix first bank at $8000 and switch 16 KB bank at $C000
			{
				mapped_addr = (PRGBANK.prgROMBankSelect * 0x4000) + (addr & 0x3FFF);
			}
			else if (CONTROL.prgROMBankMode == 3) // fix last bank at $C000 and switch 16 KB bank at $8000
			{
				mapped_addr = ((nPRGBanks - 1) * 0x4000) + (addr & 0x3FFF);
			}
		}

		return true;
	}

	if (addr >= 0x6000 && addr <= 0x7FFF)
	{
		mapped_addr = (nPRGBanks * convertKBToBytes<uint32_t>(16)) + (addr & 0x1FFF);
		return true;
	}

	return false;
}

bool nes::Mapper_001::cpuMapWrite(uint16_t addr, uint32_t& mapped_addr, uint8_t data)
{
	if (addr >= 0x8000 && addr <= 0xFFFF)
	{
		if (data & 0x80) // Reset mapper if bit 7 is set
		{
			resetMapper();
			return false;
		}

		dataShiftRegister = (dataShiftRegister | ((data & 0x01) << 5)) >> 1;
		currentLRWrite++;

		if (currentLRWrite == 5) // We reached the important last write
		{
			configRegisterAt(addr);
			currentLRWrite = 0;
			dataShiftRegister = 0x00;
		}

		return false;
	}
	
	if (addr >= 0x6000 && addr <= 0x7FFF)
	{
		//cartPersitentRAM[addr & 0x1FFF] = data;
		mapped_addr = (nPRGBanks * convertKBToBytes<uint32_t>(16)) + (addr & 0x1FFF);
		return true;
	}
	
	return false;
}

bool nes::Mapper_001::ppuMapRead(uint16_t addr, uint32_t& mapped_addr)
{
	if (addr >= 0x0000 && addr <= 0x1FFF)
	{
		if (nCHRBanks == 0)
		{
			mapped_addr = addr & 0x1FFF;
			return true;
		}

		if (CONTROL.chrROMBankMode == 0) // 8KB mode
		{
			//mapped_addr = addr & 0x1FFF;
			mapped_addr = ((chrBank0Select >> 1) * 0x2000) + (addr & 0x1FFF);
		}
		else // Separate 4KB mode
		{
			if (addr >= 0x0000 && addr <= 0x0FFF)
			{
				mapped_addr = (chrBank0Select * 0x2000) + (addr & 0x0FFF); // First 4KB of 8KB bank
			}
			else if (addr >= 0x1000 && addr <= 0x1FFF)
			{
				mapped_addr = (chrBank1Select * 0x2000) + 0x1000 + (addr & 0x0FFF); // Second 4KB of 8KB bank
			}
		}

		return true;
	}

	return false;
}

bool nes::Mapper_001::ppuMapWrite(uint16_t addr, uint32_t& mapped_addr)
{
	if (addr >= 0x0000 && addr <= 0x1FFF)
	{
		if (nCHRBanks == 0) // Treat as RAM
		{
			mapped_addr = addr & 0x1FFF;

			//if (CONTROL.chrROMBankMode == 0) // 8KB mode
			//{
			//	mapped_addr = addr & 0x1FFF;
			//}
			//else // Separate 4KB mode
			//{
			//	if (addr >= 0x0000 && addr <= 0x0FFF)
			//	{
			//		mapped_addr = (chrBank0Select * 0x1000) + (addr & 0x0FFF);
			//	}
			//	else if (addr >= 0x1000 && addr <= 0x1FFF)
			//	{
			//		mapped_addr = (chrBank1Select * 0x1000) + (addr & 0x1FFF);
			//	}
			//}

			return true;
		}
	}

	return false;
}

void nes::Mapper_001::bankSwitchingProcess(uint32_t destRegAddr)
{
}

uint32_t nes::Mapper_001::getMappedAddress() const
{
	return 0;
}

void nes::Mapper_001::resetMapper()
{
	CONTROL.prgROMBankMode = 3;
	currentLRWrite = 0;
	dataShiftRegister = 0x00;
}

void nes::Mapper_001::configRegisterAt(uint16_t address)
{
	if (address >= 0x8000 && address <= 0x9FFF)
	{
		CONTROL.reg = dataShiftRegister & 0x1F;

		setCartridgeMirroring();
	}
	else if (address >= 0xA000 && address <= 0xBFFF) // Select 4 KB or 8 KB CHR bank at PPU $0000 (low bit ignored in 8 KB mode)
	{
		//if (CONTROL.chrROMBankMode == 1)
		//{
		//	chrBank0Select = dataShiftRegister & 0x1F;
		//}
		//else
		//{
		//	chrBank0Select = (dataShiftRegister >> 1) & 0x0F; // Low bit ignored in 8KB mode
		//}

		chrBank0Select = dataShiftRegister & 0x1F;
	}
	else if (address >= 0xC000 && address <= 0xDFFF) // Select 4 KB CHR bank at PPU $1000 (ignored in 8 KB mode)
	{
		/*if (CONTROL.chrROMBankMode == 1)
		{
			chrBank1Select = dataShiftRegister & 0x1F;
		}*/
		chrBank1Select = dataShiftRegister & 0x1F;
	}
	else if (address >= 0xE000 && address <= 0xFFFF)
	{
		PRGBANK.reg = dataShiftRegister & 0x1F;
	}
}

void nes::Mapper_001::setCartridgeMirroring()
{	
	switch (CONTROL.mirroring)
	{
	case 0:
		cartRef.setNTMirroring(Cartridge::Mirroring::ONE_SCREEN_LOWER_BANK);
		break;
	case 1:
		cartRef.setNTMirroring(Cartridge::Mirroring::ONE_SCREEN_UPPER_BANK);
		break;
	case 2:
		cartRef.setNTMirroring(Cartridge::Mirroring::VERTICAL);
		break;
	case 3:
		cartRef.setNTMirroring(Cartridge::Mirroring::HORIZONTAL);
		break;
	default:
		// Can't reach the default state
		break;
	}
}
