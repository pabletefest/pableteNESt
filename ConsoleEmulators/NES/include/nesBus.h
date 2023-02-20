#pragma once
#include "cpu6502.h"
#include "ppu2C02.h"
#include "apu2A03.h"
#include "cartridge.h"

//#include <cstdint>
#include <array>
#include <memory>

namespace nes
{
	// Every device is connected to this bus. Acts as a NES abstraction
	class SystemBus
	{
	public:
		SystemBus();
		~SystemBus() = default;

		// Main Bus Read & Write
		void cpuWrite(uint16_t address, uint8_t data);
		uint8_t cpuRead(uint16_t address);

		// Connects a cartridge object to the internal buses
		void insertCardtridge(const std::shared_ptr<Cartridge>& cart);

		void reset();
		void clock();
		void runFrame();

		uint8_t getAudioSample() const;

	public:
		CPU cpu;
		PPU ppu;
		APU apu;

		// Game Cartridge
		std::shared_ptr<Cartridge> cartridge;

		std::array<uint8_t, 2048> cpuRam; // 2KB RAM

		uint64_t totalSystemClockCycles = 0;

		uint8_t controllers[2];

		uint8_t dmaPageHiAddr = 0x00;
		uint8_t dmaInternalLoAddr = 0x00;
		uint8_t dmaReadData = 0x00;
		bool dmaTransferInterrupt = false;
		bool waitForEvenCycle = true;

		mutable bool isAudioSampleReady = false;
		friend uint8_t getOutputAPU();

	public:
		uint8_t controllersCache[2]; // Internal keep state
	
	private:
		uint32_t audioElapsedTime = 0;
	};
}