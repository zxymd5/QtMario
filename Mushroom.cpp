#include "Mushroom.h"
#include "Art.h"
#include "LevelScene.h"
#include "Mario.h"
#include <math.h>

float Mushroom::GROUND_INERTIA = 0.89f;
float Mushroom::AIR_INERTIA = 0.89f;

Mushroom::Mushroom(int x, int y)
{
    sheet = Art::items;

    this->x = x;
    this->y = y;
    xPicO = 16;
    yPicO = 30;

    yPic = 0;
    height = 24;
    facing = 1;
    wPic  = hPic = 32;
    life = 0;
}

void Mushroom::move()
{
    if (life<9)
    {
        layer = 0;
        y-=2;
        life++;
        return;
    }
    float sideWaysSpeed = 3.5f;
    layer = 1;

    if (xa > 4)
    {
        facing = 1;
    }
    if (xa < -4)
    {
        facing = -1;
    }

    xa = facing * sideWaysSpeed;

    mayJump = (onGround);

    xFlipPic = facing == -1;

    runTime += (fabs(xa)) + 10;



    if (!move(xa, 0)) facing = -facing;
    onGround = false;
    move(0, ya);

    ya *= 0.85f;
    if (onGround)
    {
        xa *= GROUND_INERTIA;
    }
    else
    {
        xa *= AIR_INERTIA;
    }

    if (!onGround)
    {
        ya += 4;
    }
}

void Mushroom::bumpCheck(int xTile, int yTile)
{
    if (x + width > xTile * 32 && x - width < xTile * 32 + 32 && yTile==(int)((y-1)/32))
    {
        facing = -Sprite::levelScene->mario->facing;
        ya = -20;
    }
}

void Mushroom::collideCheck()
{
    Mario* mario = Sprite::levelScene->mario.get();

    float xMarioD = mario->x - x;
    float yMarioD = mario->y - y;
    float w = 32;
    if (xMarioD > -w && xMarioD < w)
    {
        if (yMarioD > -height && yMarioD < mario->height)
        {
            mario->getMushroom();
            Sprite::levelScene->removeSprite(this);
        }
    }
}

bool Mushroom::move(float xa, float ya)
{
    while (xa > 16)
    {
        if (!move(16, 0)) return false;
        xa -= 16;
    }
    while (xa < -16)
    {
        if (!move(-16, 0)) return false;
        xa += 16;
    }
    while (ya > 16)
    {
        if (!move(0, 16)) return false;
        ya -= 16;
    }
    while (ya < -16)
    {
        if (!move(0, -16)) return false;
        ya += 16;
    }

    bool collide = false;
    if (ya > 0)
    {
        if (isBlocking(x + xa - width, y + ya, xa, 0)) collide = true;
        else if (isBlocking(x + xa + width, y + ya, xa, 0)) collide = true;
        else if (isBlocking(x + xa - width, y + ya + 1, xa, ya)) collide = true;
        else if (isBlocking(x + xa + width, y + ya + 1, xa, ya)) collide = true;
    }
    if (ya < 0)
    {
        if (isBlocking(x + xa, y + ya - height, xa, ya)) collide = true;
        else if (collide || isBlocking(x + xa - width, y + ya - height, xa, ya)) collide = true;
        else if (collide || isBlocking(x + xa + width, y + ya - height, xa, ya)) collide = true;
    }
    if (xa > 0)
    {
        if (isBlocking(x + xa + width, y + ya - height, xa, ya)) collide = true;
        if (isBlocking(x + xa + width, y + ya - height / 2, xa, ya)) collide = true;
        if (isBlocking(x + xa + width, y + ya, xa, ya)) collide = true;

        if (avoidCliffs && onGround && !Sprite::levelScene->level->isBlocking((int) ((x + xa + width) / 32), (int) ((y) / 32 + 1), xa, 1)) collide = true;
    }
    if (xa < 0)
    {
        if (isBlocking(x + xa - width, y + ya - height, xa, ya)) collide = true;
        if (isBlocking(x + xa - width, y + ya - height / 2, xa, ya)) collide = true;
        if (isBlocking(x + xa - width, y + ya, xa, ya)) collide = true;

        if (avoidCliffs && onGround && !Sprite::levelScene->level->isBlocking((int) ((x + xa - width) / 32), (int) ((y) / 32 + 1), xa, 1)) collide = true;
    }

    if (collide)
    {
        if (xa < 0)
        {
            x = (int) ((x - width) / 32) * 32 + width;
            this->xa = 0;
        }
        if (xa > 0)
        {
            x = (int) ((x + width) / 32 + 1) * 32 - width - 2;
            this->xa = 0;
        }
        if (ya < 0)
        {
            y = (int) ((y - height) / 32) * 32 + height;
            jumpTime = 0;
            this->ya = 0;
        }
        if (ya > 0)
        {
            y = (int) (y / 32 + 1) * 32 - 2;
            onGround = true;
        }
        return false;
    }
    else
    {
        x += xa;
        y += ya;
        return true;
    }
}

bool Mushroom::isBlocking(float _x, float _y, float xa, float ya)
{
    int x = (int) (_x / 32);
    int y = (int) (_y / 32);
    if (x == (int) (this->x / 32) && y == (int) (this->y / 32)) return false;

    bool blocking = Sprite::levelScene->level->isBlocking(x, y, xa, ya);


    return blocking;
}
