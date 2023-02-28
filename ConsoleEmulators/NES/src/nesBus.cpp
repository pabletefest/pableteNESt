#include "nesBus.h"
#include "cpu6502.h"

namespace nes
{
	constexpr uint32_t CPU_CLOCK_SPEED = 1789773;
	constexpr uint32_t AUDIO_FREQUENCY = 44100;

	SystemBus::SystemBus() : cpu(this)
	{
		for (auto& elem : cpuRam)
			elem = 0x00; // Can be randomized with rand() & 0xFF
	}

	void SystemBus::cpuWrite(uint16_t address, uint8_t data)
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
		else if ((address >= 0x4000 && address <= 0x4013) || address == 0x4015 || address == 0x4017)
		{
			apu.cpuWrite(address, data);
		}
		else if (address >= 0x4016 && address <= 0x4017)
		{
			controllersCache[address & 0x0001] = controllers[address & 0x0001];
		}
		else if (address == 0x4014)
		{
			dmaPageHiAddr = data;
			dmaTransferInterrupt = true;
		}
	}

	uint8_t SystemBus::cpuRead(uint16_t address)
	{
		uint8_t dataRead = 0x00;

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
		else if ((address >= 0x4000 && address <= 0x4013) || address == 0x4015 || address == 0x4017)
		{
			dataRead = apu.cpuRead(address);
		}
		else if (address >= 0x4016 && address <= 0x4017)
		{
			dataRead = (controllersCache[address & 0x0001] & 0x80) > 0;
			controllersCache[address & 0x0001] <<= 1;
		}

		return dataRead;
	}

	void SystemBus::insertCardtridge(const std::shared_ptr<Cartridge>& cart)
	{
		this->cartridge = cart;
		ppu.connectCartridge(cart);
	}

	void SystemBus::reset()
	{
		ppu.reset();
		cpu.reset();
		apu.reset();
		totalSystemClockCycles = 0;
		dmaPageHiAddr = 0x00;
		dmaInternalLoAddr = 0x00;
		dmaReadData = 0x00;
		dmaTransferInterrupt = false;
		waitForEvenCycle = true;
		std::memset(controllers, 0, 2);
	}

	void SystemBus::clock()
	{
		ppu.clock();

		if (totalSystemClockCycles % 3 == 0)
		{
			if (dmaTransferInterrupt)
			{
				if (waitForEvenCycle && (totalSystemClockCycles % 2 == 0))
					waitForEvenCycle = false;

				if (!waitForEvenCycle)
				{
					if (totalSystemClockCycles % 2 == 0)
					{
						// Read DMA cycle
						dmaReadData = cpuRead(dmaPageHiAddr << 8 | dmaInternalLoAddr);
					}
					else
					{
						cpuWrite(0x2004, dmaReadData); // Write OAM data via ppu reg $2004
						dmaInternalLoAddr++;

						// Write DMA cycle
						if (dmaInternalLoAddr == 0x00) // Wrap around, 256 read/write pairs performed
						{
							waitForEvenCycle = true;
							dmaTransferInterrupt = false;
						}
					}
				}
			}
			else
			{
				cpu.clock();	
			}

			apu.clock();

			audioElapsedTime += AUDIO_FREQUENCY;

			if (audioElapsedTime >= CPU_CLOCK_SPEED)
			{
				isAudioSampleReady = true;
				audioElapsedTime -= CPU_CLOCK_SPEED;
			}
		}

		if (ppu.nmi)
		{
			cpu.nmi();
			ppu.nmi = false;
		}

		if (apu.irq)
		{
			cpu.irq();
			apu.irq = false;
		}

		totalSystemClockCycles++;
	}

	void SystemBus::runFrame()
	{
		do
		{
			this->clock();
		} while (!ppu.frameCompleted);

		ppu.frameCompleted = false;
	}

	float SystemBus::getAudioSample() const
	{
		isAudioSampleReady = false;

		return apu.getOutputAPU();
	}
}