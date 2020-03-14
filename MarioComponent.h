#ifndef MARIOCOMPONENT_H
#define MARIOCOMPONENT_H

#include <QMainWindow>
#include <QTimer>
#include <memory>
#include <QPainter>

#include "Scene.h"
class Sprite;
class MapScene;
class TitleScene;

using namespace std;

class MarioComponent : public QMainWindow
{
    Q_OBJECT

public:
    MarioComponent(QWidget *parent = nullptr);
    ~MarioComponent();
    void toTitle();
    void startGame();
    void startLevel(int difficulty, int type);
    void win();
    void levelWon();
    void levelFailed();
    void lose();
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
    void drawString(QPainter& painter, string text, int x, int y, int c);

private slots:
    void gameLoop();
private:
    QTimer timer;
    shared_ptr<Scene> scene;
    shared_ptr<MapScene> mapScene;
    long long llTick = 0;
    bool focus = true;
};

#endif // MARIOCOMPONENT_H
