#include "Particle.h"
#include "Art.h"
#include "LevelScene.h"

Particle::Particle(int x, int y, float xa, float ya)
    : Particle(x, y, xa, ya, rand() % 2, 0)
{

}

Particle::Particle(int x, int y, float xa, float ya, int xPic, int yPic)
{
    sheet = Art::particles;
    this->x = x;
    this->y = y;
    this->xa = xa;
    this->ya = ya;
    this->xPic = xPic;
    this->yPic = yPic;
    this->xPicO = 8;
    this->yPicO = 8;

    wPic = 16;
    hPic = 16;
    life = 10;
}

void Particle::move()
{
    if (life--<0) Sprite::levelScene->removeSprite(this);
    x+=xa;
    y+=ya;
    ya*=0.95f;
    ya+=6;
}
