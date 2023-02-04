#include "emuLoadSaveUtilities.h"
#include "nesBus.h"
#include "emuNESStateInfoTypes.h"
#include <fstream>

void loadEmulatorState(nes::SystemBus& nes)
{
    nes::EmulatorStateInfo emuStateInfo;

    std::ifstream inFile("nesEmuState.bin", std::ofstream::binary);

    if (inFile.is_open())
    {
        inFile.read((char*)&emuStateInfo, sizeof(nes::EmulatorStateInfo));
        inFile.close();

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
    }
}

void saveEmulatorState(const nes::SystemBus& nes)
{
    nes::EmulatorStateInfo emuStateInfo;

    nes.cpu.saveState(emuStateInfo);
    nes.ppu.saveState(emuStateInfo);

    memcpy(emuStateInfo.ram, nes.cpuRam.data(),  2048);
    memcpy(emuStateInfo.controllers, nes.controllers,  sizeof(nes.controllers));
    memcpy(emuStateInfo.controllersCache, nes.controllersCache,  sizeof(nes.controllersCache));
    emuStateInfo.dmaInternalLoAddr = nes.dmaInternalLoAddr;
    emuStateInfo.dmaPageHiAddr = nes.dmaPageHiAddr;
    emuStateInfo.dmaReadData = nes.dmaReadData;
    emuStateInfo.dmaTransferInterrupt = nes.dmaTransferInterrupt;
    emuStateInfo.waitForEvenCycle = nes.waitForEvenCycle;

    std::ofstream outFile("nesEmuState.bin", std::ofstream::binary);
    outFile.write((const char*)&emuStateInfo, sizeof(nes::EmulatorStateInfo));
    outFile.close();
}