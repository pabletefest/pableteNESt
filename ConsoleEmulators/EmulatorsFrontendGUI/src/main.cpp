#pragma once
#include "emulatorsMainWindow.h"

// Qt
#include <QtWidgets/QApplication>

//SDL
#include <SDL.h>

//STL
#include <iostream>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Video Initialization Error: " << SDL_GetError() << "\n";
        return 0;
    }

    EmulatorsMainWindow mainWindow;
    mainWindow.show();

    int retVal = app.exec();

    SDL_Quit();

    return retVal;
}
