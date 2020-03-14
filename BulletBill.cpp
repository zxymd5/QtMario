#include "BulletBill.h"
#include "Art.h"
#include "Sparkle.h"
#include "LevelScene.h"
#include "Mario.h"
#include "Shell.h"
#include "Fireball.h"
#include "SimpleAudioEngine.h"
#include <math.h>

BulletBill::BulletBill(float x, float y, int dir)
{
    sheet = Art::enemies;
    this->x = x;
    this->y = y;
    xPicO = 16;
    yPicO = 62;

    height = 24;
    facing = 0;
    wPic = 32;
    yPic = 5;

    xPic = 0;
    ya = -10;
    this->facing = dir;
}

void BulletBill::move()
{
    if (deadTime > 0)
    {
        deadTime--;

        if (deadTime == 0)
        {
            deadTime = 1;
            for (int i = 0; i < 8; i++)
            {
                Sprite::levelScene->addSprite(new Sparkle(x + rand() % 32 - 16 + 8,
                                                          y - rand() % 16 + 8,
                                                          rand() % 1000 / 500.00f - 1,
                                                          rand() % 1000 / 1000.00f  * -1, 0, 1, 5));
            }
            Sprite::levelScene->removeSprite(this);
        }

        x += xa;
        y += ya;
        ya *= 0.95;
        ya += 2;

        return;
    }

    float sideWaysSpeed = 8.0f;

    xa = facing * sideWaysSpeed;
    xFlipPic = facing == -1;
    move(xa, 0);
}

void BulletBill::collideCheck()
{
    if (dead) return;

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
                dead = true;

                xa = 0;
                ya = 1;
                deadTime = 100;
            }
            else
            {
                mario->getHurt();
            }
        }
    }
}

bool BulletBill::shellCollideCheck(Shell *shell)
{
    if (deadTime != 0) return false;

    float xD = shell->x - x;
    float yD = shell->y - y;

    if (xD > -32 && xD < 32)
    {
        if (yD > -height && yD < shell->height)
        {
            SimpleAudioEngine::getInstance()->playEffect(Art::samples[Art::SAMPLE_MARIO_KICK].c_str());

            dead = true;

            xa = 0;
            ya = 2;
            deadTime = 100;

            return true;
        }
    }
    return false;
}

bool BulletBill::fireballCollideCheck(Fireball *fireball)
{
    if (deadTime != 0) return false;

    float xD = fireball->x - x;
    float yD = fireball->y - y;

    if (xD > -32 && xD < 32)
    {
        if (yD > -height && yD < fireball->height)
        {
            return true;
        }
    }
    return false;
}

bool BulletBill::move(float xa, float ya)
{
    x += xa;
    return true;
}
