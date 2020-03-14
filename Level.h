#ifndef MARIO_LEVEL_H
#define MARIO_LEVEL_H
#include<stdint.h>
#include<vector>
#include<memory>

using namespace std;

class SpriteTemplate;

class Level
{
public: 
    Level();
    Level(int width, int height);
    ~Level();
    uint8_t getBlock(int x, int y);
    void setBlock(int x, int y, uint8_t b);
    void setBlockData(int x, int y, uint8_t b);
    static void loadBehaviors();
    void setSpriteTemplate(int x, int y, int type, bool winged);
    SpriteTemplate* getSpriteTemplate(int x, int y);
    void printSpriteTemplateInfo();
    void tick();
    uint8_t getBlockCapped(int x, int y);
    bool isBlocking(int x, int y, float xa, float ya);

    static const char* BIT_DESCRIPTIONS[];
    static uint8_t TILE_BEHAVIORS[256];

    static const int BIT_BLOCK_UPPER = 1 << 0;  //向上被堵住
    static const int BIT_BLOCK_ALL = 1 << 1;    //各个方向都被堵住
    static const int BIT_BLOCK_LOWER = 1 << 2;  //向下被堵住
    static const int BIT_SPECIAL = 1 << 3;      //撞击后能够出现蘑菇或者花
    static const int BIT_BUMPABLE = 1 << 4;     //能够用头撞击
    static const int BIT_BREAKABLE = 1 << 5;    //能够被撞毁
    static const int BIT_PICKUPABLE = 1 << 6;   //能够获得金币
    static const int BIT_ANIMATED = 1 << 7;     //显示动画

    static const int FILE_HEADER = 0x271c4178;

    int width;
    int height;

    vector<vector<uint8_t>> map;    //每个方格对应的障碍物类型，如砖头、金币、敌人等等
    vector<vector<uint8_t>> data;   //撞击后，砖头向上移动的距离

    vector<vector<shared_ptr<SpriteTemplate>>> spriteTemplates;

    int xExit;
    int yExit;
};

#endif
