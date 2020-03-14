#ifndef MARIO_ART_H
#define MARIO_ART_H

#include<vector>
#include<map>
#include<string>
#include<QImage>
using namespace std;

class Art
{
public:
    static const int SAMPLE_BREAK_BLOCK = 0;
    static const int SAMPLE_GET_COIN = 1;
    static const int SAMPLE_MARIO_JUMP = 2;
    static const int SAMPLE_MARIO_STOMP = 3;
    static const int SAMPLE_MARIO_KICK = 4;
    static const int SAMPLE_MARIO_POWER_UP = 5;
    static const int SAMPLE_MARIO_POWER_DOWN = 6;
    static const int SAMPLE_MARIO_DEATH = 7;
    static const int SAMPLE_ITEM_SPROUT = 8;
    static const int SAMPLE_CANNON_FIRE = 9;
    static const int SAMPLE_SHELL_BUMP = 10;
    static const int SAMPLE_LEVEL_EXIT = 11;
    static const int SAMPLE_MARIO_1UP = 12;
    static const int SAMPLE_MARIO_FIREBALL = 13;

    static vector<vector<QImage>> mario;
    static vector<vector<QImage>> smallMario;
    static vector<vector<QImage>> fireMario;
    static vector<vector<QImage>> enemies;
    static vector<vector<QImage>> items;
    static vector<vector<QImage>> level;
    static vector<vector<QImage>> particles;
    static vector<vector<QImage>> font;
    static vector<vector<QImage>> bg;
    static vector<vector<QImage>> map;
    static vector<vector<QImage>> endScene;
    static vector<vector<QImage>> gameOver;
    static QImage logo;
    static QImage titleScreen;
    static QImage tip;
    static vector<string> songs;
    static vector<string> samples;
    static void init();
    static void cutImage(const QString& path, int xSize, int ySize, vector<vector<QImage>>& image);
};

#endif
