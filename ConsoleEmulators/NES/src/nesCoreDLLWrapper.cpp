#include "nesBus.h"
#include <memory>

extern "C" __declspec(dllexport) nes::SystemBus* CreateNesCoreEmu()
{
    return new nes::SystemBus();
}

extern "C" __declspec(dllexport) void DeleteNesCoreEmu(nes::SystemBus* nesSystem)
{
    if (!nesSystem) return;

    delete nesSystem;
}

extern "C" __declspec(dllexport) void ResetEmulator(nes::SystemBus * nesSystem)
{
    if (!nesSystem) return;

    nesSystem->reset();
}

extern "C" __declspec(dllexport) void LoadROMCartridge(nes::SystemBus* nesSystem, const char* filename)
{
    if (!nesSystem) return;

    nesSystem->insertCardtridge(std::make_shared<nes::Cartridge>(filename));
    nesSystem->reset();
}

extern "C" __declspec(dllexport) void RunEmulatorFrame(nes::SystemBus * nesSystem)
{
    if (!nesSystem) return;

    do
    {
        nesSystem->clock();
    } while (!nesSystem->ppu.frameCompleted);

    nesSystem->ppu.frameCompleted = false;
}

extern "C" __declspec(dllexport) uint32_t* GetPixelsFrameBuffer(nes::SystemBus* nesSystem)
{
    if (!nesSystem) return nullptr;

    return reinterpret_cast<uint32_t*>(std::remove_const_t<uint32_t*>(nesSystem->ppu.getPixelsFrameBuffer().data()));
}

extern "C" __declspec(dllexport) void UpdateControllerPadBuffer(nes::SystemBus * nesSystem, uint8_t padRegister, uint8_t pad)
{
    if (!nesSystem) return;

    nesSystem->controllers[pad] = 0x00;
    nesSystem->controllers[pad] = padRegister;
}