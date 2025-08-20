#include "sgg/graphics.h"
#include "gamestate.hpp"
#include "config.hpp"

void update(float dt)
{
    GameState::getInstance()->update(dt);
}


void draw()
{
	GameState::getInstance()->draw();
}

void init()
{
    GameState::getInstance()->init();

    // Set warnings, errors and failed assertions to be printed on the console
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
}

int main()
{
    graphics::createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Frogger");

    init();
	GameState::getInstance()->setDebugMode(DEBUG_MODE);


    graphics::setUserData(GameState::getInstance());

    graphics::setDrawFunction(draw);
    graphics::setUpdateFunction(update);

    graphics::setCanvasSize(CANVAS_WIDTH, CANVAS_HEIGHT);
    graphics::setCanvasScaleMode(graphics::CANVAS_SCALE_FIT);

    graphics::startMessageLoop();

    GameState::getInstance()->releaseInstance();
    graphics::destroyWindow();

    _CrtDumpMemoryLeaks();

    return 0;
}