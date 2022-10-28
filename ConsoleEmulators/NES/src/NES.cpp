#include <iostream>
#include <sstream>

#include "nesBus.h"
#include "cartridge.h"

#include "SDL.h"

#ifdef LOG_MODE
#include <fstream>
#include <tuple>
#endif

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
    nes.cpu.PC = 0x8000;
    std::shared_ptr<Cartridge> cartridge = std::make_shared<Cartridge>("tests/nestest.nes");
    nes.insertCardtridge(cartridge);

    nes.reset();

    nes.cpu.PC = 0xC000;
    
    nes.cpuRam[0x01FF] = 0x4A;
    nes.cpuRam[0x01FE] = 0xBC;


    while (nes.cpu.PC != 0xC66E)
    {
#if 0
        printf("\nA register value is %d", nes.cpu.A);
        printf("\nX register value is %d", nes.cpu.X);
        printf("\nY register value is %d", nes.cpu.Y);
        printf("\nPC register value is %#x", nes.cpu.PC);
        printf("\nSP register value is %#x", nes.cpu.SP);
        printf("\nSTATUS register value is %#x\n", nes.cpu.status);
#endif
        nes.clock();
    }
}

std::tuple<std::string, std::string, uint32_t> compareWithNestestLog()
{
    std::vector<std::string> nestestPCVec;
    std::vector<std::string> myTestPCVec;

    std::ifstream ifs;
    ifs.open("nestest.txt");

    if (ifs.is_open())
    {
        std::string line;

        while (std::getline(ifs, line))
        {
            std::istringstream iss (line);
            std::string PC;
            iss >> PC;
            nestestPCVec.push_back(PC);
        }
    }

    ifs.close();

    ifs.open("CPU6502.txt");
    
    if (ifs.is_open())
    {
        std::string line;

        while (std::getline(ifs, line))
        {
            std::istringstream iss(line);
            std::string PC;
            iss >> PC;
            myTestPCVec.push_back(PC);
        }
    }

    for (uint32_t i = 0; i < nestestPCVec.size(); i++)
    {
        if (nestestPCVec[i] != myTestPCVec[i])
            return std::make_tuple(nestestPCVec[i], myTestPCVec[i], i);
    }

    return {};
}

int main(int argc, char* argv[])
{
    /*SDL_Window* window = nullptr;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Video Initialization Error: " << SDL_GetError() << "\n";
        return 0;
    }

    window = SDL_CreateWindow("NES EMULATOR", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);

    if (!window)
    {
        std::cout << "Window Creation Error: " << SDL_GetError() << "\n";
        return 0;
    }

    while (true)
        SDL_UpdateWindowSurface(window);*/

    NESBusSystem nes;

#undef LOG_MODE
#ifdef LOG_MODE
    std::ifstream ifs;
    ifs.open("CPU6502.txt");
    if (ifs.is_open())
    {
        auto result = compareWithNestestLog();
        printf("In line %d of 8991, nestest log shows %s and mine shows %s\n\n", std::get<2>(result) + 1, std::get<0>(result).c_str(), std::get<1>(result).c_str());
    }
    run_nestest(nes);
#endif

    //SDL_DestroyWindow(window);
    //SDL_Quit();

    std::cin.get();
    return 0;
}