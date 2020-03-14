#include "MarioComponent.h"
#include "LevelScene.h"
#include "TitleScene.h"
#include "MapScene.h"
#include "Art.h"
#include "Mario.h"
#include "LoseScene.h"
#include "WinScene.h"
#include "SimpleAudioEngine.h"
#include <QPainter>
#include <QtDebug>
#include <QKeyEvent>
#include <string>
#include <sstream>
using namespace std;

MarioComponent::MarioComponent(QWidget *parent)
    : QMainWindow(parent)
{
    this->setFixedSize(640, 480);

    setWindowIcon(QIcon(":/resources/Mario.ico"));
    setWindowTitle(QString("Infinite Mario Bros"));

    setFocusPolicy(Qt::StrongFocus);

    Art::init();
    toTitle();

    mapScene.reset(new MapScene(this));

    timer.setInterval(40);
    connect(&timer, &QTimer::timeout, this, &MarioComponent::gameLoop);
    timer.start();
}

void MarioComponent::startLevel(int difficulty, int type)
{
    string name = typeid (*(scene.get())).name();
    if (name.find("MapScene") != string::npos) {
        mapScene->copyMapSceneData((MapScene*)scene.get());
    }

    scene.reset(new LevelScene(this, difficulty, type));
    scene->init();
}

void MarioComponent::win()
{
    scene.reset(new WinScene(this));
    scene->init();
}

void MarioComponent::levelWon()
{
    SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
    SimpleAudioEngine::getInstance()->playBackgroundMusic(Art::songs[0].c_str(), true);

    mapScene->levelWon();
    scene.reset(new MapScene(mapScene.get()));

}

void MarioComponent::levelFailed()
{
    SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
    SimpleAudioEngine::getInstance()->playBackgroundMusic(Art::songs[0].c_str(), true);
    scene.reset(new MapScene(mapScene.get()));
    Mario::lives--;
    if (Mario::lives == 0)
    {
        lose();
    }
}

void MarioComponent::lose()
{
    scene.reset(new LoseScene(this));
    scene->init();
}

void MarioComponent::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if(scene != nullptr){
        scene->render(painter);
    }

    if(!focus && llTick / 4 % 2 == 0) {
        string msg = "CLICK TO PLAY";

        drawString(painter, msg, 320 - msg.length() * 8 + 2, 220 + 2, 0);
        drawString(painter, msg, 320 - msg.length() * 8, 220, 7);
    }
}

void MarioComponent::keyPressEvent(QKeyEvent *event)
{
    scene->toggleKey(event->key(), true);
}

void MarioComponent::keyReleaseEvent(QKeyEvent *event)
{
    scene->toggleKey(event->key(), false);
}

void MarioComponent::focusInEvent(QFocusEvent *event)
{
    focus = true;
}

void MarioComponent::focusOutEvent(QFocusEvent *event)
{
    focus = false;
}

void MarioComponent::drawString(QPainter &painter, string text, int x, int y, int c)
{
    for(int i = 0; i < text.length(); i++) {
        QImage img = Art::font[text[i] - 32][c];
        QRectF rect(x + i * img.width() * 2, y,
                    img.width() * 2, img.height() * 2);
        painter.drawImage(rect, img);
    }
}

MarioComponent::~MarioComponent()
{
    SimpleAudioEngine::getInstance()->end();
}

void MarioComponent::toTitle()
{
    Mario::resetStatic();
    scene.reset(new TitleScene(this));
    scene->init();
}

void MarioComponent::startGame()
{
    mapScene->init();
    scene.reset(new MapScene(mapScene.get()));
    SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
    SimpleAudioEngine::getInstance()->playBackgroundMusic(Art::songs[0].c_str(), true);
}

void MarioComponent::gameLoop()
{
    llTick++;
    if(scene.get() != nullptr) {
    scene->tick();
    update();
    }
}

