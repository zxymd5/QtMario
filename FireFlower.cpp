#include "FireFlower.h"
#include "Art.h"
#include "LevelScene.h"
#include "Mario.h"

FireFlower::FireFlower(int x, int y)
{
    sheet = Art::items;

    this->x = x;
    this->y = y;
    xPicO = 16;
    yPicO = 30;

    xPic = 1;
    yPic = 0;
    height = 24;
    facing = 1;
    wPic  = hPic = 32;
    life = 0;
}

void FireFlower::move()
{
    if (life<9)
    {
        layer = 0;
        y-=2;
        life++;
        return;
    }
}

void FireFlower::collideCheck()
{
    Mario* mario = Sprite::levelScene->mario.get();

    float xMarioD = mario->x - x;
    float yMarioD = mario->y - y;
    float w = 16;
    if (xMarioD > -w && xMarioD < w)
    {
        if (yMarioD > -height && yMarioD < mario->height)
        {
            mario->getFlower();
            Sprite::levelScene->removeSprite(this);
        }
    }
}
