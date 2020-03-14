#include "FlowerEnemy.h"
#include "LevelScene.h"
#include "Mario.h"
#include "Sparkle.h"
#include<math.h>

FlowerEnemy::FlowerEnemy(int x, int y) :
    Enemy(x, y, 1, Enemy::ENEMY_SPIKY, false)
{
    noFireballDeath = false;
    this->xPic = 0;
    this->yPic = 6;
    this->yPicO = 48;
    this->height = 24;
    this->width = 4;

    yStart = y;
    ya = -16;
    this->y -= 2;
    this->layer = 0;
    for (int i = 0; i < 4; i++)
    {
        move();
    }
}

void FlowerEnemy::move()
{
    if (deadTime > 0)
    {
        deadTime--;

        if (deadTime == 0)
        {
            deadTime = 1;
            for (int i = 0; i < 8; i++)
            {
                Sprite::levelScene->addSprite(new Sparkle(x + rand() % 32 - 8, (y - rand() % 16) + 8,
                                                          rand() % 1000 / 250.00 - 2,
                                                          rand() % 1000 / 1000.00 * -1.00, 0, 1, 5));
            }
            Sprite::levelScene->removeSprite(this);
        }

        x += xa;
        y += ya;
        ya *= 0.95;
        ya += 2;

        return;
    }

    llTick++;

    if (y>=yStart)
    {
        y = yStart;

        int xd = (int)(fabs(Sprite::levelScene->mario.get()->x - x));
        jumpTime++;
        if (jumpTime>40 && xd>48)
        {
            ya = -16;
        }
        else
        {
            ya = 0;
        }
    }
    else
    {
        jumpTime = 0;
    }

    y+=ya;
    ya*=0.9;
    ya+=0.2f;

    xPic = ((llTick/2)&1)*2+((llTick/6)&1);
}
