#include "Shell.h"
#include "Art.h"
#include "LevelScene.h"
#include "Sparkle.h"
#include "Mario.h"
#include "Fireball.h"
#include "SpriteTemplate.h"
#include "SimpleAudioEngine.h"
#include <math.h>

float Shell::GROUND_INERTIA = 0.89f;
float Shell::AIR_INERTIA = 0.89f;

Shell::Shell(float x, float y, int type)
{
    sheet = Art::enemies;

    this->x = x;
    this->y = y;
    xPicO = 16;
    yPicO = 62;

    yPic = type;
    height = 24;
    facing = 0;
    wPic = 32;

    xPic = 4;
    ya = -10;
}

void Shell::move()
{
    if (carried)
    {
        Sprite::levelScene->checkShellCollide(this);
        return;
    }

    if (deadTime > 0)
    {
        deadTime--;

        if (deadTime == 0)
        {
            deadTime = 1;
            for (int i = 0; i < 8; i++)
            {
                Sprite::levelScene->addSprite(new Sparkle((x + rand() % 32 - 16) + 8,
                                                          (y - rand() % 16) + 8,
                                                          rand() % 1000 / 500.00f - 1,
                                                          rand() % 1000 / 1000.00f * -1, 0, 1, 5));
            }
            Sprite::levelScene->removeSprite(this);
        }

        x += xa;
        y += ya;
        ya *= 0.95;
        ya += 2;

        return;
    }

    if (facing != 0) anim++;

    float sideWaysSpeed = 22.0f;

    if (xa > 4)
    {
        facing = 1;
    }
    if (xa < -4)
    {
        facing = -1;
    }

    xa = facing * sideWaysSpeed;

    if (facing != 0)
    {
        Sprite::levelScene->checkShellCollide(this);
    }

    xFlipPic = facing == -1;

    runTime += (fabs(xa)) + 10;

    xPic = (anim / 2) % 4 + 3;



    if (!move(xa, 0))
    {
        SimpleAudioEngine::getInstance()->playEffect(Art::samples[Art::SAMPLE_SHELL_BUMP].c_str());

        facing = -facing;
    }
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

void Shell::bumpCheck(int xTile, int yTile)
{
    if (x + width > xTile * 32 && x - width < xTile * 32 + 32 && yTile == (int) ((y - 1) / 32))
    {
        facing = -Sprite::levelScene->mario.get()->facing;
        ya = -20;
    }
}

void Shell::collideCheck()
{
    if (carried || dead || deadTime>0) return;

    Mario* mario = Sprite::levelScene->mario.get();

    float xMarioD = mario->x - x;
    float yMarioD = mario->y - y;
    float w = 32;
    if (xMarioD > -w && xMarioD < w)
    {
        if (yMarioD > -height && yMarioD < mario->height)
        {
            if (mario->ya > 0 && yMarioD <= 0 && (!mario->onGround || !mario->wasOnGround))
            {
                mario->stomp(this);
                if (facing != 0)
                {
                    xa = 0;
                    facing = 0;
                }
                else
                {
                    facing = mario->facing;
                }
            }
            else
            {
                if (facing != 0)
                {
                    mario->getHurt();
                }
                else
                {
                    mario->kick(this);
                    facing = mario->facing;
                }
            }
        }
    }
}

bool Shell::shellCollideCheck(Shell *shell)
{
    if (deadTime != 0) return false;

    Mario* mario = Sprite::levelScene->mario.get();

    float xD = shell->x - x;
    float yD = shell->y - y;

    if (xD > -32 && xD < 32)
    {
        if (yD > -height && yD < shell->height)
        {
            SimpleAudioEngine::getInstance()->playEffect(Art::samples[Art::SAMPLE_MARIO_KICK].c_str());

            if (mario->carried == shell || mario->carried == this)
            {
                mario->carried = nullptr;
            }

            die();
            shell->die();
            return true;
        }
    }
    return false;
}

bool Shell::fireballCollideCheck(Fireball *fireball)
{
    if (deadTime != 0) return false;

    float xD = fireball->x - x;
    float yD = fireball->y - y;

    if (xD > -32 && xD < 32)
    {
        if (yD > -height && yD < fireball->height)
        {
            if (facing!=0) return true;

            SimpleAudioEngine::getInstance()->playEffect(Art::samples[Art::SAMPLE_MARIO_KICK].c_str());

            xa = fireball->facing * 4;
            ya = -10;
            if (spriteTemplate != nullptr) spriteTemplate->isDead = true;
            deadTime = 100;
            hPic = -hPic;
            yPicO = -yPicO + 32;
            return true;
        }
    }
    return false;
}

void Shell::die()
{
    dead = true;

    carried = false;

    xa = -facing * 4;
    ya = -10;
    deadTime = 100;
}

void Shell::release(Mario *mario)
{
    carried = false;
    facing = mario->facing;
    x += facing * 16;
}

bool Shell::move(float xa, float ya)
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

bool Shell::isBlocking(float _x, float _y, float xa, float ya)
{
    int x = (int) (_x / 32);
    int y = (int) (_y / 32);
    if (x == (int) (this->x / 32) && y == (int) (this->y / 32)) return false;

    bool blocking = Sprite::levelScene->level->isBlocking(x, y, xa, ya);

    if (blocking && ya == 0 && xa!=0)
    {
        Sprite::levelScene->bump(x, y, true);
    }

    return blocking;
}
