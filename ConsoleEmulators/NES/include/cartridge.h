#pragma once
#include "mapper.h"

#include <string>
#include <memory>
#include <vector>

// Represents an abstraction of a  NES cartridge 
class Cartridge
{
public:
    Cartridge(const std::string& filename);
    ~Cartridge() = default;

	// Communication with Main Bus
	bool cpuRead(uint16_t addr, uint8_t& data);
	bool cpuWrite(uint16_t addr, uint8_t data);

	// Communication with PPU Bus
	bool ppuRead(uint16_t addr, uint8_t& data);
	bool ppuWrite(uint16_t addr, uint8_t data);

private:
	uint8_t nMapperID = 0;
	uint8_t nPRGBanks = 0;
	uint8_t nCHRBanks = 0;

	std::vector<uint8_t> vPRGMemory;
	std::vector<uint8_t> vCHRMemory;

	std::shared_ptr<Mapper> pMapper;
};