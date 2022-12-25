#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_emulatorsfrontendgui.h"

class EmulatorsMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    EmulatorsMainWindow(QWidget *parent = nullptr);
    ~EmulatorsMainWindow() override;

private:
    Ui::EmulatorsFrontendGUIClass ui;
};
