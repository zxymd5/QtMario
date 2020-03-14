#include "Enemy.h"
#include "Art.h"
#include "LevelScene.h"
#include "Sparkle.h"
#include "Mario.h"
#include "SpriteTemplate.h"
#include "Shell.h"
#include "Fireball.h"
#include "SimpleAudioEngine.h"

#include <math.h>
#include <QDebug>

float Enemy::GROUND_INERTIA = 0.89f;
float Enemy::AIR_INERTIA = 0.89f;
Enemy::Enemy(int x, int y, int dir, int type, bool winged)
{
    this->type = type;
    sheet = Art::enemies;
    this->winged = winged;

    this->x = x;
    this->y = y;
    xPicO = 16;
    yPicO = 62;

    avoidCliffs = type == Enemy::ENEMY_RED_KOOPA;

    noFireballDeath = type == Enemy::ENEMY_SPIKY;

    yPic = type;
    if (yPic > 1) height = 24;
    facing = dir;
    if (facing == 0) facing = 1;
    this->wPic = 32;
}

void Enemy::move()
{
    wingTime++;
    if (deadTime > 0)
    {
        deadTime--;

        if (deadTime == 0)
        {
            deadTime = 1;
            for (int i = 0; i < 8; i++)
            {
                Sprite::levelScene->addSprite(new Sparkle((x + rand() % 32 - 16) + 8, (y - rand() % 16) + 8,
                                                          (float) (rand() % 1000 / 500.00 - 1),
                                                          (float) (-rand() % 1000 / 1000.00), 0, 1, 5));
            }
            Sprite::levelScene->removeSprite(this);
        }

        if (flyDeath)
        {
            x += xa;
            y += ya;
            ya *= 0.95;
            ya += 2;
        }
        return;
    }


    float sideWaysSpeed = 3.8f;

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

    int runFrame = ((int) (runTime / 40)) % 2;

    if (!onGround)
    {
        runFrame = 1;
    }


    if (!move(xa, 0)) facing = -facing;
    onGround = false;
    move(0, ya);

    ya *= winged ? 0.95f : 0.85f;
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
        if (winged)
        {
            ya += 1.2f;
        }
        else
        {
            ya += 4;
        }
    }
    else if (winged)
    {
        ya = -20;
    }

    if (winged) runFrame = wingTime / 4 % 2;

    xPic = runFrame;
}

bool Enemy::move(float xa, float ya)
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
            jumpTime = 0;
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

void Enemy::bumpCheck(int xTile, int yTile)
{
    if (deadTime != 0) return;

    if (x + width > xTile * 32 && x - width < xTile * 32 + 32 && yTile == (int) ((y - 1) / 32))
    {
        SimpleAudioEngine::getInstance()->playEffect(Art::samples[Art::SAMPLE_MARIO_KICK].c_str());

        xa = -Sprite::levelScene->mario.get()->facing * 4;
        ya = -10;
        flyDeath = true;
        if (spriteTemplate != nullptr) spriteTemplate->isDead = true;
        deadTime = 100;
        winged = false;
        hPic = -hPic;
        yPicO = -yPicO + 32;
    }
}

void Enemy::collideCheck()
{
    if (deadTime != 0)
    {
        return;
    }

    Mario* mario = Sprite::levelScene->mario.get();

    float xMarioD = mario->x - x;
    float yMarioD = mario->y - y;

    if (xMarioD > -width*2-8 && xMarioD < width*2+8)
    {
        if (yMarioD > -height && yMarioD < mario->height)
        {
            if (type != Enemy::ENEMY_SPIKY && mario->ya > 0 && yMarioD <= 0 && (!mario->onGround || !mario->wasOnGround))
            {
                mario->stomp(this);
                if (winged)
                {
                    winged = false;
                    ya = 0;
                }
                else
                {
                    this->yPicO = 62 - (64 - 16);
                    hPic = 16;
                    if (spriteTemplate != nullptr) spriteTemplate->isDead = true;
                    deadTime = 10;
                    winged = false;
                    visible = false;

                    if (type == Enemy::ENEMY_RED_KOOPA)
                    {
                        Sprite::levelScene->addSprite(new Shell(x, y, 0));
                    }
                    else if (type == Enemy::ENEMY_GREEN_KOOPA)
                    {
                        Sprite::levelScene->addSprite(new Shell(x, y, 1));
                    }
                }
            }
            else
            {
                mario->getHurt();
            }
        }
    }
}

bool Enemy::shellCollideCheck(Shell *shell)
{
    if (deadTime != 0) return false;

    float xD = shell->x - x;
    float yD = shell->y - y;

    if (xD > -32 && xD < 32)
    {
        if (yD > -height && yD < shell->height)
        {
            SimpleAudioEngine::getInstance()->playEffect(Art::samples[Art::SAMPLE_MARIO_KICK].c_str());

            xa = shell->facing * 4;
            ya = -10;
            flyDeath = true;
            if (spriteTemplate != nullptr) spriteTemplate->isDead = true;
            deadTime = 100;
            winged = false;
            hPic = -hPic;
            yPicO = -yPicO + 32;
            return true;
        }
    }
    return false;
}

void Enemy::render(QPainter &painter)
{
    if (winged)
    {
        int xPixel = (int) (xOld) - xPicO;
        int yPixel = (int) (yOld) - yPicO;

        if (type == Enemy::ENEMY_GREEN_KOOPA || type == Enemy::ENEMY_RED_KOOPA)
        {
        }
        else
        {
            xFlipPic = !xFlipPic;

            qDebug() << "first: " << wingTime << "\n";

            QImage img = sheet[wingTime / 4 % 2][4].mirrored(xFlipPic, yFlipPic);
            QRectF rect(xPixel + (xFlipPic ? wPic / 2 : 0) + (xFlipPic ? 16 : -16),
                        yPixel + (yFlipPic ? hPic / 2 : 0) - 16,
                        2 * img.width(), 2 * img.height());
            painter.drawImage(rect, img);
            xFlipPic = !xFlipPic;
        }
    }

    Sprite::render(painter);

    if (winged)
    {
        int xPixel = (int) (xOld) - xPicO;
        int yPixel = (int) (yOld) - yPicO;

        qDebug() << "second: " << wingTime << "\n";
        QImage img = sheet[wingTime / 4 % 2][4].mirrored(xFlipPic, yFlipPic);

        if (type == Enemy::ENEMY_GREEN_KOOPA || type == Enemy::ENEMY_RED_KOOPA)
        {
            QRectF rect(xPixel + (xFlipPic ? wPic / 2 : 0) + (xFlipPic ? 16 : -16),
                        yPixel + (yFlipPic ? hPic / 2 : 0) - 16,
                        2 * img.width(), 2 * img.height());
            painter.drawImage(rect, img);
        }
        else
        {
            QRectF rect(xPixel + (xFlipPic ? wPic / 2 : 0) + (xFlipPic ? 16 : -16),
                        yPixel + (yFlipPic ? hPic / 2 : 0) - 12,
                        2 * img.width(), 2 * img.height());
            painter.drawImage(rect, img);
        }
    }
}

bool Enemy::fireballCollideCheck(Fireball *fireball)
{
    if (deadTime != 0) return false;

    float xD = fireball->x - x;
    float yD = fireball->y - y;

    if (xD > -32 && xD < 32)
    {
        if (yD > -height && yD < fireball->height)
        {
            if (noFireballDeath) return true;
            SimpleAudioEngine::getInstance()->playEffect(Art::samples[Art::SAMPLE_MARIO_KICK].c_str());

            xa = fireball->facing * 4;
            ya = -10;
            flyDeath = true;
            if (spriteTemplate != nullptr) spriteTemplate->isDead = true;
            deadTime = 100;
            winged = false;
            hPic = -hPic;
            yPicO = -yPicO + 32;
            return true;
        }
    }
    return false;
}

bool Enemy::isBlocking(float _x, float _y, float xa, float ya)
{
    int x = (int) (_x / 32);
    int y = (int) (_y / 32);
    if (x == (int) (this->x / 32) && y == (int) (this->y / 32)) return false;

    bool blocking = Sprite::levelScene->level->isBlocking(x, y, xa, ya);

    return blocking;
}
