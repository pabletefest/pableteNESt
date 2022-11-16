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
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define PPU_SCANLINE_DOTS 341
#define PPU_NUM_SCANLINES 262

constexpr const char* APP_TITLE = "pableteNESt (NES EMULATOR)";

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Video Initialization Error: " << SDL_GetError() << "\n";
        return 0;
    }

    window = SDL_CreateWindow(APP_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (!window)
    {
        std::cout << "Window Creation Error: " << SDL_GetError() << "\n";
        return 0;
    }

    //SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    //SDL_Surface* screen = SDL_GetWindowSurface(window);
    //SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, screen);
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, PPU_SCANLINE_DOTS, PPU_NUM_SCANLINES);
    //SDL_RenderSetLogicalSize(renderer, 1280, 720);
    //SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    /*std::vector<PPU::Pixel> pixels = std::vector<PPU::Pixel>(640 * 480);
    memset(pixels.data(), 0, sizeof(PPU::Pixel) * pixels.size());*/

    SDL_Event event;
    bool isRunnning = true;

    uint32_t fps = 0;

    SDL_AddTimer(1000, printFPS, (void*)&fps);

    NESBusSystem nes;

    std::shared_ptr<Cartridge> cartridge = std::make_shared<Cartridge>("tests/nestest.nes");
    nes.insertCardtridge(cartridge);
    nes.reset();

    std::vector<PPU::Pixel> pixels2 = std::vector<PPU::Pixel>(128 * 128);

    // Iterate over sprites indeces
    for (int indexTileX = 0; indexTileX < 16; indexTileX++)
    {
        for (int indexTileY = 0; indexTileY < 16; indexTileY++)
        {
            int tileOffset = indexTileY * 256 + indexTileX * 16;

            // Iterate over sprite rows
            for (int sprRow = 0; sprRow < 8; sprRow++)
            {
                //Each tile is 16 bytes, each row is 2 bytes separated each one in two blocks of 8 bytes (LSB and MSB, each one separated 8 bytes)
                uint8_t LSB = nes.ppu.ppuRead(0x0000 + tileOffset + sprRow + 0x0000);
                uint8_t MSB = nes.ppu.ppuRead(0x0000 + tileOffset + sprRow + 0x0008);

                // Iterate over sprite column from a row using LSB and MSB
                for (int sprCol = 0; sprCol < 8; sprCol++)
                {
                    uint8_t lsb = (LSB & 0x80) >> 7;
                    uint8_t msb = (MSB & 0x80) >> 7;

                    uint8_t pixel = (msb << 1) | lsb;
                    
                    uint8_t nesColour = nes.ppu.ppuRead(0x3F00 + (1 * 4 + pixel));
                    if (nesColour != 0x00)
                        printf("");
                    pixels2[(indexTileY * 8 + sprRow) + (indexTileX * 8 + sprCol)] = nes.ppu.getRGBAFromNesColour(nesColour);

                    lsb <<= 1;
                    msb <<= 1;
                }
            }
        }
    }

    nes.ppu.ppuRead(0);

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
                    break;
                }
            }
        }

        do 
        {
            nes.clock();
        } 
        while (!nes.ppu.frameCompleted);

        nes.ppu.frameCompleted = false;

        //const std::vector<PPU::Pixel>& pixels = nes.ppu.getPixelsFrameBuffer();
        //SDL_UpdateTexture(texture, nullptr, pixels.data(), sizeof(PPU::Pixel) * 341);
        SDL_UpdateTexture(texture, nullptr, pixels2.data(), sizeof(PPU::Pixel) * 128);
       
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
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

#ifdef LOG_MODE

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

#endif

uint32_t printFPS(uint32_t interval, void* params)
{
    auto fpsPtr = reinterpret_cast<uint32_t*>(params);

    printf("Current framerate: %d\n", *fpsPtr);
    char windowTitleStr[100];
    sprintf_s(windowTitleStr, "%s - FPS: %02d", APP_TITLE, *fpsPtr);
    SDL_SetWindowTitle(window, windowTitleStr);

    *fpsPtr = 0;

    return interval;
}