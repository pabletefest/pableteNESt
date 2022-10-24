#include "nesBus.h"
#include "cpu6502.h"

NESBusSystem::NESBusSystem() : cpu(this)
{
	for (auto& elem : cpuRam)
		elem = 0x00;
}

void NESBusSystem::cpuWrite(uint16_t address, uint8_t data)
{
	if (cartridge->cpuWrite(address, data))
	{
		// The cartridge "sees all" and has the facility to veto
		// the propagation of the bus transaction if it requires.
		// This allows the cartridge to map any address to some
		// other data, including the facility to divert transactions
		// with other physical devices. The NES does not do this
		// but I figured it might be quite a flexible way of adding
		// "custom" hardware to the NES in the future!
	}
	else if (address >= 0x0000 && address <= 0x1FFF)
	{
		cpuRam[address & 0x07FF] = data;
	}
	else if (address >= 0x2000 && address <= 0x3FFF)
	{
		ppu.cpuWrite(address & 0x0007, data);
	}
}

uint8_t NESBusSystem::cpuRead(uint16_t address)
{
	uint8_t dataRead = 0x00;
	if (address == 0x4ABC)
	{
		dataRead = 0;
	}
	if (cartridge->cpuRead(address, dataRead))
	{
		// Cartridge Address Range
	}
	if (address >= 0x0000 && address <= 0x1FFF)
	{
		dataRead = cpuRam[address & 0x07FF];
	}
	else if (address >= 0x2000 && address <= 0x3FFF)
	{
		dataRead = ppu.cpuRead(address & 0x0007);
	}

	return dataRead;
}

void NESBusSystem::insertCardtridge(const std::shared_ptr<Cartridge>& cart)
{
	this->cartridge = cart;
	ppu.connectCartridge(cart);
}

void NESBusSystem::reset()
{
	cpu.reset();
	totalSystemClockCycles = 0;
}

void NESBusSystem::clock()
{
	ppu.clock();

	if (totalSystemClockCycles % 3 == 0)
	{
		cpu.clock();
	}

	totalSystemClockCycles++;
}