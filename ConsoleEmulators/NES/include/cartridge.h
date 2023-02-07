#pragma once
#include "mapper.h"

#include <string>
#include <memory>
#include <vector>

namespace nes
{
	// Represents an abstraction of a  NES cartridge 
	class Cartridge : public std::enable_shared_from_this<Cartridge>
	{
	public:

		enum class Mirroring
		{
			VERTICAL,
			HORIZONTAL,
			ONE_SCREEN,
			ONE_SCREEN_LOWER_BANK,
			ONE_SCREEN_UPPER_BANK,
			FOUR_SCREEN
		};

		Cartridge(const std::string& filename);
		~Cartridge() = default;

		// Communication with Main Bus
		bool cpuRead(uint16_t addr, uint8_t& data);
		bool cpuWrite(uint16_t addr, uint8_t data);

		// Communication with PPU Bus
		bool ppuRead(uint16_t addr, uint8_t& data);
		bool ppuWrite(uint16_t addr, uint8_t data);

		Mirroring getNTMirroring() const
		{
			return mirroring;
		}

		void setNTMirroring(Mirroring mirroringMode)
		{
			mirroring = mirroringMode;
		}

		bool isValidROM() const;

		bool isCHRRAMCart() const;
		bool hasBatteryBackedRAM() const;

		std::vector<uint8_t>& getPRGMemoryData()
		{
			return vPRGMemory;
		}

		const std::vector<uint8_t>& getPRGMemoryData() const
		{
			return vPRGMemory;
		}

		std::vector<uint8_t>& getCHRMemoryData()
		{
			return vCHRMemory;
		}

		const std::vector<uint8_t>& getCHRMemoryData() const
		{
			return vCHRMemory;
		}

		uint8_t getMapperID() const
		{
			return nMapperID;
		}

		uint8_t getNumPRGBanks() const
		{
			return nPRGBanks;
		}

		uint8_t getNumCHRBanks() const
		{
			return nCHRBanks;
		}

		std::string getGameName() const
		{
			return gameName;
		}

	private:
		std::string gameName;

		uint8_t nMapperID = 0;
		uint8_t nPRGBanks = 0;
		uint8_t nCHRBanks = 0;

		std::vector<uint8_t> vPRGMemory;
		std::vector<uint8_t> vCHRMemory;

		Mirroring mirroring;

		std::shared_ptr<Mapper> pMapper;
	};
}