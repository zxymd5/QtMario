#ifndef MARIO_BGLEVELGENERATOR_H
#define MARIO_BGLEVELGENERATOR_H

#include "Level.h"

class BgLevelGenerator
{
public:
    static void createLevel(int width, int height, bool distant, int type, Level* level);
    void createLevel(Level* level);

	BgLevelGenerator(int width, int height, bool distant, int type);

private:
	int width;
	int height;
	bool distant;
	int type;
};

#endif
