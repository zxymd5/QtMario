#include "Sprite.h"
#include "LevelScene.h"
#include "MarioComponent.h"
#include "SpriteTemplate.h"
#include "Art.h"
#include "BulletBill.h"
#include "Shell.h"
#include "Mario.h"
#include "Fireball.h"

LevelScene* Sprite::levelScene = nullptr;

Sprite::Sprite()
{

}

Sprite::~Sprite()
{

}

void Sprite::move()
{
    x += xa;
    y += ya;
}

void Sprite::tick()
{
    xOld = x;
    yOld = y;
    move();
}

void Sprite::tickNoMove()
{
    xOld = x;
    yOld = y;
}

void Sprite::collideCheck()
{

}

void Sprite::bumpCheck(int xTile, int yTile)
{

}

void Sprite::release(Mario *mario)
{

}

void Sprite::render(QPainter& painter)
{
    if(!visible) {
        return;
    }

    int xPixel = (int)(xOld) - xPicO;
    int yPixel = (int)(yOld) - yPicO;
    QImage img = sheet[xPic][yPic].mirrored(xFlipPic, yFlipPic);
    QRectF rect(xPixel, yPixel, 2 * img.width(), 2 * img.height());
    painter.drawImage(rect, img);
}

bool Sprite::shellCollideCheck(Shell *shell)
{
    return false;
}

bool Sprite::fireballCollideCheck(Fireball *fireball)
{
    return false;
}
