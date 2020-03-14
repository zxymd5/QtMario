#include "Sparkle.h"
#include "Art.h"
#include "LevelScene.h"

Sparkle::Sparkle(int x, int y, float xa, float ya, int xPic, int yPic, int timeSpan)
{
    sheet = Art::particles;
    this->x = x;
    this->y = y;
    this->xa = xa;
    this->ya = ya;
    this->xPic = xPic;
    xPicStart = xPic;
    this->yPic = yPic;
    this->xPicO = 8;
    this->yPicO = 8;

    wPic = 16;
    hPic = 16;
    life = 10 + rand() % timeSpan;
}

void Sparkle::move()
{
    if (life>10)
        xPic = 7;
    else
        xPic = xPicStart+(10-life)*4/10;

    if (life--<0)
        Sprite::levelScene->removeSprite(this);

    x+=xa;
    y+=ya;
}
