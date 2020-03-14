#ifndef MARIO_LEVELGENERATOR_H
#define MARIO_LEVELGENERATOR_H

#include "Level.h"

class LevelGenerator
{
public:
	LevelGenerator(int width, int height);

	static const int TYPE_OVERGROUND = 0;
	static const int TYPE_UNDERGROUND = 1;
	static const int TYPE_CASTLE = 2;

	static const int ODDS_STRAIGHT = 0;
	static const int ODDS_HILL_STRAIGHT = 1;
	static const int ODDS_TUBES = 2;
	static const int ODDS_JUMP = 3;
	static const int ODDS_CANNONS = 4;

    static void createLevel(int width, int height, int difficulty, int type, Level* level);
    void createLevel(int difficulty, int type, Level* level);
    int buildStraight(int xo, int maxLength, bool safe, Level* level);
    void decorate(int x0, int x1, int floor, Level* level);
    void addEnemyLine(int x0, int x1, int y, Level* level);
    void fixWalls(Level* level);
	void blockify(Level* level, vector<vector<bool>>& blockMap, int width, int height);
    int buildZone(int x, int maxLength, Level* level);
    int buildHillStraight(int xo, int maxLength, Level* level);
    int buildTubes(int xo, int maxLength, Level* level);
    int buildJump(int xo, int maxLength, Level* level);
    int buildCannons(int xo, int maxLength, Level* level);

private:
	int width;
	int height;
	int odds[5];
    int totalOdds = 0;
	int difficulty;
    int type;

};

#endif
