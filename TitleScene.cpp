#include "TitleScene.h"

#include "MarioComponent.h"
#include "BgRenderer.h"
#include "LevelGenerator.h"
#include "BgLevelGenerator.h"
#include "Art.h"
#include "SimpleAudioEngine.h"

#include <math.h>

TitleScene::TitleScene(MarioComponent *component)
{
    this->component = component;
    bgLayer[0].reset(new BgRenderer(2048, 15, 640, 480, 1));
    BgLevelGenerator::createLevel(2048, 15, false, LevelGenerator::TYPE_OVERGROUND, bgLayer[0]->level.get());

    bgLayer[1].reset(new BgRenderer(2048, 15, 640, 480, 2));
    BgLevelGenerator::createLevel(2048, 15, true, LevelGenerator::TYPE_OVERGROUND, bgLayer[1]->level.get());
}

void TitleScene::init()
{
    SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
    SimpleAudioEngine::getInstance()->playBackgroundMusic(Art::songs[4].c_str(), true);
}

void TitleScene::tick()
{
    llTick++;
    if (!wasDown && keys[Qt::Key_S])
    {
        component->startGame();
    }
    if (keys[Qt::Key_S])
    {
        wasDown = false;
    }
}

void TitleScene::render(QPainter &painter)
{
    bgLayer[1]->render(painter, llTick >= 2048 * 32? llTick - 2048 * 32 : llTick, 0);
    bgLayer[0]->render(painter, llTick >= 2048 * 32? llTick - 2048 * 32 : llTick, 0);

    int yo = 32-abs((int)(sin(llTick/6.0)*16));
    QRectF logoRect(0, yo, Art::logo.width() * 2, Art::logo.height() * 2);
    painter.drawImage(logoRect, Art::logo);

    QRectF titleRect(0, 240, Art::titleScreen.width() * 2, Art::titleScreen.height() * 2);
    painter.drawImage(titleRect, Art::titleScreen);

    QRectF tipRect(0, 190, Art::tip.width(), Art::tip.height());
    painter.drawImage(tipRect, Art::tip);
}
