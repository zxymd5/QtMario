#ifndef WINSCENE_H
#define WINSCENE_H

#include "Scene.h"

class MarioComponent;

class WinScene : public Scene
{
public:
    WinScene(MarioComponent* component);
    void render(QPainter &painter);
    void tick();
private:
    void drawString(QPainter &painter,  string text, int x, int y, int c);

    MarioComponent* component;
    long long llTick = 0;
    string scrollMessage = "Thank you for saving me, Mario!";
    bool wasDown = true;
};

#endif // WINSCENE_H
