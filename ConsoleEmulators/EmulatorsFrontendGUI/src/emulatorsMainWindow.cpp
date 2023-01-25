#include "emulatorsMainWindow.h"
#include <debuggingWindows.h>

#include <iostream>
//#include <filesystem>
//
//// WinApi
//#include <tchar.h>
//#include <Windows.h>

// SDL
//#include <SDL_syswm.h>

// Qt
//#include <qwindow.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qevent.h>

#define SCREEN_WIDTH 768 /*1024*/ /*1280*/
#define SCREEN_HEIGHT 720 /*960*/ /*720*/

#define PPU_SCANLINE_DOTS 256
#define PPU_NUM_SCANLINES 240

#define PATTERN_TABLE_WIDTH 128
#define PATTERN_TABLE_HEIGHT 128

constexpr const char* APP_TITLE = "pableteNESt (NES EMULATOR)";

EmulatorsMainWindow::EmulatorsMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupGUI();

    holdingKeysMap.insert({ InputType::A, false });
    holdingKeysMap.insert({ InputType::B, false });
    holdingKeysMap.insert({ InputType::SELECT, false });
    holdingKeysMap.insert({ InputType::START, false });
    holdingKeysMap.insert({ InputType::UP, false });
    holdingKeysMap.insert({ InputType::DOWN, false });
    holdingKeysMap.insert({ InputType::LEFT, false });
    holdingKeysMap.insert({ InputType::RIGHT, false });

    renderTimer.setTimerType(Qt::PreciseTimer);
    renderTimer.setInterval(1000 / 60); // 1000 / 60 -> 60 fps
    connect(&renderTimer, SIGNAL(timeout()), this, SLOT(onRenderFrame()));

    QTimer::singleShot(100, this, [this]() {
        this->InitSDLRendering();
    });

    //initEmulator();
}

EmulatorsMainWindow::~EmulatorsMainWindow()
{
    SDL_DestroyTexture(gameTexture);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
}

void EmulatorsMainWindow::InitSDLRendering()
{
    // Set Qt window handle from existing SDl window
    /*window = SDL_CreateWindow(APP_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(window, &wmInfo);
    HWND hwnd = wmInfo.info.win.window;
    setCentralWidget(QWidget::createWindowContainer(QWindow::fromWinId((WId)hwnd), this));*/

    // Create SDL window from Qt one
    window = SDL_CreateWindowFrom(reinterpret_cast<void*>(centralWidget()->winId()));

    if (!window)
    {
        std::cout << "Window Creation Error: " << SDL_GetError() << "\n";
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    gameTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB32, SDL_TEXTUREACCESS_STREAMING, PPU_SCANLINE_DOTS, PPU_NUM_SCANLINES);

    //rendererWidget->setRenderer(renderer);

    //resetRendererBackground();
    
    connect(this, SIGNAL(windowTitleUpdate(QString)), this, SLOT(onWindowTitleUpdate(QString)), Qt::AutoConnection);
    SDL_AddTimer(1000, EmulatorsMainWindow::printFPS, (void*)this);

    /*initEmulator();
    renderTimer.start();*/
}

void EmulatorsMainWindow::keyPressEvent(QKeyEvent* event)
{
    //nes.controllers[0] = 0x00;

    switch (event->key())
    {
    case Qt::Key_X:
        //nes.controllers[0] |= 0x80; // A
        holdingKeysMap[InputType::A] = true;
        break;
    case Qt::Key_Z:
        //nes.controllers[0] |= 0x40; // B
        holdingKeysMap[InputType::B] = true;
        break;
    case Qt::Key_A:
        //nes.controllers[0] |= 0x20; // SELECT
        holdingKeysMap[InputType::SELECT] = true;
        break;
    case Qt::Key_S:
        //nes.controllers[0] |= 0x10; // START
        holdingKeysMap[InputType::START] = true;
        break;
    case Qt::Key_Up:
        //nes.controllers[0] |= 0x08; // UP
        holdingKeysMap[InputType::UP] = true;
        break;
    case Qt::Key_Down:
        //nes.controllers[0] |= 0x04; // DOWN
        holdingKeysMap[InputType::DOWN] = true;
        break;
    case Qt::Key_Left:
        //nes.controllers[0] |= 0x02; // LEFT
        holdingKeysMap[InputType::LEFT] = true;
        break;
    case Qt::Key_Right:
        //nes.controllers[0] |= 0x01; // RIGHT
        holdingKeysMap[InputType::RIGHT] = true;
        break;
    }
}

void EmulatorsMainWindow::keyReleaseEvent(QKeyEvent* event)
{
    //nes.controllers[0] = 0x00;

    switch (event->key())
    {
    case Qt::Key_X:
        //nes.controllers[0] |= 0x00; // A
        holdingKeysMap[InputType::A] = false;
        break;
    case Qt::Key_Z:
        //nes.controllers[0] |= 0x00; // B
        holdingKeysMap[InputType::B] = false;
        break;
    case Qt::Key_A:
        //nes.controllers[0] |= 0x00; // SELECT
        holdingKeysMap[InputType::SELECT] = false;
        break;
    case Qt::Key_S:
        //nes.controllers[0] |= 0x00; // START
        holdingKeysMap[InputType::START] = false;
        break;
    case Qt::Key_Up:
        //nes.controllers[0] |= 0x00; // UP
        holdingKeysMap[InputType::UP] = false;
        break;
    case Qt::Key_Down:
        //nes.controllers[0] |= 0x00; // DOWN
        holdingKeysMap[InputType::DOWN] = false;
        break;
    case Qt::Key_Left:
        //nes.controllers[0] |= 0x00; // LEFT
        holdingKeysMap[InputType::LEFT] = false;
        break;
    case Qt::Key_Right:
        //nes.controllers[0] |= 0x00; // RIGHT
        holdingKeysMap[InputType::RIGHT] = false;
        break;
    }
}

//#define MAX_PATH 256

void EmulatorsMainWindow::setupGUI()
{
    ui.setupUi(this);
    //rendererWidget = new RendererWidget(renderer);
    //setCentralWidget(new RenderingWidget(nes));
    setCentralWidget(new QWidget());
    setBaseSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    resize(SCREEN_WIDTH, SCREEN_HEIGHT);
    setWindowTitle(APP_TITLE);
    this->setFocusPolicy(Qt::StrongFocus);
    centralWidget()->setFocusPolicy(Qt::NoFocus);

    QMenu* fileMenu = menuBar()->addMenu("File");
    QAction* openROMAction = fileMenu->addAction("Open ROM");
    connect(openROMAction, SIGNAL(triggered()), this, SLOT(onOpenROM()));

    QMenu* toolsMenu = menuBar()->addMenu("Tools");
    QMenu* debuggingMenu = toolsMenu->addMenu("Debugging");
    QAction* openNametablesViewerAction = debuggingMenu->addAction("Nametables Viewer (PPU VRAM)");
    connect(openNametablesViewerAction, SIGNAL(triggered()), this, SLOT(openNametablesViewer()));
    QAction* openPatternTablesViewerAction = debuggingMenu->addAction("Pattern tables Viewer (CHR memory data)");
    connect(openPatternTablesViewerAction, SIGNAL(triggered()), this, SLOT(openPatternTablesViewer()));
}

void EmulatorsMainWindow::initEmulator()
{
    /*TCHAR executablePath[MAX_PATH];
    GetModuleFileName(NULL, executablePath, MAX_PATH);
    auto path = std::filesystem::current_path();*/

    std::shared_ptr<nes::Cartridge> cartridge = std::make_shared<nes::Cartridge>("roms/Donkey Kong.nes");
    nes.insertCardtridge(cartridge);
    nes.reset();
}

void EmulatorsMainWindow::resetRendererBackground()
{
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderPresent(renderer);
}

uint32_t EmulatorsMainWindow::printFPS(uint32_t interval, void* params)
{
    auto mainWindow = reinterpret_cast<EmulatorsMainWindow*>(params);

    //printf("Current framerate: %d\n", mainWindow->fps);
    char windowTitleStr[100];
    sprintf_s(windowTitleStr, "%s - FPS: %02d", APP_TITLE, mainWindow->fps);
    SDL_SetWindowTitle(mainWindow->window, windowTitleStr);
    emit mainWindow->windowTitleUpdate(windowTitleStr);

    mainWindow->fps = 0;

    return interval;
}

void EmulatorsMainWindow::handleUserInput()
{
    for (const auto& [key, value] : holdingKeysMap)
    {
        switch (key)
        {
        case InputType::A:
            nes.controllers[0] |= value ? 0x80 : 0x00;
            break;
        case InputType::B:
            nes.controllers[0] |= value ? 0x40 : 0x00;
            break;
        case InputType::SELECT:
            nes.controllers[0] |= value ? 0x20 : 0x00;
            break;
        case InputType::START:
            nes.controllers[0] |= value ? 0x10 : 0x00;
            break;
        case InputType::UP:
            nes.controllers[0] |= value ? 0x08 : 0x00;
            break;
        case InputType::DOWN:
            nes.controllers[0] |= value ? 0x04 : 0x00;
            break;
        case InputType::LEFT:
            nes.controllers[0] |= value ? 0x02 : 0x00;
            break;
        case InputType::RIGHT:
            nes.controllers[0] |= value ? 0x01 : 0x00;
            break;
        }
    }
}

void EmulatorsMainWindow::onOpenROM()
{
    QString filePath = QFileDialog::getOpenFileName(nullptr, "Open ROM file", "/home", tr("ROMs (*.nes)"));

    std::shared_ptr<nes::Cartridge> cartridge = std::make_shared<nes::Cartridge>(filePath.toStdString());

    if (filePath.isEmpty() || !cartridge->isValidROM())
    {
        QMessageBox::critical(nullptr, "ROM reading error", "Could not open the specifed ROM");
        //resetRendererBackground();
        return;
    }
    
    nes.insertCardtridge(cartridge);
    nes.reset();

    currentGame = filePath.split("/").last();
    setWindowTitle(QString(windowTitle() + " - %1").arg(currentGame));

    renderTimer.start();
}

void EmulatorsMainWindow::onWindowTitleUpdate(QString newTitle)
{
    if (currentGame.isEmpty())
        setWindowTitle(newTitle);
    else
        setWindowTitle(QString(newTitle + " - %1").arg(currentGame));
}

void EmulatorsMainWindow::openNametablesViewer()
{
    if (!nes.cartridge || !nes.cartridge->isValidROM())
    {
        QMessageBox::warning(nullptr, "ROM detection error", "Please load first a valid ROM to debug the game");
        return;
    }

    NametablesPreviewWidget* nametablesViewer = new NametablesPreviewWidget(nes);
    nametablesViewer->setWindowTitle("PPU & Graphics Debugging: Nametables");
    nametablesViewer->setAttribute(Qt::WA_DeleteOnClose);
    nametablesViewer->open();
}

void EmulatorsMainWindow::openPatternTablesViewer()
{
    if (!nes.cartridge || !nes.cartridge->isValidROM())
    {
        QMessageBox::warning(nullptr, "ROM detection error", "Please load first a valid ROM to debug the game");
        return;
    }

    CHRMemoryPreviewWidget* patternMemoryViewer = new CHRMemoryPreviewWidget(nes);
    patternMemoryViewer->setWindowTitle("PPU & Graphics Debugging: Pattern Tables");
    patternMemoryViewer->setAttribute(Qt::WA_DeleteOnClose);
    patternMemoryViewer->open();
}

void EmulatorsMainWindow::onRenderFrame()
{
    nes.controllers[0] = 0x00; // Reset every frame
    handleUserInput();
    nes.runFrame();

    /*do
    {
        nes.clock();
    } while (!nes.ppu.frameCompleted);
        
    nes.ppu.frameCompleted = false;*/

    const std::vector<nes::PPU::Pixel>& pixels = nes.ppu.getPixelsFrameBuffer();
    SDL_UpdateTexture(gameTexture, nullptr, pixels.data(), sizeof(nes::PPU::Pixel) * PPU_SCANLINE_DOTS);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, gameTexture, NULL, NULL);
    SDL_RenderPresent(renderer);

    fps++;

    //this->update();
    //centralWidget()->update();
}
