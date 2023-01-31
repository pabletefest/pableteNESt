#include "cartridge.h"
#include "mapper_000.h"
#include "mapper_001.h"
#include "mapper_002.h"
#include "emu_typedefs.h"

#include <fstream>

namespace nes
{
	Cartridge::Cartridge(const std::string& filename)
	{
		// iNES Format Header
		struct sHeader
		{
			char name[4];
			uint8_t prg_rom_chunks;
			uint8_t chr_rom_chunks;
			uint8_t mapper1;
			uint8_t mapper2;
			uint8_t prg_ram_size;
			uint8_t tv_system1;
			uint8_t tv_system2;
			char unused[5];
		} header;

		std::ifstream ifs;

		ifs.open(filename, std::ifstream::binary);

		if (ifs.is_open())
		{
			// Read file header
			ifs.read((char*)&header, sizeof(sHeader));

			// If a "trainer" exists we just need to read past
			// it before we get to the good stuff
			if (header.mapper1 & 0x04)
				ifs.seekg(512, std::ios_base::cur);

			// Determine Mapper ID
			nMapperID = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);
			mirroring = (header.mapper1 & 0x01) ? Mirroring::VERTICAL : Mirroring::HORIZONTAL;

			// "Discover" File Format
			uint8_t nFileType = 1;

			if (nFileType == 0)
			{

			}

			if (nFileType == 1)
			{
				nPRGBanks = header.prg_rom_chunks;
				vPRGMemory.resize(nPRGBanks * convertKBToBytes(16));
				ifs.read((char*)vPRGMemory.data(), vPRGMemory.size());

				nCHRBanks = header.chr_rom_chunks;
				vCHRMemory.resize(nCHRBanks * convertKBToBytes(8));
				ifs.read((char*)vCHRMemory.data(), vCHRMemory.size());
			}

			if (nFileType == 2)
			{

			}

			// Load appropriate mapper
			switch (nMapperID)
			{
			case 0:
				pMapper = std::make_shared<Mapper_000>(nPRGBanks, nCHRBanks);
				break;
			case 1:
				pMapper = std::make_shared<Mapper_001>(nPRGBanks, nCHRBanks, header.mapper1 & 0x02, *this);
				
				if (nCHRBanks == 0)
					vCHRMemory.resize(convertKBToBytes(8));

				if (header.mapper1 & 0x02)
				{
					vPRGMemory.resize(vPRGMemory.capacity() + 0x2000);
				}

				break;
			case 2:
				pMapper = std::make_shared<Mapper_002>(nPRGBanks, nCHRBanks);
				vCHRMemory.resize(convertKBToBytes(8));
				break;
			}

			ifs.close();
		}
	}

	bool Cartridge::cpuRead(uint16_t addr, uint8_t& data)
	{
		uint32_t mapped_addr = 0;

		if (pMapper->cpuMapRead(addr, mapped_addr))
		{
			data = vPRGMemory[mapped_addr];
			return true;
		}
		
		return false;
	}

	bool Cartridge::cpuWrite(uint16_t addr, uint8_t data)
	{
		uint32_t mapped_addr = 0;

		if (pMapper->cpuMapWrite(addr, mapped_addr, data))
		{
			vPRGMemory[mapped_addr] = data;
			return true;
		}

		return false;
	}

	bool Cartridge::ppuRead(uint16_t addr, uint8_t& data)
	{
		uint32_t mapped_addr = 0;

		if (pMapper->ppuMapRead(addr, mapped_addr))
		{
			data = vCHRMemory[mapped_addr];
			return true;
		}
		else
			return false;
	}

	bool Cartridge::ppuWrite(uint16_t addr, uint8_t data)
	{
		uint32_t mapped_addr = 0;

		if (pMapper->ppuMapWrite(addr, mapped_addr))
		{
			vCHRMemory[mapped_addr] = data;
			return true;
		}
		else
			return false;
	}

	bool Cartridge::isValidROM() const
	{
		return pMapper.operator bool(); // If mapper is not initialized something went wrong when reading .nes file
	}
}