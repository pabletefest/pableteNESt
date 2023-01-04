#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_emulatorsfrontendgui.h"

// EMU_CORE NES
#include <nesBus.h>

// SDL
#include <SDL.h>

// Qt
#include <qtimer.h>

class RendererWidget : public QWidget
{
public:
    RendererWidget(SDL_Renderer* renderer, QWidget* parent = nullptr)
        : rendererHandler(renderer), QWidget(parent)
    {
    }

    ~RendererWidget() override = default;

    void setRenderer(SDL_Renderer* renderer)
    {
        rendererHandler = renderer;
    }

    void setNeedToReset(bool isNeeded)
    {
        needToReset = isNeeded;
    }

protected:
    void paintEvent(QPaintEvent* event) override
    {
        if (needToReset)
        {
            SDL_SetRenderDrawColor(rendererHandler, 0, 0, 0, 255);
            SDL_RenderClear(rendererHandler);
            SDL_RenderPresent(rendererHandler);
        }

        QWidget::paintEvent(event);
    }

private:
    SDL_Renderer* rendererHandler = NULL;
    bool needToReset{ false };
};

class EmulatorsMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    EmulatorsMainWindow(QWidget *parent = nullptr);
    ~EmulatorsMainWindow() override;

    void InitSDLRendering(); // Setup after Qt MainWindow ctor ended

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private:
    void setupGUI();
    void initEmulator();
    void resetRendererBackground();
    static uint32_t printFPS(uint32_t interval, void* params);

signals:
    void windowTitleUpdate(QString newTitle);

private slots:
    void onRenderFrame();
    void onOpenROM();
    void onWindowTitleUpdate(QString newTitle);
    void openNametablesViewer();
    void openPatternTablesViewer();

private:
    Ui::EmulatorsFrontendGUIClass ui;

    RendererWidget* rendererWidget = nullptr;

    SDL_Renderer* renderer = NULL;
    SDL_Window* window = NULL;
    SDL_Texture* gameTexture = NULL;

    nes::SystemBus nes;

    QTimer renderTimer;
    uint32_t fps = 0;
};