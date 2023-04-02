#include "debuggingWindows.h"

#include <qpainter.h>

DebuggingWindow::DebuggingWindow(const nes::SystemBus& nesConsole, QWidget* parent)
    : nes(nesConsole), QWidget(parent)
{
    /*this->resize(1536, 720);*/
    this->setLayout(new QHBoxLayout());

    nametablesWidget = new NametablesPreviewWidget(nes);
    this->layout()->addWidget(nametablesWidget);
}