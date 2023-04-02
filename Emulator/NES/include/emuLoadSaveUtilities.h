#pragma once

namespace nes
{
    class SystemBus;
}

void loadEmulatorState(nes::SystemBus& nes);

void saveEmulatorState(const nes::SystemBus& nes);