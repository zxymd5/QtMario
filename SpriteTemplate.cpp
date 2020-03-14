#include "SpriteTemplate.h"
#include "LevelScene.h"
#include "Enemy.h"
#include "FlowerEnemy.h"
#include <iostream>

SpriteTemplate::SpriteTemplate(int type, bool winged)
{
	this->type = type;
	this->winged = winged;
	lastVisibleTick = -1;
	isDead = false;
}

void SpriteTemplate::spawn(LevelScene* world, int x, int y, int dir)
{
	if (isDead)
		return;
    if (type == Enemy::ENEMY_FLOWER)
    {
        sprite = new FlowerEnemy(x*32+30, y*32+48);
    }
    else
    {
        sprite = new Enemy(x*32+16, y*32+30, dir, type, winged);
    }
    sprite->spriteTemplate = this;
    world->addSprite(sprite);

}
