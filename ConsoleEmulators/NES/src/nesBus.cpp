#include "nesBus.h"
#include "cpu6502.h"

NESBusSystem::NESBusSystem() : cpu(this)
{
	for (auto& elem : ram)
		elem = 0x00;
}

void NESBusSystem::cpuWrite(uint16_t address, uint8_t data)
{
	if (address >= 0x0000 && address <= 0xFFFF)
		ram[address] = data;
}

uint8_t NESBusSystem::cpuRead(uint16_t address)
{
	if (address >= 0x0000 && address <= 0xFFFF)
		return ram[address];
}

void NESBusSystem::reset()
{
	cpu.reset();
	totalSystemClockCycles = 0;
}

void NESBusSystem::clock()
{
	cpu.clock();
	totalSystemClockCycles++;
}
