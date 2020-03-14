#include "Mario.h"
#include "LevelScene.h"
#include "Art.h"
#include "Sparkle.h"
#include "BulletBill.h"
#include "Enemy.h"
#include "Shell.h"
#include "Fireball.h"
#include "SimpleAudioEngine.h"

#include<math.h>
#include<iostream>

using namespace std;

bool Mario::large = false;
bool Mario::fire = false;
int Mario::coins = 0;
int Mario::lives = 3;
string Mario::levelString = "none";
Mario* Mario::instance = nullptr;
float Mario::GROUND_INERTIA = 0.89f;
float Mario::AIR_INERTIA = 0.89f;

Mario::Mario()
{
    Mario::instance = this;
    x = 64;
    y = 0;
    yPic = 0;
    facing = 1;
    setLarge(Mario::large, Mario::fire);
}

void Mario::setLarge(bool large, bool fire)
{
    if (fire) large = true;
    if (!large) fire = false;

    lastLarge = Mario::large;
    lastFire = Mario::fire;

    Mario::large = large;
    Mario::fire = fire;

    newLarge = Mario::large;
    newFire = Mario::fire;

    blink(true);
}

void Mario::blink(bool on)
{
    Mario::large = on?newLarge:lastLarge;
    Mario::fire = on?newFire:lastFire;

    if (large)
    {
        sheet = Art::mario;
        if (fire){
            sheet = Art::fireMario;
        }
        xPicO = 32;
        yPicO = 62;
        wPic = hPic = 32;
    }
    else
    {
        sheet = Art::smallMario;
        xPicO = 16;
        yPicO = 30;
        wPic = hPic = 16;
    }

    calcPic();
}

void Mario::calcPic()
{
    int runFrame = 0;

    if (large)
    {
        runFrame = ((int) (runTime / 40)) % 4;
        if (runFrame == 3) runFrame = 1;
        if (carried == nullptr && fabs(xa) > 20) runFrame += 3;
        if (carried != nullptr) runFrame += 10;
        if (!onGround)
        {
            if (carried != nullptr) runFrame = 12;
            else if (fabs(xa) > 20) runFrame = 7;
            else runFrame = 6;
        }
    }
    else
    {
        runFrame = ((int) (runTime / 40)) % 2;
        if (carried == nullptr && fabs(xa) > 20) runFrame += 2;
        if (carried != nullptr) runFrame += 8;
        if (!onGround)
        {
            if (carried != nullptr) runFrame = 9;
            else if (fabs(xa) > 20) runFrame = 5;
            else runFrame = 4;
        }
    }

    if (onGround && ((facing == -1 && xa > 0) || (facing == 1 && xa < 0)))
    {
        if (xa > 2 || xa < -2) runFrame = large ? 9 : 7;

        if (xa > 6 || xa < -6)
        {
            for (int i = 0; i < 3; i++)
            {
                Sprite::levelScene->addSprite(new Sparkle(x + rand() % 16 - 8, y + rand() % 8,
                                                          rand() % 1000 / 250.00 - 2,
                                                          rand() % 1000 / 1000.00 * -1, 0, 1, 5));
            }
        }
    }

    if (large)
    {
        if (ducking) runFrame = 14;
        height = ducking ? 24 : 48;
    }
    else
    {
        height = 24;
    }

    xPic = runFrame;
}

void Mario::move()
{
    if (winTime > 0)
    {
        winTime++;

        xa = 0;
        ya = 0;
        return;
    }

    if (deathTime > 0)
    {
        deathTime++;
        if (deathTime < 11)
        {
            xa = 0;
            ya = 0;
        }
        else if (deathTime == 11)
        {
            ya = -30;
        }
        else
        {
            ya += 4;
        }
        x += xa;
        y += ya;
        return;
    }

    if (powerUpTime != 0)
    {
        if (powerUpTime > 0)
        {
            powerUpTime--;
            blink(((powerUpTime / 3) & 1) == 0);
        }
        else
        {
            powerUpTime++;
            blink(((-powerUpTime / 3) & 1) == 0);
        }

        if (powerUpTime == 0)
        {
            Sprite::levelScene->paused = false;
        }

        calcPic();
        return;
    }

    if (invulnerableTime > 0) invulnerableTime--;
    visible = ((invulnerableTime / 2) & 1) == 0;

    wasOnGround = onGround;
    float sideWaysSpeed = Scene::keys[Qt::Key_A] ? 3.0f : 1.5f;

    if (onGround)
    {
        if (Scene::keys[Qt::Key_Down] && large)
        {
            ducking = true;
        }
        else
        {
            ducking = false;
        }
    }

    if (xa > 4)
    {
        facing = 1;
    }
    if (xa < -4)
    {
        facing = -1;
    }

    if (Scene::keys[Qt::Key_S] || (jumpTime < 0 && !onGround && !sliding))
    {
        if (jumpTime < 0)
        {
            xa = xJumpSpeed;
            ya = -jumpTime * yJumpSpeed;
            jumpTime++;
        }
        else if(onGround && mayJump)
        {
            SimpleAudioEngine::getInstance()->playEffect(Art::samples[Art::SAMPLE_MARIO_JUMP].c_str());
            xJumpSpeed = 0;
            yJumpSpeed = -4.0f;
            jumpTime = 7;
            ya = jumpTime * yJumpSpeed;
            onGround = false;
            sliding = false;
        }
        else if (sliding && mayJump)
        {
            SimpleAudioEngine::getInstance()->playEffect(Art::samples[Art::SAMPLE_MARIO_JUMP].c_str());
            xJumpSpeed = -facing * 14.0f;
            yJumpSpeed = -4.0f;
            jumpTime = -6;
            xa = xJumpSpeed;
            ya = -jumpTime * yJumpSpeed;
            onGround = false;
            sliding = false;
            facing = -facing;
        }
        else if (jumpTime > 0)
        {
            xa += xJumpSpeed;
            ya = jumpTime * yJumpSpeed;
            jumpTime--;
        }
    }
    else
    {
        jumpTime = 0;
    }

    if (Scene::keys[Qt::Key_Left] && !ducking) {
        if (facing == 1) sliding = false;
        xa -= sideWaysSpeed;
        if (jumpTime >= 0) facing = -1;
    }

    if (Scene::keys[Qt::Key_Right] && !ducking) {
        if (facing == -1) sliding = false;
        xa += sideWaysSpeed;
        if (jumpTime >= 0) facing = 1;
    }

    if ((!Scene::keys[Qt::Key_Left] && !Scene::keys[Qt::Key_Right])
            || ducking || ya < 0 || onGround)
    {
        sliding = false;
    }

    if (Scene::keys[Qt::Key_A] && canShoot && Mario::fire && Sprite::levelScene->fireballsOnScreen < 2)
    {
        SimpleAudioEngine::getInstance()->playEffect(Art::samples[Art::SAMPLE_MARIO_FIREBALL].c_str());
        Sprite::levelScene->addSprite(new Fireball(x + facing * 12, y-40, facing));
    }

    canShoot = !Scene::keys[Qt::Key_A];
    mayJump = (onGround || sliding) && !Scene::keys[Qt::Key_S];
    xFlipPic = facing == -1;

    runTime += fabs(xa) + 10;
    if (fabs(xa) < 1.0f)
    {
        runTime = 0;
        xa = 0;
    }

    calcPic();

    if (sliding)
    {
        for (int i = 0; i < 1; i++)
        {
            Sprite::levelScene->addSprite(new Sparkle(x + rand() % 8 - 4 + facing * 16,
                                                      y + rand() % 8 - 48,
                                                      rand() % 1000 / 500.00 - 2,
                                                      rand() % 1000 / 1000.00, 0, 1, 5));
        }
        ya *= 0.5f;
    }

    onGround = false;

    move(xa, 0);
    move(0, ya);

    if(!onGround)
    {
        int b = 8;
    }

    if(y > Sprite::levelScene->level->height * 32 + 32)
    {
        die();
    }

    if (x < 0)
    {
        x = 0;
        xa = 0;
    }

    if (x > Sprite::levelScene->level->xExit * 32)
    {
        win();
    }

    if (x > Sprite::levelScene->level->width * 32)
    {
        x = Sprite::levelScene->level->width * 32;
        xa = 0;
    }

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
        ya += 6;
    }

    if (carried != nullptr)
    {
        carried->x = x + facing * 16;
        carried->y = y - 4;
        if (!Scene::keys[Qt::Key_A])
        {
            carried->release(this);
            carried = nullptr;
        }
    }
}

bool Mario::move(float xa, float ya)
{
    while (xa > 16 && fabs(xa - 16) > 0)
    {
        if (!move(16, 0)) return false;
        xa -= 16;
    }
    while (xa < -16 && fabs(xa + 16) > 0)
    {
        if (!move(-16, 0)) return false;
        xa += 16;
    }
    while (ya > 16 && fabs(ya - 16) > 0)
    {
        if (!move(0, 16)) return false;
        ya -= 16;
    }
    while (ya < -16 && fabs(ya + 16) > 0)
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
        sliding = true;
        if (isBlocking(x + xa + width, y + ya - height, xa, ya)) collide = true;
        else sliding = false;
        if (isBlocking(x + xa + width, y + ya - height / 2, xa, ya)) collide = true;
        else sliding = false;
        if (isBlocking(x + xa + width, y + ya, xa, ya)) collide = true;
        else sliding = false;
    }
    if (xa < 0)
    {
        sliding = true;
        if (isBlocking(x + xa - width, y + ya - height, xa, ya)) collide = true;
        else sliding = false;
        if (isBlocking(x + xa - width, y + ya - height / 2, xa, ya)) collide = true;
        else sliding = false;
        if (isBlocking(x + xa - width, y + ya, xa, ya)) collide = true;
        else sliding = false;
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
            y = (int) ((y - 1) / 32 + 1) * 32 - 1;
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

void Mario::win()
{
    xDeathPos = (int) x;
    yDeathPos = (int) y;
    Sprite::levelScene->paused = true;
    winTime = 1;
    SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
    SimpleAudioEngine::getInstance()->playEffect(Art::samples[Art::SAMPLE_LEVEL_EXIT].c_str());
}

void Mario::die()
{
    xDeathPos = (int) x;
    yDeathPos = (int) y;
    Sprite::levelScene->paused = true;
    deathTime = 1;
    SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
    SimpleAudioEngine::getInstance()->playEffect(Art::samples[Art::SAMPLE_MARIO_DEATH].c_str());
}

void Mario::stomp(BulletBill *bill)
{
    if (deathTime > 0 || Sprite::levelScene->paused) return;

    float targetY = bill->y - bill->height / 2;
    move(0, targetY - y);

    SimpleAudioEngine::getInstance()->playEffect(Art::samples[Art::SAMPLE_MARIO_KICK].c_str());
    xJumpSpeed = 0;
    yJumpSpeed = -4.0f;
    jumpTime = 8;
    ya = jumpTime * yJumpSpeed;
    onGround = false;
    sliding = false;
    invulnerableTime = 1;
}

void Mario::stomp(Enemy *enemy)
{
    if (deathTime > 0 || Sprite::levelScene->paused) return;

    float targetY = enemy->y - enemy->height / 2;
    move(0, targetY - y);

    SimpleAudioEngine::getInstance()->playEffect(Art::samples[Art::SAMPLE_MARIO_KICK].c_str());
    xJumpSpeed = 0;
    yJumpSpeed = -4.0f;
    jumpTime = 8;
    ya = jumpTime * yJumpSpeed;
    onGround = false;
    sliding = false;
    invulnerableTime = 1;
}

void Mario::stomp(Shell *shell)
{
    LevelScene* world = Sprite::levelScene;

    if (deathTime > 0 || world->paused) return;

    if (Scene::keys[Qt::Key_A] && shell->facing == 0)
    {
        carried = shell;
        shell->carried = true;
    }
    else
    {
        float targetY = shell->y - shell->height / 2;
        move(0, targetY - y);

        SimpleAudioEngine::getInstance()->playEffect(Art::samples[Art::SAMPLE_MARIO_KICK].c_str());
        xJumpSpeed = 0;
        yJumpSpeed = -4.0f;
        jumpTime = 8;
        ya = jumpTime * yJumpSpeed;
        onGround = false;
        sliding = false;
        invulnerableTime = 1;
    }
}

void Mario::kick(Shell *shell)
{
    LevelScene* world = Sprite::levelScene;
    if (deathTime > 0 || world->paused) return;

    if (Scene::keys[Qt::Key_A])
    {
        carried = shell;
        shell->carried = true;
    }
    else
    {
        SimpleAudioEngine::getInstance()->playEffect(Art::samples[Art::SAMPLE_MARIO_KICK].c_str());
        invulnerableTime = 1;
    }
}

void Mario::getHurt()
{
    if (deathTime > 0 || Sprite::levelScene->paused) return;
    if (invulnerableTime > 0) return;

    if (large)
    {
        Sprite::levelScene->paused = true;
        powerUpTime = -3 * 6;
        SimpleAudioEngine::getInstance()->playEffect(Art::samples[Art::SAMPLE_MARIO_POWER_DOWN].c_str());
        if (fire)
        {
            Sprite::levelScene->mario->setLarge(true, false);
        }
        else
        {
            Sprite::levelScene->mario->setLarge(false, false);
        }
        invulnerableTime = 32;
    }
    else
    {
        die();
    }
}

void Mario::getFlower()
{
    LevelScene* world = Sprite::levelScene;

    if (deathTime > 0 || world->paused) return;

    if (!fire)
    {
        world->paused = true;
        powerUpTime = 3 * 6;
        SimpleAudioEngine::getInstance()->playEffect(Art::samples[Art::SAMPLE_MARIO_POWER_UP].c_str());
        world->mario->setLarge(true, true);
    }
    else
    {
        Mario::getCoin();
        SimpleAudioEngine::getInstance()->playEffect(Art::samples[Art::SAMPLE_GET_COIN].c_str());
    }
}

void Mario::getMushroom()
{
    if (deathTime > 0 || Sprite::levelScene->paused) return;

    if (!large)
    {
        Sprite::levelScene->paused = true;
        powerUpTime = 3 * 6;
        SimpleAudioEngine::getInstance()->playEffect(Art::samples[Art::SAMPLE_MARIO_POWER_UP].c_str());
        Sprite::levelScene->mario->setLarge(true, false);
    }
    else
    {
        Mario::getCoin();
        SimpleAudioEngine::getInstance()->playEffect(Art::samples[Art::SAMPLE_GET_COIN].c_str());
    }
}

void Mario::get1Up()
{
    SimpleAudioEngine::getInstance()->playEffect(Art::samples[Art::SAMPLE_MARIO_1UP].c_str());
    lives++;
    if (lives==99)
    {
        lives = 99;
    }
}

void Mario::getCoin()
{
    coins++;
    if (coins==100)
    {
        coins = 0;
        get1Up();
    }
}

void Mario::resetStatic()
{
    large = false;
    fire = false;
    coins = 0;
    lives = 3;
    levelString = "none";
}

bool Mario::isBlocking(float _x, float _y, float xa, float ya)
{
    int x = (int) (_x / 32);
    int y = (int) (_y / 32);
    if (x == (int) (this->x / 32) && y == (int) (this->y / 32)) return false;

    bool blocking = Sprite::levelScene->level->isBlocking(x, y, xa, ya);

    uint8_t block = Sprite::levelScene->level->getBlock(x, y);

    if (((Level::TILE_BEHAVIORS[block & 0xff]) & Level::BIT_PICKUPABLE) > 0)
    {
        Mario::getCoin();
        SimpleAudioEngine::getInstance()->playEffect(Art::samples[Art::SAMPLE_GET_COIN].c_str());
        Sprite::levelScene->level->setBlock(x, y, (uint8_t) 0);
        for (int xx = 0; xx < 2; xx++)
        {
            for (int yy = 0; yy < 2; yy++)
            {
                Sprite::levelScene->addSprite(new Sparkle(x * 32 + xx * 16 + rand() % 16,
                                                          y * 32 + yy * 16 + rand() % 16, 0, 0, 0, 2, 5));
            }
        }
    }

    if (blocking && ya < 0)
    {
        Sprite::levelScene->bump(x, y, large);
    }

    return blocking;
}
