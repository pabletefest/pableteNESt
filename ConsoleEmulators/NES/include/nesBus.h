#pragma once
#include "cpu6502.h"

//#include <cstdint>
#include <array>

// Every device is connected to this bus. Acts as a NES abstraction
class NESBusSystem
{
public:
	NESBusSystem();
	~NESBusSystem() = default;

	void writeData(uint16_t address, uint8_t data);
	uint8_t readData(uint16_t address);

	void reset();
	void clock();

public:
	CPU cpu;
	std::array<uint8_t, 64 * 1024> ram;
	uint64_t totalSystemClockCycles{0};
};