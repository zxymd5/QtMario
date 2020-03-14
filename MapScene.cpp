#include "MapScene.h"
#include "MarioComponent.h"
#include "ImprovedNoise.h"
#include "Mario.h"
#include "LevelGenerator.h"
#include "Art.h"
#include "SimpleAudioEngine.h"

#include<sstream>
using namespace std;

MapScene::MapScene(MarioComponent *component)
{
    this->marioComponent = component;
}

MapScene::MapScene(MapScene *mapScene)
{
    copyMapSceneData(mapScene);
}

void MapScene::copyMapSceneData(MapScene *mapScene)
{
    marioComponent = mapScene->marioComponent;

    int width = mapScene->level.size();
    int height = mapScene->level[0].size();
    level.resize(width);
    data.resize(width);

    for(int i = 0; i < width; i++) {
        level[i].resize(height);
        data[i].resize(height);
    }

    for(int i = 0; i < width; i++) {
        for(int j = 0; j < height; j++) {
            level[i][j] = mapScene->level[i][j];
            data[i][j] = mapScene->data[i][j];
        }
    }

    xMario = mapScene->xMario;
    yMario = mapScene->yMario;
    xMarioA = mapScene->xMarioA;
    yMarioA = mapScene->yMarioA;
    llTick = mapScene->llTick;
    moveTime = mapScene->moveTime;
    worldNumber = mapScene->worldNumber;
    levelId = mapScene->levelId;
    farthest = mapScene->farthest;
    xFarthestCap = mapScene->xFarthestCap;
    yFarthestCap = mapScene->yFarthestCap;
    canEnterLevel = mapScene->canEnterLevel;
}

void MapScene::init()
{
    worldNumber = -1;
    nextWorld();
}

void MapScene::tick()
{
    xMario += xMarioA;
    yMario += yMarioA;
    llTick++;
    int x = xMario / 32;
    int y = yMario / 32;
    if (level[x][y] == TILE_ROAD)
    {
        data[x][y] = 0;
    }

    if (moveTime > 0)
    {
        moveTime--;
    }
    else
    {
        xMarioA = 0;
        yMarioA = 0;
        if (canEnterLevel && keys[Qt::Key_S])
        {
            if (level[x][y] == TILE_LEVEL && data[x][y] == -11)
            {
            }
            else
            {
                if (level[x][y] == TILE_LEVEL && data[x][y] != 0 && data[x][y] > -10)
                {
                    stringstream ss;
                    ss << (worldNumber + 1) << "-";

                    Mario::levelString = ss.str();
                    int difficulty = worldNumber+1;
                    int type = LevelGenerator::TYPE_OVERGROUND;
                    if (data[x][y] > 1 && rand() % 3 == 0)
                    {
                        type = LevelGenerator::TYPE_UNDERGROUND;
                    }
                    if (data[x][y] < 0)
                    {
                        if (data[x][y] == -2)
                        {
                            Mario::levelString += "X";
                            difficulty += 2;
                        }
                        else if (data[x][y] == -1)
                        {
                            Mario::levelString += "?";
                        }
                        else
                        {
                            Mario::levelString += "#";
                            difficulty += 1;
                        }

                        type = LevelGenerator::TYPE_CASTLE;
                    }
                    else
                    {
                        stringstream ss;
                        ss << data[x][y];
                        Mario::levelString += ss.str();
                    }

                    marioComponent->startLevel(difficulty, type);
                }
            }
        }
        canEnterLevel = !keys[Qt::Key_S];

        if (keys[Qt::Key_Left])
        {
            keys[Qt::Key_Left] = false;
            tryWalking(-1, 0);
        }
        if (keys[Qt::Key_Right])
        {
            keys[Qt::Key_Right] = false;
            tryWalking(1, 0);
        }
        if (keys[Qt::Key_Up])
        {
            keys[Qt::Key_Up] = false;
            tryWalking(0, -1);
        }
        if (keys[Qt::Key_Down])
        {
            keys[Qt::Key_Down] = false;
            tryWalking(0, 1);
        }
    }
}

void MapScene::render(QPainter& painter)
{
    renderStatic(painter);
    renderDynamic(painter);
}

void MapScene::renderDynamic(QPainter& painter)
{
    for (int y = 0; y <= 240 / 16; y++)
    {
        for (int x = 320 / 16; x >= 0; x--)
        {
            if (level[x][y] == TILE_WATER)
            {
                if (isWater(x * 2 - 1, y * 2 - 1))
                {
                    painter.drawImage(QRectF(x * 32 - 16, y * 32 - 16, 32, 32), Art::map[15][4 + (llTick / 6 + y) % 4]);
                }
            }
            else if (level[x][y] == TILE_DECORATION)
            {
                painter.drawImage(QRectF(x * 32, y * 32, 32, 32), Art::map[(llTick + y * 12) / 6 % 4][10 + worldNumber % 4]);
            }
            else if (level[x][y] == TILE_LEVEL && data[x][y] == -2 && llTick / 12 % 2 == 0)
            {
                painter.drawImage(QRectF((x + 1) * 32, (y - 1) * 32, 32, 32), Art::map[3][7]);
            }
        }
    }
    if (!Mario::large)
    {
        painter.drawImage(QRectF(xMario, yMario - 12, 32, 32), Art::map[(llTick) / 6 % 2][1]);
    }
    else
    {
        if (!Mario::fire)
        {
            painter.drawImage(QRectF(xMario, yMario - 12 - 32, 32, 32), Art::map[(llTick) / 6 % 2 + 2][0]);

            painter.drawImage(QRectF(xMario, yMario - 12, 32, 32), Art::map[(llTick) / 6 % 2 + 2][1]);
        }
        else
        {
            painter.drawImage(QRectF(xMario, yMario - 12 - 32, 32, 32), Art::map[(llTick) / 6 % 2 + 4][0]);
            painter.drawImage(QRectF(xMario, yMario - 12, 32, 32), Art::map[(llTick) / 6 % 2 + 4][1]);
        }
    }

    char szBuff[128];
    sprintf(szBuff, "MARIO %d", Mario::lives);
    drawStringDropShadow(painter, szBuff, 0, 0, 7);
    sprintf(szBuff, "WORLD %d", worldNumber+1);
    drawStringDropShadow(painter, szBuff, 32, 0, 7);
}

void MapScene::renderStatic(QPainter& painter)
{
    for (int x = 0; x < 320 / 16; x++)
    {
        for (int y = 0; y < 240 / 16; y++)
        {
            painter.drawImage(QRectF(x * 32, y * 32, 32, 32), Art::map[worldNumber / 4][0]);

            if (level[x][y] == TILE_LEVEL) {
                int type = data[x][y];
                if (type == 0)
                {
                    painter.drawImage(QRectF(x * 32, y * 32, 32, 32), Art::map[0][7]);
                }
                else if (type == -1)
                {
                    painter.drawImage(QRectF(x * 32, y * 32, 32, 32), Art::map[3][8]);
                }
                else if (type == -3)
                {
                    painter.drawImage(QRectF(x * 32, y * 32, 32, 32), Art::map[0][8]);
                }
                else if (type == -10)
                {
                    painter.drawImage(QRectF(x * 32, y * 32, 32, 32), Art::map[1][8]);
                }
                else if (type == -11)
                {
                    painter.drawImage(QRectF(x * 32, y * 32, 32, 32), Art::map[1][7]);
                }
                else if (type == -2)
                {
                    painter.drawImage(QRectF(x * 32, (y - 1) * 32, 32, 32), Art::map[2][7]);
                    painter.drawImage(QRectF(x * 32, y * 32, 32, 32), Art::map[2][8]);
                }
                else
                {
                    painter.drawImage(QRectF(x * 32, y * 32, 32, 32), Art::map[type - 1][6]);
                }
            }
            else if (level[x][y] == TILE_ROAD) {
                int p0 = isRoad(x - 1, y) ? 1 : 0;
                int p1 = isRoad(x, y - 1) ? 1 : 0;
                int p2 = isRoad(x + 1, y) ? 1 : 0;
                int p3 = isRoad(x, y + 1) ? 1 : 0;
                int s = p0 + p1 * 2 + p2 * 4 + p3 * 8;
                painter.drawImage(QRectF(x * 32, y * 32, 32, 32), Art::map[s][2]);
            }
            else if (level[x][y] == TILE_WATER)
            {
                for (int xx = 0; xx < 2; xx++)
                {
                    for (int yy = 0; yy < 2; yy++)
                    {
                        int p0 = isWater(x * 2 + (xx - 1), y * 2 + (yy - 1)) ? 0 : 1;
                        int p1 = isWater(x * 2 + (xx + 0), y * 2 + (yy - 1)) ? 0 : 1;
                        int p2 = isWater(x * 2 + (xx - 1), y * 2 + (yy + 0)) ? 0 : 1;
                        int p3 = isWater(x * 2 + (xx + 0), y * 2 + (yy + 0)) ? 0 : 1;
                        int s = p0 + p1 * 2 + p2 * 4 + p3 * 8 - 1;
                        if (s >= 0 && s < 14)
                        {
                            painter.drawImage(QRectF(x * 32 + xx * 16, y * 32 + yy * 16, 32, 32), Art::map[s][4 + ((xx + yy) & 1)]);
                        }
                    }
                }
            }
        }
    }
}

void MapScene::drawStringDropShadow(QPainter &painter, string text, int x, int y, int c)
{
    drawString(painter, text, x * 16 + 10, y * 16 + 10, 0);
    drawString(painter, text, x * 16 + 8, y * 16 + 8, c);
}

void MapScene::drawString(QPainter &painter, string text, int x, int y, int c)
{
    for(int i = 0; i < text.length(); i++) {
        QImage img = Art::font[text[i] - 32][c];
        QRectF rect(x + i * img.width() * 2, y,
                    img.width() * 2, img.height() * 2);
        painter.drawImage(rect, img);
    }
}

void MapScene::nextWorld()
{
    worldNumber++;

    if (worldNumber==8)
    {
        marioComponent->win();
        return;
    }

    moveTime = 0;
    levelId = 0;
    farthest = 0;
    xFarthestCap = 0;
    yFarthestCap = 0;

    while (!generateLevel());
}

bool MapScene::generateLevel()
{
    ImprovedNoise n0;
    ImprovedNoise n1;
    ImprovedNoise dec;

    int width = 320 / 16 + 1;
    int height = 240 / 16 + 1;

    level.resize(width);
    data.resize(width);

    for(int i = 0; i < width; i++) {
        level[i].resize(height);
        data[i].resize(height);
    }

    for(int i = 0; i < width; i++) {
        for(int j = 0; j < height; j++) {
            level[i][j] = 0;
            data[i][j] = 0;
        }
    }

    double xo0 = (double)rand() / ((double)RAND_MAX) * 512.00;
    double yo0 = (double)rand() / ((double)RAND_MAX) * 512.00;
    double xo1 = (double)rand() / ((double)RAND_MAX) * 512.00;
    double yo1 = (double)rand() / ((double)RAND_MAX) * 512.00;
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            double t0 = n0.perlinNoise(x * 10.0 + xo0, y * 10.0 + yo0);
            double t1 = n1.perlinNoise(x * 10.0 + xo1, y * 10.0 + yo1);
            double td = (t0 - t1);
            double t = (td * 2);
            level[x][y] = t > 0 ? TILE_WATER : TILE_GRASS;
        }
    }

    int lowestX = 9999;
    int lowestY = 9999;
    int t = 0;
    for (int i = 0; i < 100 && t < 12; i++)
    {
        int x = (rand() % ((width - 1) / 3)) * 3 + 2;
        int y = (rand() % ((height - 1) / 3)) * 3 + 1;
        if (level[x][y] == TILE_GRASS)
        {
            if (x < lowestX)
            {
                lowestX = x;
                lowestY = y;
            }
            level[x][y] = TILE_LEVEL;
            data[x][y] = -1;
            t++;
        }
    }

    data[lowestX][lowestY] = -2;
    while (findConnection(width, height))
        ;

    findCaps(width, height);

    if (xFarthestCap == 0) return false;

    data[xFarthestCap][yFarthestCap] = -2;
    data[xMario / 32][yMario / 32] = -11;

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            if (level[x][y] == TILE_GRASS && (x != xFarthestCap || y != yFarthestCap - 1))
            {
                double t0 = dec.perlinNoise(x * 10.0 + xo0, y * 10.0 + yo0);
                if (t0 > 0) level[x][y] = TILE_DECORATION;
            }
        }
    }

    return true;
}

void MapScene::tryWalking(int xd, int yd)
{
    int x = xMario / 32;
    int y = yMario / 32;
    int xt = xMario / 32 + xd;
    int yt = yMario / 32 + yd;

    if (level[xt][yt] == TILE_ROAD || level[xt][yt] == TILE_LEVEL)
    {
        if (level[xt][yt] == TILE_ROAD)
        {
            if ((data[xt][yt] != 0) && (data[x][y] != 0 && data[x][y] > -10)) return;
        }
        xMarioA = xd * 16;
        yMarioA = yd * 16;
        moveTime = calcDistance(x, y, xd, yd) * 2 + 1;
    }
}

int MapScene::calcDistance(int x, int y, int xa, int ya)
{
    int distance = 0;
    while (true)
    {
        x += xa;
        y += ya;
        if (level[x][y] != TILE_ROAD) return distance;
        if (level[x - ya][y + xa] == TILE_ROAD) return distance;
        if (level[x + ya][y - xa] == TILE_ROAD) return distance;
        distance++;
    }
}

void MapScene::levelWon()
{
    int x = xMario / 32;
    int y = yMario / 32;
    if (data[x][y] == -2)
    {
        nextWorld();
        return;
    }
    if (data[x][y] != -3) data[x][y] = 0;
    else data[x][y] = -10;
}

void MapScene::findCaps(int width, int height)
{
    int xCap = -1;
    int yCap = -1;

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            if (level[x][y] == TILE_LEVEL)
            {
                int roads = 0;
                for (int xx = x - 1; xx <= x + 1; xx++)
                    for (int yy = y - 1; yy <= y + 1; yy++)
                    {
                        if (level[xx][yy] == TILE_ROAD) roads++;
                    }

                if (roads == 1)
                {
                    if (xCap == -1)
                    {
                        xCap = x;
                        yCap = y;
                    }
                    data[x][y] = 0;
                }
                else
                {
                    data[x][y] = 1;
                }
            }
        }
    }

    xMario = xCap * 32;
    yMario = yCap * 32;

    travel(xCap, yCap, -1, 0);
}

bool MapScene::findConnection(int width, int height)
{
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            if (level[x][y] == TILE_LEVEL && data[x][y] == -1)
            {
                connect(x, y, width, height);
                return true;
            }
        }
    }
    return false;
}

void MapScene::travel(int x, int y, int dir, int depth)
{
    if (level[x][y] != TILE_ROAD && level[x][y] != TILE_LEVEL)
    {
        return;
    }
    if (level[x][y] == TILE_ROAD)
    {
        if (data[x][y] == 1) return;
        else data[x][y] = 1;
    }

    if (level[x][y] == TILE_LEVEL)
    {
        if (data[x][y] > 0)
        {
            if (levelId != 0 && rand() % (4) == 0)
            {
                data[x][y] = -3;
            }
            else
            {
                data[x][y] = ++levelId;
            }
        }
        else if (depth > 0)
        {
            data[x][y] = -1;
            if (depth > farthest)
            {
                farthest = depth;
                xFarthestCap = x;
                yFarthestCap = y;
            }
        }
    }

    if (dir != 2) travel(x - 1, y, 0, depth++);
    if (dir != 3) travel(x, y - 1, 1, depth++);
    if (dir != 0) travel(x + 1, y, 2, depth++);
    if (dir != 1) travel(x, y + 1, 3, depth++);
}

void MapScene::drawRoad(int x0, int y0, int x1, int y1)
{
    bool xFirst = rand() % 2;

    if (xFirst)
    {
        while (x0 > x1)
        {
            data[x0][y0] = 0;
            level[x0--][y0] = TILE_ROAD;
        }
        while (x0 < x1)
        {
            data[x0][y0] = 0;
            level[x0++][y0] = TILE_ROAD;
        }
    }
    while (y0 > y1)
    {
        data[x0][y0] = 0;
        level[x0][y0--] = TILE_ROAD;
    }
    while (y0 < y1)
    {
        data[x0][y0] = 0;
        level[x0][y0++] = TILE_ROAD;
    }
    if (!xFirst)
    {
        while (x0 > x1)
        {
            data[x0][y0] = 0;
            level[x0--][y0] = TILE_ROAD;
        }
        while (x0 < x1)
        {
            data[x0][y0] = 0;
            level[x0++][y0] = TILE_ROAD;
        }
    }
}

bool MapScene::isRoad(int x, int y)
{
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (level[x][y] == TILE_ROAD) return true;
    if (level[x][y] == TILE_LEVEL) return true;
    return false;
}

bool MapScene::isWater(int x, int y)
{
    if (x < 0) x = 0;
    if (y < 0) y = 0;

    for (int xx = 0; xx < 2; xx++)
    {
        for (int yy = 0; yy < 2; yy++)
        {
            if (level[(x + xx) / 2][(y + yy) / 2] != TILE_WATER) return false;
        }
    }

    return true;
}

void MapScene::connect(int xSource, int ySource, int width, int height)
{
    int maxDist = 10000;
    int xTarget = 0;
    int yTarget = 0;
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            if (level[x][y] == TILE_LEVEL && data[x][y] == -2)
            {
                int xd = abs(xSource - x);
                int yd = abs(ySource - y);
                int d = xd * xd + yd * yd;
                if (d < maxDist)
                {
                    xTarget = x;
                    yTarget = y;
                    maxDist = d;
                }
            }
        }
    }

    drawRoad(xSource, ySource, xTarget, yTarget);
    level[xSource][ySource] = TILE_LEVEL;
    data[xSource][ySource] = -2;
    return;
}
