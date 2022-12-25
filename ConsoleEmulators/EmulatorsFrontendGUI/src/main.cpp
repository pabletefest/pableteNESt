#include "emulatorsMainWindow.h"

// EMU_CORE NES
#include <nesBus.h>

// Qt
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    EmulatorsMainWindow mainWindow;
    mainWindow.show();

    nes::SystemBus nes;

    return app.exec();
}
