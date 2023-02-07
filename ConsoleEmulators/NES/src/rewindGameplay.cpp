#include "rewindGameplay.h"
#include "nesBus.h"
#include "emu_typedefs.h"

RewindManager::RewindManager(nes::SystemBus& nesEmu)
    : nes(nesEmu)
{

}

void RewindManager::stackFrame()
{
    if (rewindStackQueue.size() == 360) // Max 6 secs of frames
        rewindStackQueue.pop_front();

    nes::EmulatorStateInfo emuStateInfo;

    nes.cpu.saveState(emuStateInfo);
    nes.ppu.saveState(emuStateInfo);

    memcpy(emuStateInfo.ram, nes.cpuRam.data(), 2048);
    memcpy(emuStateInfo.controllers, nes.controllers, sizeof(nes.controllers));
    memcpy(emuStateInfo.controllersCache, nes.controllersCache, sizeof(nes.controllersCache));
    emuStateInfo.dmaInternalLoAddr = nes.dmaInternalLoAddr;
    emuStateInfo.dmaPageHiAddr = nes.dmaPageHiAddr;
    emuStateInfo.dmaReadData = nes.dmaReadData;
    emuStateInfo.dmaTransferInterrupt = nes.dmaTransferInterrupt;
    emuStateInfo.waitForEvenCycle = nes.waitForEvenCycle;

    if (nes.cartridge->isCHRRAMCart())
    {
        if (!emuStateInfo.cartState.has_value())
            emuStateInfo.cartState = nes::EmulatorStateInfo::CartridgeState{};

        auto& chrMemory = nes.cartridge->getCHRMemoryData();
        memcpy((*emuStateInfo.cartState).CHRMemory, chrMemory.data(), chrMemory.size());
    }

    if (nes.cartridge->hasBatteryBackedRAM())
    {
        auto& prgMemory = nes.cartridge->getPRGMemoryData();
        auto offsetWRAM = nes.cartridge->getNumPRGBanks() * convertKBToBytes(16);
        memcpy((*emuStateInfo.cartState).wram, prgMemory.data() + offsetWRAM, 0x2000);
    }

    rewindStackQueue.push_back(emuStateInfo);
}

bool RewindManager::unstackFrame()
{
    if (rewindStackQueue.empty())
        return false;

    const auto& emuStateInfo = rewindStackQueue.back();

    nes.cpu.loadState(emuStateInfo);
    nes.ppu.loadState(emuStateInfo);

    memcpy(nes.cpuRam.data(), emuStateInfo.ram, 2048);
    memcpy(nes.controllers, emuStateInfo.controllers, sizeof(nes.controllers));
    memcpy(nes.controllersCache, emuStateInfo.controllersCache, sizeof(nes.controllersCache));
    nes.dmaInternalLoAddr = emuStateInfo.dmaInternalLoAddr;
    nes.dmaPageHiAddr = emuStateInfo.dmaPageHiAddr;
    nes.dmaReadData = emuStateInfo.dmaReadData;
    nes.dmaTransferInterrupt = emuStateInfo.dmaTransferInterrupt;
    nes.waitForEvenCycle = emuStateInfo.waitForEvenCycle;

    if (emuStateInfo.cartState.has_value())
    {
        if (nes.cartridge->isCHRRAMCart())
        {
            auto& chrMemory = nes.cartridge->getCHRMemoryData();
            memcpy(chrMemory.data(), (*emuStateInfo.cartState).CHRMemory, chrMemory.size());
        }

        if (nes.cartridge->hasBatteryBackedRAM())
        {
            auto& prgMemory = nes.cartridge->getPRGMemoryData();
            auto offsetWRAM = nes.cartridge->getNumPRGBanks() * convertKBToBytes(16);
            memcpy(prgMemory.data() + offsetWRAM, (*emuStateInfo.cartState).wram, 0x2000);
        }
    }

    rewindStackQueue.pop_back();

    return true;
}

void RewindManager::clearStack()
{
    //while (!rewindStackQueue.empty())
    //    rewindStackQueue.pop_back();

    rewindStackQueue.clear();
}
