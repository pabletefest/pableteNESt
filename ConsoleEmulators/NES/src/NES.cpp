#pragma once
#include <iostream>
#include <sstream>
#include <iomanip>

#include "nesBus.h"
#include "cartridge.h"

#include "SDL.h"
#include "SDL_ttf.h"

#ifdef LOG_MODE
#include <fstream>
#include <tuple>
#endif

void run_nestest(nes::SystemBus& nes);

std::tuple<std::string, std::string, uint32_t> compareWithNestestLog();

uint32_t printFPS(uint32_t interval, void* params);

template<typename T>
std::string getTextFromBuffer(T* nametable, uint16_t bufferSize)
{
    std::stringstream text;

    for (uint16_t i = 0; i < bufferSize; i++)
    {
        if (i % 32 == 0 && i != 0)
            text << "\n";

        char chr[4];
        sprintf_s(chr, 4, "%02X", nametable[i]);
        text << std::setw(3) << std::setfill(' ') << chr;
        //text << std::hex << std::to_string(nametable[i]); // Get nametable as text (nestest shows fine)
    }

    return text.str();
}

static SDL_Window* window = nullptr;
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define PPU_SCANLINE_DOTS 256
#define PPU_NUM_SCANLINES 240

#define PATTERN_TABLE_WIDTH 128
#define PATTERN_TABLE_HEIGHT 128

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
    SDL_Texture* gameTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB32, SDL_TEXTUREACCESS_STREAMING, PPU_SCANLINE_DOTS, PPU_NUM_SCANLINES);
    SDL_Texture* sprTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB32, SDL_TEXTUREACCESS_STREAMING, PATTERN_TABLE_WIDTH, PATTERN_TABLE_HEIGHT);
    SDL_Texture* bgTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB32, SDL_TEXTUREACCESS_STREAMING, PATTERN_TABLE_WIDTH, PATTERN_TABLE_HEIGHT);
    //SDL_Texture* bgTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_BGRA8888, SDL_TEXTUREACCESS_STREAMING, PATTERN_TABLE_WIDTH, PATTERN_TABLE_HEIGHT);
    //SDL_RenderSetLogicalSize(renderer, 1280, 720);
    //SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    /*std::vector<nes::PPU::Pixel> pixels = std::vector<PPU::Pixel>(640 * 480);
    memset(pixels.data(), 0, sizeof(PPU::Pixel) * pixels.size());*/

    SDL_Event event;
    bool isRunnning = true;

    uint32_t fps = 0;

    SDL_AddTimer(1000, printFPS, (void*)&fps);

    nes::SystemBus nes;

    //std::shared_ptr<nes::Cartridge> cartridge = std::make_shared<nes::Cartridge>("tests/nestest.nes");
    //std::shared_ptr<nes::Cartridge> cartridge = std::make_shared<nes::Cartridge>("roms/Donkey Kong.nes");
    //std::shared_ptr<nes::Cartridge> cartridge = std::make_shared<nes::Cartridge>("roms/Super Mario Bros.nes");
    //std::shared_ptr<nes::Cartridge> cartridge = std::make_shared<nes::Cartridge>("roms/Pac-Man.nes");
    //std::shared_ptr<nes::Cartridge> cartridge = std::make_shared<nes::Cartridge>("roms/Ice Climber.nes");
    //std::shared_ptr<nes::Cartridge> cartridge = std::make_shared<nes::Cartridge>("roms/Space Invaders.nes");
    //std::shared_ptr<nes::Cartridge> cartridge = std::make_shared<nes::Cartridge>("roms/Space Invaders (J).nes");
    //std::shared_ptr<nes::Cartridge> cartridge = std::make_shared<nes::Cartridge>("roms/Empire Invaders (Space Invaders Hack).nes");
    std::shared_ptr<nes::Cartridge> cartridge = std::make_shared<nes::Cartridge>("roms/Contra.nes");
    //std::shared_ptr<nes::Cartridge> cartridge = std::make_shared<nes::Cartridge>("roms/Tetris.nes");
    //std::shared_ptr<nes::Cartridge> cartridge = std::make_shared<nes::Cartridge>("roms/Castlevania.nes");
    //std::shared_ptr<nes::Cartridge> cartridge = std::make_shared<nes::Cartridge>("roms/Dragon Warrior.nes"); // Dragon Quest 2
    //std::shared_ptr<nes::Cartridge> cartridge = std::make_shared<nes::Cartridge>("roms/Dragon Warrior II.nes"); // Dragon Quest 2
    //std::shared_ptr<nes::Cartridge> cartridge = std::make_shared<nes::Cartridge>("roms/Dragon Warrior III.nes"); // Dragon Quest 3
    //std::shared_ptr<nes::Cartridge> cartridge = std::make_shared<nes::Cartridge>("roms/Final Fantasy II.nes");
    //std::shared_ptr<nes::Cartridge> cartridge = std::make_shared<nes::Cartridge>("roms/Mega Man.nes");
    //std::shared_ptr<nes::Cartridge> cartridge = std::make_shared<nes::Cartridge>("roms/Metal Gear.nes");
    //std::shared_ptr<nes::Cartridge> cartridge = std::make_shared<nes::Cartridge>("roms/Prince of Persia.nes");

    nes.insertCardtridge(cartridge);
    nes.reset();

    // Fill palette ram with a greyscale for debugging windows
    for (uint32_t i = 0; i < 0x32; i++)
    {
        uint16_t addr = 0x3F00 + i;
        if ((addr & 0x0F) == 0 || ((addr & 0x0F) % 4) == 0)
            nes.ppu.ppuWrite(addr, 0x0d);
        else
        {
            switch ((addr & 0x00FF) % 4)
            {
            case 0:
                nes.ppu.ppuWrite(addr, 0x00);
                break;
            case 1:
                nes.ppu.ppuWrite(addr, 0x10);
                break;
            case 2:
                nes.ppu.ppuWrite(addr, 0x20);
                break;
            }
        }
    }

    std::vector<nes::PPU::Pixel> sprPatternTable = nes.ppu.getPatternTableBuffer(0, 0);
    std::vector<nes::PPU::Pixel> bgPatternTable = nes.ppu.getPatternTableBuffer(1, 0);

    TTF_Init();
    TTF_Font* pixelEmulatorFont = TTF_OpenFont("fonts/PixelEmulator.ttf", 20);
    SDL_Color whiteColour = { 255, 255, 255 };

    while (isRunnning)
    {
        nes.controllers[0] = 0x00; // Reset every frame

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
                case SDLK_x:
                    nes.controllers[0] |= 0x80; // A
                    break;
                case SDLK_z:
                    nes.controllers[0] |= 0x40; // B
                    break;
                case SDLK_a:
                    nes.controllers[0] |= 0x20; // Select
                    break;
                case SDLK_s:
                    nes.controllers[0] |= 0x10; // Start
                    break;
                case SDLK_UP:
                    nes.controllers[0] |= 0x08; // UP ARROW
                    break;
                case SDLK_DOWN:
                    nes.controllers[0] |= 0x04; // DOWN ARROW
                    break;
                case SDLK_LEFT:
                    nes.controllers[0] |= 0x02; // LEFT ARROW
                    break;
                case SDLK_RIGHT:
                    nes.controllers[0] |= 0x01; // RIGHT ARROW
                    break;
                }
            }
            else if (event.type == SDL_KEYUP)
            {
                SDL_Keycode keyCode = event.key.keysym.sym;

                switch (keyCode)
                {
                case SDLK_x:
                    nes.controllers[0] |= 0x00;
                    break;
                case SDLK_z:
                    nes.controllers[0] |= 0x00;
                    break;
                case SDLK_a:
                    nes.controllers[0] |= 0x00;
                    break;
                case SDLK_s:
                    nes.controllers[0] |= 0x00;
                    break;
                case SDLK_UP:
                    nes.controllers[0] |= 0x00;
                    break;
                case SDLK_DOWN:
                    nes.controllers[0] |= 0x00;
                    break;
                case SDLK_LEFT:
                    nes.controllers[0] |= 0x00;
                    break;
                case SDLK_RIGHT:
                    nes.controllers[0] |= 0x00;
                    break;
                }
            }
        }

        do
        {
            nes.clock();
        } while (!nes.ppu.frameCompleted);

        nes.ppu.frameCompleted = false;

        const std::vector<nes::PPU::Pixel>& pixels = nes.ppu.getPixelsFrameBuffer();
        sprPatternTable = nes.ppu.getPatternTableBuffer(0, 0);
        bgPatternTable = nes.ppu.getPatternTableBuffer(1, 0);
        SDL_UpdateTexture(gameTexture, nullptr, pixels.data(), sizeof(nes::PPU::Pixel) * PPU_SCANLINE_DOTS);
        SDL_UpdateTexture(sprTexture, nullptr, sprPatternTable.data(), sizeof(nes::PPU::Pixel) * PATTERN_TABLE_WIDTH);
        SDL_UpdateTexture(bgTexture, nullptr, bgPatternTable.data(), sizeof(nes::PPU::Pixel) * PATTERN_TABLE_WIDTH);

        SDL_Rect gameViewport;
        gameViewport.w = PPU_SCANLINE_DOTS * 3;
        gameViewport.h = PPU_NUM_SCANLINES * 3;
        gameViewport.x = 0;
        gameViewport.y = 0;

        SDL_Rect sprViewport;
        sprViewport.w = SCREEN_WIDTH - gameViewport.w;
        sprViewport.h = gameViewport.h / 2;
        sprViewport.x = gameViewport.w;
        sprViewport.y = 0;

        SDL_Rect bgViewport;
        bgViewport.w = SCREEN_WIDTH - gameViewport.w;
        bgViewport.h = gameViewport.h / 2;
        bgViewport.x = gameViewport.w;
        bgViewport.y = sprViewport.h;

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, gameTexture, NULL, &gameViewport);
        SDL_RenderCopy(renderer, sprTexture, NULL, &sprViewport);
        SDL_RenderCopy(renderer, bgTexture, NULL, &bgViewport);

        /*uint8_t* nameTable = reinterpret_cast<uint8_t*>(nes.ppu.getNametable(1));
        SDL_Surface* nameTableSurface = TTF_RenderText_Solid_Wrapped(pixelEmulatorFont, getTextFromBuffer<uint8_t>(nameTable, 960).c_str(), whiteColour, 0);
        SDL_Texture* nameTableTexture = SDL_CreateTextureFromSurface(renderer, nameTableSurface);
        SDL_RenderCopy(renderer, nameTableTexture, NULL, &gameViewport);
        SDL_FreeSurface(nameTableSurface);
        SDL_DestroyTexture(nameTableTexture);*/

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
    TTF_CloseFont(pixelEmulatorFont);
    TTF_Quit();
    SDL_DestroyTexture(gameTexture);
    SDL_DestroyTexture(sprTexture);
    SDL_DestroyTexture(bgTexture);
    SDL_DestroyWindow(window);
    SDL_Quit();

    std::cin.get();
    return 0;
}

#ifdef LOG_MODE

void run_nestest(nes::SystemBus& nes)
{
    nes.cpu.PC = 0x8000;
    std::shared_ptr<nes::Cartridge> cartridge = std::make_shared<Cartridge>("tests/nestest.nes");
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