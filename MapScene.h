#ifndef MAPSCENE_H
#define MAPSCENE_H
#include "Scene.h"

class MarioComponent;

class MapScene : public Scene
{
public:
    MapScene(MarioComponent* component);
    MapScene(MapScene* mapScene);
    void copyMapSceneData(MapScene* mapScene);
    virtual void init();
    virtual void tick();
    virtual void render(QPainter& painter);
    void renderDynamic(QPainter& painter);
    void renderStatic(QPainter& painter);
    void drawStringDropShadow(QPainter& painter, string text, int x, int y, int c);
    void drawString(QPainter& painter, string text, int x, int y, int c);
    void nextWorld();
    bool generateLevel();
    void tryWalking(int xd, int yd);
    int calcDistance(int x, int y, int xa, int ya);
    void levelWon();
private:
    void findCaps(int width, int height);
    bool findConnection(int width, int height);
    void connect(int xSource, int ySource, int width, int height);
    void travel(int x, int y, int dir, int depth);
    void drawRoad(int x0, int y0, int x1, int y1);

    bool isRoad(int x, int y);
    bool isWater(int x, int y);

    static const int TILE_GRASS = 0;
    static const int TILE_WATER = 1;
    static const int TILE_LEVEL = 2;
    static const int TILE_ROAD = 3;
    static const int TILE_DECORATION = 4;

    MarioComponent *marioComponent;
    vector<vector<int>> level;
    vector<vector<int>> data;
    int xMario = 0;
    int yMario = 0;
    int xMarioA = 0;
    int yMarioA = 0;
    long long llTick = 0;
    int moveTime = 0;
    int worldNumber;
    int levelId = 0;
    int farthest = 0;
    int xFarthestCap = 0;
    int yFarthestCap = 0;
    bool canEnterLevel = false;
};

#endif // MAPSCENE_H
