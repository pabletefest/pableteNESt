#include "cpu6502.h"
#include "nesBus.h"

CPU::CPU(NESBusSystem* nesBus) : bus(nesBus)
{
}

void CPU::writeData(uint16_t address, uint8_t data)
{
	bus->writeData(address, data);
}

uint8_t CPU::readData(uint16_t address)
{
	return bus->readData(address);
}

void CPU::reset()
{
}

void CPU::clock()
{
}
