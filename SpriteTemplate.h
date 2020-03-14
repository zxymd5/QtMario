#ifndef MARIO_SPRITETEMPLATE_H
#define MARIO_SPRITETEMPLATE_H

#include "Sprite.h"
#include <memory>
class LevelScene;

using namespace std;
class SpriteTemplate
{
public:
    long long lastVisibleTick;
	bool isDead;
    Sprite* sprite = nullptr;

	SpriteTemplate(int type, bool winged);
    void spawn(LevelScene* world, int x, int y, int dir);
    int type;
private:
	bool winged;
};

#endif
