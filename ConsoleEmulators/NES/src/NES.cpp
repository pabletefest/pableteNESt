#include <iostream>
#include <sstream>
#include "nesBus.h"
#include "cartridge.h"

void runTest3x10(NESBusSystem& nes)
{
    // Load Program (assembled at https://www.masswerk.at/6502/assembler.html)
        /*
            *=$8000
            LDX #10
            STX $0000
            LDX #3
            STX $0001
            LDY $0000
            LDA #0
            CLC
            loop
            ADC $0001
            DEY
            BNE loop
            STA $0002
            NOP
            NOP
            NOP
        */

    std::stringstream ss;
    ss << "A2 0A 8E 00 00 A2 03 8E \
        01 00 AC 00 00 A9 00 18 \
        6D 01 00 88 D0 FA 8D 02 \
        00 EA EA EA";

    uint16_t romOffset = 0x8000;

    while (!ss.eof())
    {
        std::string byte;
        ss >> byte;
        nes.cpuRam[romOffset++] = (uint8_t)std::stoul(byte, nullptr, 16);
    }

    //NMI Vector
    nes.cpuRam[0xFFFA] = 0x00;
    nes.cpuRam[0xFFFB] = 0x00;

    //Reset Vector
    nes.cpuRam[0xFFFC] = 0x00;
    nes.cpuRam[0xFFFD] = 0x80;

    //IRQ Vector
    nes.cpuRam[0xFFFE] = 0x00;
    nes.cpuRam[0xFFFF] = 0x80;

    nes.reset();

    while (nes.cpuRam[0x0002] != 0x001e)
    {
        do
        {
            nes.clock();
        } while (!nes.cpu.isLastCompleted());

        printf("\nA register value is %d", nes.cpu.A);
        printf("\nX register value is %d", nes.cpu.X);
        printf("\nY register value is %d", nes.cpu.Y);
        printf("\nPC register value is %#x", nes.cpu.PC);
        printf("\nSP register value is %#x", nes.cpu.SP);
        printf("\nSTATUS register value is %#x\n", nes.cpu.status);
    }

    while (nes.cpu.PC != 0x8000)
    {
        do
        {
            nes.clock();
        } while (!nes.cpu.isLastCompleted());
    }
}

void run_nestest(NESBusSystem& nes)
{
    std::shared_ptr<Cartridge> cartridge = std::make_shared<Cartridge>("tests/nestest.nes");
    nes.insertCardtridge(cartridge);

    nes.reset();

    while (true)
    {
        printf("\nA register value is %d", nes.cpu.A);
        printf("\nX register value is %d", nes.cpu.X);
        printf("\nY register value is %d", nes.cpu.Y);
        printf("\nPC register value is %#x", nes.cpu.PC);
        printf("\nSP register value is %#x", nes.cpu.SP);
        printf("\nSTATUS register value is %#x\n", nes.cpu.status);

        nes.clock();
    }
}

int main()
{
    NESBusSystem nes;

    run_nestest(nes);

    /*std::cout << "Hello World!\n";
    std::cin.get();*/

    //runTest3x10(nes);

    std::cin.get();
}