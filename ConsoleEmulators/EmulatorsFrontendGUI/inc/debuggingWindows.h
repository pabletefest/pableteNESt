#pragma once
#include <nesBus.h>

#include <iostream>
#include <sstream>
#include <iomanip>

#include "SDL_ttf.h"
#include "SDL.h"

#include <qwidget.h>
#include <qdialog.h>
#include <qtimer.h>
#include <qboxlayout.h>

//class NametablesPreviewWidget;
//class CHRMemoryPreviewWidget;

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

// Results in a reference (std::decay might simplify the code)
template<typename T>
using ConvertToReference = typename std::add_lvalue_reference<typename std::remove_const<typename std::remove_reference<T>::type>::type>::type;

class NametablesPreviewWidget : public QDialog
{
    Q_OBJECT

public:
    NametablesPreviewWidget(const nes::SystemBus& nesConsole, QWidget* parent = nullptr)
        : nes(const_cast<ConvertToReference<decltype(nesConsole)>>(nesConsole)), // Same as const_cast<nes::SystemBus&> but templated for any type
        QDialog(parent)
    {
        memcpy(nameTables[0], nes.ppu.getNametable(0), 1024);
        memcpy(nameTables[1], nes.ppu.getNametable(1), 1024);

        if (nes.cartridge->getNTMirroring() == nes::Cartridge::Mirroring::HORIZONTAL)
        {
            this->setLayout(new QVBoxLayout());
            this->resize(480, 720);
        }
        else if (nes.cartridge->getNTMirroring() == nes::Cartridge::Mirroring::VERTICAL)
        {
            this->setLayout(new QHBoxLayout());
            this->resize(1152, 640);
        }

        nametable0Widget = new QWidget();
        nametable1Widget = new QWidget();
        this->layout()->addWidget(nametable0Widget);
        this->layout()->addWidget(nametable1Widget);

        TTF_Init();
        pixelEmulatorFont = TTF_OpenFont("fonts/PixelEmulator.ttf", 20);

        windowNT0 = SDL_CreateWindowFrom(reinterpret_cast<void*>(nametable0Widget->winId()));
        windowNT1 = SDL_CreateWindowFrom(reinterpret_cast<void*>(nametable1Widget->winId()));

        if (!windowNT0 || !windowNT1)
        {
            std::cout << "Window Creation Error: " << SDL_GetError() << "\n";
            return;
        }

        rendererNT0 = SDL_CreateRenderer(windowNT0, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        rendererNT1 = SDL_CreateRenderer(windowNT1, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        connect(&renderTimer, &QTimer::timeout, this, [this]() {

            memcpy(nameTables[0], nes.ppu.getNametable(0), 1024);
            memcpy(nameTables[1], nes.ppu.getNametable(1), 1024);

            SDL_RenderClear(rendererNT0);
            SDL_RenderClear(rendererNT1);

            SDL_Surface* nameTableSurfaceNT0 = TTF_RenderText_Solid_Wrapped(pixelEmulatorFont, getTextFromBuffer<uint8_t>(nameTables[0], 960).c_str(), whiteColour, 0);
            SDL_Texture* nameTableTextureNT0 = SDL_CreateTextureFromSurface(rendererNT0, nameTableSurfaceNT0);
            SDL_Surface* nameTableSurfaceNT1 = TTF_RenderText_Solid_Wrapped(pixelEmulatorFont, getTextFromBuffer<uint8_t>(nameTables[1], 960).c_str(), whiteColour, 0);
            SDL_Texture* nameTableTextureNT1 = SDL_CreateTextureFromSurface(rendererNT1, nameTableSurfaceNT1);

            SDL_RenderCopy(rendererNT0, nameTableTextureNT0, NULL, NULL);
            SDL_RenderCopy(rendererNT1, nameTableTextureNT1, NULL, NULL);

            SDL_FreeSurface(nameTableSurfaceNT0);
            SDL_DestroyTexture(nameTableTextureNT0);
            SDL_FreeSurface(nameTableSurfaceNT1);
            SDL_DestroyTexture(nameTableTextureNT1);

            SDL_RenderPresent(rendererNT0);
            SDL_RenderPresent(rendererNT1);
        });

        renderTimer.start(1000 / 60);
    }

    ~NametablesPreviewWidget() override
    {
        TTF_CloseFont(pixelEmulatorFont);
        TTF_Quit();
        SDL_DestroyWindow(windowNT0);
        SDL_DestroyWindow(windowNT1);
        SDL_DestroyRenderer(rendererNT0);
        SDL_DestroyRenderer(rendererNT1);
    };

private:
    nes::SystemBus& nes;

    uint8_t nameTables[2][1024];

    QWidget* nametable0Widget = nullptr;
    QWidget* nametable1Widget = nullptr;

    TTF_Font* pixelEmulatorFont = NULL;
    SDL_Color whiteColour = { 255, 255, 255 };

    SDL_Window* windowNT0 = NULL;
    SDL_Renderer* rendererNT0 = NULL;
    SDL_Window* windowNT1 = NULL;
    SDL_Renderer* rendererNT1 = NULL;

    QTimer renderTimer;
};

#define PATTERN_TABLE_WIDTH 128
#define PATTERN_TABLE_HEIGHT 128

class CHRMemoryPreviewWidget : public QDialog
{
public:
    CHRMemoryPreviewWidget(const nes::SystemBus& nesConsole, QWidget* parent = nullptr)
        : nes(const_cast<ConvertToReference<decltype(nesConsole)>>(nesConsole)), QDialog(parent)
    {
        chrTilesData[0] = nes.ppu.getPatternTableBuffer(0, 0);
        chrTilesData[1] = nes.ppu.getPatternTableBuffer(1, 0);

        this->setLayout(new QVBoxLayout());
        this->resize(512, 768);

        spriteTilesWidget = new QWidget();
        backgroundTilesWidget = new QWidget();

        this->layout()->addWidget(spriteTilesWidget);
        this->layout()->addWidget(backgroundTilesWidget);

        windowSPR = SDL_CreateWindowFrom(reinterpret_cast<void*>(spriteTilesWidget->winId()));
        windowBG = SDL_CreateWindowFrom(reinterpret_cast<void*>(backgroundTilesWidget->winId()));

        if (!windowBG || !windowSPR)
        {
            std::cout << "Window Creation Error: " << SDL_GetError() << "\n";
            return;
        }

        rendererSPR = SDL_CreateRenderer(windowSPR, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        rendererBG = SDL_CreateRenderer(windowBG, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        sprTilesTexture = SDL_CreateTexture(rendererSPR, SDL_PIXELFORMAT_ARGB32, SDL_TEXTUREACCESS_STREAMING, PATTERN_TABLE_WIDTH, PATTERN_TABLE_HEIGHT);
        bgTilesTexture = SDL_CreateTexture(rendererBG, SDL_PIXELFORMAT_ARGB32, SDL_TEXTUREACCESS_STREAMING, PATTERN_TABLE_WIDTH, PATTERN_TABLE_HEIGHT);

        connect(&renderTimer, &QTimer::timeout, this, [this]() {

            chrTilesData[0] = nes.ppu.getPatternTableBuffer(0, 0);
            chrTilesData[1] = nes.ppu.getPatternTableBuffer(1, 0);

            SDL_RenderClear(rendererSPR);
            SDL_RenderClear(rendererBG);

            SDL_UpdateTexture(sprTilesTexture, nullptr, chrTilesData[0].data(), sizeof(nes::PPU::Pixel) * PATTERN_TABLE_WIDTH);
            SDL_UpdateTexture(bgTilesTexture, nullptr, chrTilesData[1].data(), sizeof(nes::PPU::Pixel) * PATTERN_TABLE_WIDTH);

            SDL_RenderCopy(rendererSPR, sprTilesTexture, NULL, NULL);   
            SDL_RenderCopy(rendererBG, bgTilesTexture, NULL, NULL);

            SDL_RenderPresent(rendererSPR);
            SDL_RenderPresent(rendererBG);
        });

        renderTimer.start(1000 / 60);
    }

    ~CHRMemoryPreviewWidget() override
    {
        SDL_DestroyTexture(bgTilesTexture);
        SDL_DestroyTexture(sprTilesTexture);
        SDL_DestroyWindow(windowBG);
        SDL_DestroyWindow(windowSPR);
        SDL_DestroyRenderer(rendererBG);
        SDL_DestroyRenderer(rendererSPR);
    }

private:
    nes::SystemBus& nes;

    std::array<std::vector<nes::PPU::Pixel>, 2> chrTilesData;

    QWidget* spriteTilesWidget = nullptr;
    QWidget* backgroundTilesWidget = nullptr;

    SDL_Texture* sprTilesTexture = NULL;
    SDL_Texture* bgTilesTexture = NULL;
    SDL_Window* windowSPR = NULL;
    SDL_Renderer* rendererSPR = NULL;
    SDL_Window* windowBG = NULL;
    SDL_Renderer* rendererBG = NULL;

    QTimer renderTimer;
};

class DebuggingWindow : public QWidget
{
    Q_OBJECT

public:
    DebuggingWindow(const nes::SystemBus& nesConsole, QWidget* parent = nullptr);
    ~DebuggingWindow() override = default;

private:
    const nes::SystemBus& nes;

    NametablesPreviewWidget* nametablesWidget{nullptr};
    CHRMemoryPreviewWidget* chrMemWidget{nullptr};
};