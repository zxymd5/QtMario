#ifndef LOSESCENE_H
#define LOSESCENE_H

#include "Scene.h"

class MarioComponent;

class LoseScene : public Scene
{
public:
    LoseScene(MarioComponent* component);
    void render(QPainter &painter);
    void tick();
private:
    void drawString(QPainter &painter,  string text, int x, int y, int c);

    MarioComponent* component;
    int llTick = 0;
    string scrollMessage = "Game over!";
    bool wasDown = true;
};

#endif // LOSESCENE_H
