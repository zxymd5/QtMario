#include "LevelGenerator.h"
#include "Enemy.h"
#include "SpriteTemplate.h"

LevelGenerator::LevelGenerator(int width, int height)
{
	this->width = width;
	this->height = height;
}

void LevelGenerator::createLevel(int width, int height, int difficulty, int type, Level* level)
{
	LevelGenerator levelGenerator(width, height);
    levelGenerator.createLevel(difficulty, type, level);
}

void LevelGenerator::createLevel(int difficulty, int type, Level* level)
{
    this->type = type;
    this->difficulty = difficulty;
    odds[ODDS_STRAIGHT] = 20;
    odds[ODDS_HILL_STRAIGHT] = 10;
    odds[ODDS_TUBES] = 2 + 1 * difficulty;
    odds[ODDS_JUMP] = 2 * difficulty;
    odds[ODDS_CANNONS] = -10 + 5 * difficulty;

    if (type != LevelGenerator::TYPE_OVERGROUND)
    {
        odds[ODDS_HILL_STRAIGHT] = 0;
    }

    for (int i = 0; i < 5; i++)
    {
        if (odds[i] < 0) odds[i] = 0;
        totalOdds += odds[i];
        odds[i] = totalOdds - odds[i];
    }

    int length = 0;
    length += buildStraight(0, level->width, true, level);
    while (length < level->width - 64)
    {
        length += buildZone(length, level->width - length, level);
    }

    int floor = height - 1 - rand() % 4;

    level->xExit = length + 8;
    level->yExit = floor;

    for (int x = length; x < level->width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            if (y >= floor)
            {
                level->setBlock(x, y, (uint8_t)(1 + 9 * 16));
            }
        }
    }

    if (type == LevelGenerator::TYPE_CASTLE || 
        type == LevelGenerator::TYPE_UNDERGROUND)
    {
        int ceiling = 0;
        int run = 0;
        for (int x = 0; x < level->width; x++)
        {
            if (run-- <= 0 && x > 4)
            {
                ceiling = rand() % 4;
                run = rand() % 4 + 4;
            }
            for (int y = 0; y < level->height; y++)
            {
                if ((x > 4 && y <= ceiling) || x < 1)
                {
                    level->setBlock(x, y, (uint8_t)(1 + 9 * 16));
                }
            }
        }
    }

    fixWalls(level);
}

int LevelGenerator::buildStraight(int xo, int maxLength, bool safe, Level* level)
{
    int length = rand() % 10 + 2;
    if (safe) length = 10 + rand() % 5;
    if (length > maxLength) length = maxLength;

    int floor = height - 1 - rand() % 4;
    for (int x = xo; x < xo + length; x++)
    {
        for (int y = 0; y < height; y++)
        {
            if (y >= floor)
            {
                level->setBlock(x, y, (uint8_t)(1 + 9 * 16));
            }
        }
    }

    if (!safe)
    {
        if (length > 5)
        {
            decorate(xo, xo + length, floor, level);
        }
    }

    return length;
}

void LevelGenerator::decorate(int x0, int x1, int floor, Level* level)
{
    if (floor < 1)
        return;
    bool rocks = true;
    addEnemyLine(x0 + 1, x1 - 1, floor - 1, level);
    int s = rand() % 4;
    int e = rand() % 4;

    if (floor - 2 > 0)
    {
        if ((x1 - 1 - e) - (x0 + 1 + s) > 1)
        {
            for (int x = x0 + 1 + s; x < x1 - 1 - e; x++)
            {
                level->setBlock(x, floor - 2, (uint8_t)(2 + 2 * 16));
            }
        }
    }

    s = rand() % 4;
    e = rand() % 4;

    if (floor - 4 > 0)
    {
        if ((x1 - 1 - e) - (x0 + 1 + s) > 2)
        {
            for (int x = x0 + 1 + s; x < x1 - 1 - e; x++)
            {
                if (rocks)
                {
                    if (x != x0 + 1 && x != x1 - 2 && rand() % 3 == 0)
                    {
                        if (rand() % 4 == 0)
                        {
                            level->setBlock(x, floor - 4, (uint8_t)(4 + 2 + 1 * 16));
                        }
                        else
                        {
                            level->setBlock(x, floor - 4, (uint8_t)(4 + 1 + 1 * 16));
                        }
                    }
                    else if (rand() % 4 == 0)
                    {
                        if (rand() % 4 == 0)
                        {
                            level->setBlock(x, floor - 4, (uint8_t)(2 + 1 * 16));
                        }
                        else
                        {
                            level->setBlock(x, floor - 4, (uint8_t)(1 + 1 * 16));
                        }
                    }
                    else
                    {
                        level->setBlock(x, floor - 4, (uint8_t)(0 + 1 * 16));
                    }
                }
            }
        }
    }
}

void LevelGenerator::addEnemyLine(int x0, int x1, int y, Level* level)
{
    for (int x = x0; x < x1; x++)
    {
        if (rand() % 35 < difficulty + 1)
        {
            int type = rand() % 4;
            if (difficulty < 1)
            {
                type = Enemy::ENEMY_GOOMBA;
            }
            else if (difficulty < 3)
            {
                type = rand() % 3;
            }
            level->setSpriteTemplate(x, y, type, rand() % 35 < difficulty);
        }

    }
}

void LevelGenerator::fixWalls(Level* level)
{
    vector<vector<bool>> blockMap;

    blockMap.resize(width + 1);
    for(int i = 0; i < width + 1; i++) {
        blockMap[i].resize(height + 1);
    }

    for(int i = 0; i < width + 1; i++) {
        for(int j = 0; j < height + 1; j++) {
            blockMap.at(i).at(j) = false;
        }
    }

    for (int x = 0; x < width + 1; x++)
    {
        for (int y = 0; y < height + 1; y++)
        {
            int blocks = 0;
            for (int xx = x - 1; xx < x + 1; xx++)
            {
                for (int yy = y - 1; yy < y + 1; yy++)
                {
                    if (level->getBlockCapped(xx, yy) == (uint8_t)(1 + 9 * 16)) blocks++;
                }
            }
            blockMap[x][y] = blocks == 4;
        }
    }
    blockify(level, blockMap, width + 1, height + 1);
}

void LevelGenerator::blockify(Level* level, vector<vector<bool>>& blocks, int width, int height)
{
    int to = 0;
    if (type == LevelGenerator::TYPE_CASTLE)
    {
        to = 4 * 2;
    }
    else if (type == LevelGenerator::TYPE_UNDERGROUND)
    {
        to = 4 * 3;
    }

    bool b[2][2];
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            for (int xx = x; xx <= x + 1; xx++)
            {
                for (int yy = y; yy <= y + 1; yy++)
                {
                    int _xx = xx;
                    int _yy = yy;
                    if (_xx < 0) _xx = 0;
                    if (_yy < 0) _yy = 0;
                    if (_xx > width - 1) _xx = width - 1;
                    if (_yy > height - 1) _yy = height - 1;
                    b[xx - x][yy - y] = blocks[_xx][_yy];
                }
            }

            if (b[0][0] == b[1][0] && b[0][1] == b[1][1])
            {
                if (b[0][0] == b[0][1])
                {
                    if (b[0][0])
                    {
                        level->setBlock(x, y, (uint8_t)(1 + 9 * 16 + to));
                    }
                    else
                    {
                        // KEEP OLD BLOCK!
                    }
                }
                else
                {
                    if (b[0][0])
                    {
                        level->setBlock(x, y, (uint8_t)(1 + 10 * 16 + to));
                    }
                    else
                    {
                        level->setBlock(x, y, (uint8_t)(1 + 8 * 16 + to));
                    }
                }
            }
            else if (b[0][0] == b[0][1] && b[1][0] == b[1][1])
            {
                if (b[0][0])
                {
                    level->setBlock(x, y, (uint8_t)(2 + 9 * 16 + to));
                }
                else
                {
                    level->setBlock(x, y, (uint8_t)(0 + 9 * 16 + to));
                }
            }
            else if (b[0][0] == b[1][1] && b[0][1] == b[1][0])
            {
                level->setBlock(x, y, (uint8_t)(1 + 9 * 16 + to));
            }
            else if (b[0][0] == b[1][0])
            {
                if (b[0][0])
                {
                    if (b[0][1])
                    {
                        level->setBlock(x, y, (uint8_t)(3 + 10 * 16 + to));
                    }
                    else
                    {
                        level->setBlock(x, y, (uint8_t)(3 + 11 * 16 + to));
                    }
                }
                else
                {
                    if (b[0][1])
                    {
                        level->setBlock(x, y, (uint8_t)(2 + 8 * 16 + to));
                    }
                    else
                    {
                        level->setBlock(x, y, (uint8_t)(0 + 8 * 16 + to));
                    }
                }
            }
            else if (b[0][1] == b[1][1])
            {
                if (b[0][1])
                {
                    if (b[0][0])
                    {
                        level->setBlock(x, y, (uint8_t)(3 + 9 * 16 + to));
                    }
                    else
                    {
                        level->setBlock(x, y, (uint8_t)(3 + 8 * 16 + to));
                    }
                }
                else
                {
                    if (b[0][0])
                    {
                        level->setBlock(x, y, (uint8_t)(2 + 10 * 16 + to));
                    }
                    else
                    {
                        level->setBlock(x, y, (uint8_t)(0 + 10 * 16 + to));
                    }
                }
            }
            else
            {
                level->setBlock(x, y, (uint8_t)(0 + 1 * 16 + to));
            }
        }
    }
}

int LevelGenerator::buildZone(int x, int maxLength, Level* level)
{
    int t = rand() % totalOdds;
    int type = 0;
    for (int i = 0; i < 5; i++)
    {
        if (odds[i] <= t)
        {
            type = i;
        }
    }

    switch (type)
    {
    case ODDS_STRAIGHT:
        return buildStraight(x, maxLength, false, level);
    case ODDS_HILL_STRAIGHT:
        return buildHillStraight(x, maxLength, level);
    case ODDS_TUBES:
        return buildTubes(x, maxLength, level);
    case ODDS_JUMP:
        return buildJump(x, maxLength, level);
    case ODDS_CANNONS:
        return buildCannons(x, maxLength, level);
    }
    return 0;
}

int LevelGenerator::buildHillStraight(int xo, int maxLength, Level* level)
{
    int length = rand() % 10 + 10;
    if (length > maxLength) length = maxLength;

    int floor = height - 1 - rand() % 4;
    for (int x = xo; x < xo + length; x++)
    {
        for (int y = 0; y < height; y++)
        {
            if (y >= floor)
            {
                level->setBlock(x, y, (uint8_t)(1 + 9 * 16));
            }
        }
    }

    addEnemyLine(xo + 1, xo + length - 1, floor - 1, level);

    int h = floor;

    bool keepGoing = true;

    vector<bool> occupied;
    occupied.assign(length, false);
    while (keepGoing)
    {
        h = h - 2 - rand() % 3;

        if (h <= 0)
        {
            keepGoing = false;
        }
        else
        {
            int l = rand() % 5 + 3;
            int xxo = rand() % (length - l - 2) + xo + 1;

            if (occupied[xxo - xo] || occupied[xxo - xo + l] || occupied[xxo - xo - 1] || occupied[xxo - xo + l + 1])
            {
                keepGoing = false;
            }
            else
            {
                occupied[xxo - xo] = true;
                occupied[xxo - xo + l] = true;
                addEnemyLine(xxo, xxo + l, h - 1, level);
                if (rand() % 4 == 0)
                {
                    decorate(xxo - 1, xxo + l + 1, h, level);
                    keepGoing = false;
                }
                for (int x = xxo; x < xxo + l; x++)
                {
                    for (int y = h; y < floor; y++)
                    {
                        int xx = 5;
                        if (x == xxo) xx = 4;
                        if (x == xxo + l - 1) xx = 6;
                        int yy = 9;
                        if (y == h) yy = 8;

                        if (level->getBlock(x, y) == 0)
                        {
                            level->setBlock(x, y, (uint8_t)(xx + yy * 16));
                        }
                        else
                        {
                            if (level->getBlock(x, y) == (uint8_t)(4 + 8 * 16)) level->setBlock(x, y, (uint8_t)(4 + 11 * 16));
                            if (level->getBlock(x, y) == (uint8_t)(6 + 8 * 16)) level->setBlock(x, y, (uint8_t)(6 + 11 * 16));
                        }
                    }
                }
            }
        }
    }

    return length;
}

int LevelGenerator::buildTubes(int xo, int maxLength, Level* level)
{
    int length = rand() % 10 + 5;
    if (length > maxLength) length = maxLength;

    int floor = height - 1 - rand() % 4;
    int xTube = xo + 1 + rand() % 4;
    int tubeHeight = floor - rand() % 2 - 2;
    for (int x = xo; x < xo + length; x++)
    {
        if (x > xTube + 1)
        {
            xTube += 3 + rand() % 4;
            tubeHeight = floor - rand() % 2 - 2;
        }
        if (xTube >= xo + length - 2) xTube += 10;

        if (x == xTube && rand() % 11 < difficulty + 1)
        {
            level->setSpriteTemplate(x, tubeHeight, Enemy::ENEMY_FLOWER, false);
        }

        for (int y = 0; y < height; y++)
        {
            if (y >= floor)
            {
                level->setBlock(x, y, (uint8_t)(1 + 9 * 16));
            }
            else
            {
                if ((x == xTube || x == xTube + 1) && y >= tubeHeight)
                {
                    int xPic = 10 + x - xTube;
                    if (y == tubeHeight)
                    {
                        level->setBlock(x, y, (uint8_t)(xPic + 0 * 16));
                    }
                    else
                    {
                        level->setBlock(x, y, (uint8_t)(xPic + 1 * 16));
                    }
                }
            }
        }
    }

    return length;
}

int LevelGenerator::buildJump(int xo, int maxLength, Level* level)
{
    int js = rand() % 4 + 2;
    int jl = rand() % 2 + 2;
    int length = js * 2 + jl;

    bool hasStairs = rand() % 3 == 0;

    int floor = height - 1 - rand() % 4;
    for (int x = xo; x < xo + length; x++)
    {
        if (x < xo + js || x > xo + length - js - 1)
        {
            for (int y = 0; y < height; y++)
            {
                if (y >= floor)
                {
                    level->setBlock(x, y, (uint8_t)(1 + 9 * 16));
                }
                else if (hasStairs)
                {
                    if (x < xo + js)
                    {
                        if (y >= floor - (x - xo) + 1)
                        {
                            level->setBlock(x, y, (uint8_t)(9 + 0 * 16));
                        }
                    }
                    else
                    {
                        if (y >= floor - ((xo + length) - x) + 2)
                        {
                            level->setBlock(x, y, (uint8_t)(9 + 0 * 16));
                        }
                    }
                }
            }
        }
    }

    return length;
}

int LevelGenerator::buildCannons(int xo, int maxLength, Level* level)
{
    int length = rand() % 10 + 2;
    if (length > maxLength) length = maxLength;

    int floor = height - 1 - rand() % 4;
    int xCannon = xo + 1 + rand() % 4;
    for (int x = xo; x < xo + length; x++)
    {
        if (x > xCannon)
        {
            xCannon += 2 + rand() % 4;
        }
        if (xCannon == xo + length - 1) xCannon += 10;
        int cannonHeight = floor - rand() % 4 - 1;

        for (int y = 0; y < height; y++)
        {
            if (y >= floor)
            {
                level->setBlock(x, y, (uint8_t)(1 + 9 * 16));
            }
            else
            {
                if (x == xCannon && y >= cannonHeight)
                {
                    if (y == cannonHeight)
                    {
                        level->setBlock(x, y, (uint8_t)(14 + 0 * 16));
                    }
                    else if (y == cannonHeight + 1)
                    {
                        level->setBlock(x, y, (uint8_t)(14 + 1 * 16));
                    }
                    else
                    {
                        level->setBlock(x, y, (uint8_t)(14 + 2 * 16));
                    }
                }
            }
        }
    }

    return length;
}
