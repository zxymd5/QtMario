#include "Fireball.h"
#include "LevelScene.h"
#include "Sparkle.h"
#include "Art.h"

#include<math.h>

float Fireball::GROUND_INERTIA = 0.89f;
float Fireball::AIR_INERTIA = 0.89f;

Fireball::Fireball(float x, float y, int facing)
{
    sheet = Art::particles;

    this->x = x;
    this->y = y;

    xPicO = 8;
    yPicO = 8;

    yPic = 3;
    height = 16;
    this->facing = facing;
    wPic = 16;
    hPic = 16;

    xPic = 4;
    ya = 8;
}

void Fireball::move()
{
    LevelScene* world = Sprite::levelScene;

    if (deadTime > 0)
    {
        for (int i = 0; i < 8; i++)
        {
            world->addSprite(new Sparkle((int) (x + rand() % 16 - 8)+8,
                                         (int) (y + rand() % 16 - 8) + 4,
                                         rand() % 1000 / 1000.00 * 4.00 - 2 - 2 * facing,
                                         rand() % 1000 / 1000.00 * 4.00 - 2, 0, 1, 5));
        }
        world->removeSprite(this);

        return;
    }

    if (facing != 0) anim++;

    float sideWaysSpeed = 16.0f;

    if (xa > 4)
    {
        facing = 1;
    }
    if (xa < -4)
    {
        facing = -1;
    }

    xa = facing * sideWaysSpeed;

    world->checkFireballCollide(this);

    xFlipPic = facing == -1;

    runTime += (fabs(xa)) + 10;

    xPic = (anim) % 4;

    if (!move(xa, 0))
    {
        die();
    }

    onGround = false;
    move(0, ya);
    if (onGround) ya = -20;

    ya *= 0.95f;
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
        ya += 3.0;
    }
}

bool Fireball::isBlocking(float _x, float _y, float xa, float ya)
{
    int x = (int) (_x / 32);
    int y = (int) (_y / 32);
    if (x == (int) (this->x / 32) && y == (int) (this->y / 32)) return false;

    LevelScene* world = Sprite::levelScene;

    bool blocking = world->level->isBlocking(x, y, xa, ya);

    return blocking;
}

void Fireball::die()
{
    dead = true;

    xa = -facing * 4;
    ya = -10;
    deadTime = 100;
}

bool Fireball::move(float xa, float ya)
{
    LevelScene* world = Sprite::levelScene;

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

        if (avoidCliffs && onGround && !world->level->isBlocking((int) ((x + xa + width) / 32), (int) ((y) / 32 + 1), xa, 1)) collide = true;
    }
    if (xa < 0)
    {
        if (isBlocking(x + xa - width, y + ya - height, xa, ya)) collide = true;
        if (isBlocking(x + xa - width, y + ya - height / 2, xa, ya)) collide = true;
        if (isBlocking(x + xa - width, y + ya, xa, ya)) collide = true;

        if (avoidCliffs && onGround && !world->level->isBlocking((int) ((x + xa - width) / 32), (int) ((y) / 32 + 1), xa, 1)) collide = true;
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
            x = (int) ((x + width) / 32 + 1) * 32 - width - 1;
            this->xa = 0;
        }
        if (ya < 0)
        {
            y = (int) ((y - height) / 32) * 32 + height;
            this->ya = 0;
        }
        if (ya > 0)
        {
            y = (int) (y / 32 + 1) * 32 - 1;
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
