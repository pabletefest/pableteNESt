#include <iostream>
#include <sstream>

#include "nesBus.h"
#include "cartridge.h"

#include "SDL.h"

#ifdef LOG_MODE
#include <fstream>
#include <tuple>
#endif

void run_nestest(NESBusSystem& nes);
std::tuple<std::string, std::string, uint32_t> compareWithNestestLog();
uint32_t printFPS(uint32_t interval, void* params);

static SDL_Window* window = nullptr;

int main(int argc, char* argv[])
{
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

    //SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_Surface* screen = SDL_GetWindowSurface(window);
    //SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, screen);
    //SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, 640, 480);
    //uint32_t* pixels = new uint32_t[640 * 480];
    //memset(pixels, 0xFF10A57F, 640* 480);

    //SDL_UpdateTexture(texture, NULL, pixels, 0);

    SDL_Event event;
    bool isRunnning = true;

    uint32_t fps = 0;

    SDL_AddTimer(1000, printFPS, (void*)&fps);

    NESBusSystem nes;

    std::shared_ptr<Cartridge> cartridge = std::make_shared<Cartridge>("tests/nestest.nes");
    nes.insertCardtridge(cartridge);
    nes.reset();

    while (isRunnning)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                isRunnning = false;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                SDL_Keycode keyCode = event.key.keysym.sym;

                switch (keyCode)
                {
                case SDLK_SPACE:
                case SDLK_RETURN:
                    /*uint32_t colour = SDL_MapRGB(screen->format, rand() % 256, rand() % 256, rand() % 256);
                    SDL_FillRect(screen, NULL, colour);*/
                    SDL_SetRenderDrawColor(renderer, rand() % 256, rand() % 256, rand() % 256, rand() % 256);
                    break;
                }
            }
        }
       
        SDL_RenderClear(renderer);
        //SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        //SDL_UpdateWindowSurface(window);

        fps++;
    }

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
    //SDL_DestroyTexture(texture);
    SDL_DestroyWindow(window);
    SDL_Quit();

    std::cin.get();
    return 0;
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
            std::istringstream iss(line);
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

uint32_t printFPS(uint32_t interval, void* params)
{
    auto fpsPtr = reinterpret_cast<uint32_t*>(params);

    printf("Current framerate: %d\n", *fpsPtr);
    char windowTitleStr[100];
    sprintf_s(windowTitleStr, "NES EMULATOR - FPS: %02d", *fpsPtr);
    SDL_SetWindowTitle(window, windowTitleStr);

    *fpsPtr = 0;

    return interval;
}