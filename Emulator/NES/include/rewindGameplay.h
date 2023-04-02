#pragma once
#include "emuNESStateInfoTypes.h"
#include <deque>

namespace nes
{
    class SystemBus;
}

class RewindManager
{
public:
    RewindManager(nes::SystemBus& nesEmu);
    ~RewindManager() = default;

    void stackFrame();
    bool unstackFrame(); // Returns true while rewinding, false when buffer is empty
    void clearStack();

private:
    nes::SystemBus& nes;
    std::deque<nes::EmulatorStateInfo> rewindStackQueue; // it's a queue to be able to remove old frames to add new ones when full
};