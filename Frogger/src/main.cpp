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

    return 0;
}
