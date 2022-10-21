#include "nesBus.h"
#include "cpu6502.h"

NESBusSystem::NESBusSystem() : cpu(this)
{
	for (auto& elem : ram)
		elem = 0x00;
}

void NESBusSystem::writeData(uint16_t address, uint8_t data)
{
	ram[address] = data;

#if _DEBUG
#include <stdlib.h>
	printf("Written value %d to address %#x\n", data, address);
#endif
}

uint8_t NESBusSystem::readData(uint16_t address)
{
	return ram[address];
#if _DEBUG
#include <stdlib.h>
	printf("Read value %d from address %#x\n", ram[address], address);
#endif
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
